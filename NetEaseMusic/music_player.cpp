#include "music_player.h"
#include <QDebug>

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

MusicPlayer::MusicPlayer()
{
    initSDL();
}

void MusicPlayer::initSDL()
{
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        qDebug()<<"Could not initialize SDL:"<<SDL_GetError()<<endl;
    }
}

void MusicPlayer::openMusicFile(std::string path)
{
    if(path.empty() || path == _musicFilePath)
        return;

    qDebug()<<"song path:"<<path.c_str()<<endl;

    av_register_all();
    avformat_network_init();
    _pFormatCtx = avformat_alloc_context();

    if(0 != avformat_open_input(&_pFormatCtx,path.c_str(),nullptr,nullptr))
    {
        qDebug()<<"Couldn't open input stream."<<endl;
    }
    if(avformat_find_stream_info(_pFormatCtx,nullptr)<0)
    {
        qDebug()<<"Couldn't find stream information."<<endl;
    }
    _audioIndex = -1;
    for(int i=0; i<_pFormatCtx->nb_streams; ++i)
    {
        if(_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            _audioIndex = i;
            break;
        }
    }
    if(-1 == _audioIndex)
    {
        qDebug()<<"Didn't find a video stream."<<endl;
    }
    _pCodecCtx = _pFormatCtx->streams[_audioIndex]->codec;
    _pCodec = avcodec_find_decoder(_pCodecCtx->codec_id);
    if(nullptr == _pCodec)
    {
        qDebug()<<"Codec not found."<<endl;
    }
    if(avcodec_open2(_pCodecCtx,_pCodec,nullptr)<0)
    {
        qDebug()<<"Could not open codec."<<endl;
    }

    _outChannelLayout = AV_CH_LAYOUT_STEREO;
    _outNbSamples = _pCodecCtx->frame_size;
    _outSampleFmt = AV_SAMPLE_FMT_S16;
    _outSampleRate = 44100;
    _outChannels = av_get_channel_layout_nb_channels(_outChannelLayout);
    _outBufferSize = av_samples_get_buffer_size(nullptr,_outChannels,_outNbSamples,_outSampleFmt,1);

    _pOutBuffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE*2);

//init sdl before below code;

    _wantedSpec.freq = _outSampleRate;
    _wantedSpec.format = AUDIO_S16SYS;
    _wantedSpec.channels = _outChannels;
    _wantedSpec.silence = 0;
    _wantedSpec.samples = _outNbSamples;
    _wantedSpec.size = _outBufferSize;
    _wantedSpec.callback = fillAudio;

    if(SDL_OpenAudio(&_wantedSpec,nullptr)<0)
    {
        qDebug()<<"SDL_OpenAudio error."<<endl;
    }

    _pWantedFrame = av_frame_alloc();
    _pWantedFrame->channel_layout = _outChannelLayout;
    _pWantedFrame->format = _outSampleFmt;
    _pWantedFrame->sample_rate = _outSampleRate;
    _pWantedFrame->channels = _outChannels;

    _musicFilePath = path;
    _playQueue.clear();

}



void MusicPlayer::threadProducePacketBegin()
{
    _threadProducePacket = std::thread(std::mem_fn(&MusicPlayer::producePakcet),this);
}

void MusicPlayer::producePakcet()
{
    std::unique_lock<std::mutex> lk(_mtxStatus);
    _status = PLAYING;
    lk.unlock();


    AVPacket avPacket;

    while(_aIsPlaying.load())
    {
        PlayStatus playStatus;
        std::unique_lock<std::mutex> lk(_mtxStatus);
        playStatus = _status;
        lk.unlock();

        switch (playStatus)
        {
        case PLAYING:
        {
            int res = av_read_frame(_pFormatCtx,&avPacket);
            if(0 == res)
            {
                if(avPacket.stream_index == _audioIndex)
                {
                    //qDebug()<<"produce"<<endl;
                    _playQueue.push(avPacket);
                }
                else
                {
                    av_free_packet(&avPacket);
                }
            }
            break;
        }
        case SEEK:
        {
            AVRational aVRational = {1, 1000};
            int64_t res = av_rescale_q(_millisecondSeek,aVRational,_pFormatCtx->streams[_audioIndex]->time_base);
            if(av_seek_frame(_pFormatCtx,_audioIndex,res,AVSEEK_FLAG_ANY)<0)
            {
                qDebug()<<"seek error"<<endl;
                // If the error occurs, try the fallback way.
                if(av_seek_frame(_pFormatCtx,_audioIndex,res,AVSEEK_FLAG_BACKWARD)<0){
                    _status = FINISH;
                }

            }
            _progress = _millisecondSeek;
            if(-1 != _audioIndex)
            {
                avcodec_flush_buffers(_pCodecCtx);
            }

            _status = PLAYING;

            break;
        }

        case PAUSE:
        {
            SDL_Delay(1000);
            break;
        }

        case FINISH:
        {
            _aIsPlaying.store(false);
        }
        default:
            break;
        }
    }
}

void MusicPlayer::fillAudio(void* udata,Uint8* stream,int len)
{
    //qDebug()<<"fill Audio"<<endl;

    uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE];
    memset(stream, 0, len);
    static int audio_buf_pos=0;
    static int audio_buf_size=0;

    while(len > 0)
    {
        if(audio_buf_pos >= audio_buf_size)
        {
            audio_buf_size = decode(audio_buff);
            if(audio_buf_size < 0){
                //qDebug()<<"audio_buf_size < 0"<<endl;
                return;
            }
            audio_buf_pos = 0;
        }

        int audio_len = audio_buf_size - audio_buf_pos;
        if(audio_len > len)
            audio_len = len;

        SDL_MixAudio(stream,audio_buff + audio_buf_pos, audio_len, _volume);
        len -= audio_len;
        audio_buf_pos += audio_len;
        stream += audio_len;
    }
}

int MusicPlayer::decode(uint8_t* audio_buf)
{
    AVFrame *pFrame = nullptr;
    SwrContext *pSwrCtx = nullptr;
    //AVPacket avPacket;

    int ret = -1;

    auto avPacket = _playQueue.tryPop();
    if(0 == avPacket.size)
    {
        qDebug()<<"Play queue is empty."<<endl;
        return ret;
    }

    while(true)
    {
        if(avPacket.pts == AV_NOPTS_VALUE)
        {
            break;
        }

        AVRational avRational = {1,1000};
        qint64 res = av_rescale_q(avPacket.pts,_pFormatCtx->streams[_audioIndex]->time_base,avRational);

        if(avPacket.size > 0)
        {
            pFrame = av_frame_alloc();
            int gotFrame;
            int len = avcodec_decode_audio4(_pCodecCtx,pFrame,&gotFrame,&avPacket);

            if(len < 0)   break;

            if(pFrame->channels > 0 && pFrame->channel_layout == 0)
            {
                pFrame->channel_layout = av_get_default_channel_layout(pFrame->channels);
            }
            else if(pFrame->channels == 0 && pFrame->channel_layout > 0)
            {
                pFrame->channels = av_get_channel_layout_nb_channels(pFrame->channel_layout);
            }

            pSwrCtx = swr_alloc_set_opts(nullptr,_pWantedFrame->channel_layout,
                                         (AVSampleFormat)_pWantedFrame->format,
                                         _pWantedFrame->sample_rate,
                                         pFrame->channel_layout,
                                         (AVSampleFormat)pFrame->format,
                                         pFrame->sample_rate,0,nullptr);

            int errorNum = swr_init(pSwrCtx);
            if(0 != errorNum)
            {
                char errorBuff[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(errorNum, errorBuff, sizeof(errorBuff) - 1);
                qDebug() << "swr_init failed:" << errorBuff;
                break;
            }

            int dst_nb_samples = av_rescale_rnd(
                        swr_get_delay(
                            pSwrCtx,pFrame->sample_rate) + pFrame->nb_samples,
                            pFrame->sample_rate,
                            pFrame->sample_rate,
                            AVRounding(1)
            );
            int len2 = swr_convert(
                        pSwrCtx,
                        &audio_buf,
                        dst_nb_samples,
                        (const uint8_t**)pFrame->data,
                        pFrame->nb_samples
            );
            if(len2 < 0) break;

            ret = _pWantedFrame->channels * len2 * av_get_bytes_per_sample((AVSampleFormat)_pWantedFrame->format);
        }
        break;
    }


    swr_free(&pSwrCtx);
    av_frame_free(&pFrame);
    av_packet_unref(&avPacket);
    return ret;
}

void MusicPlayer::play()
{
    _aIsPlaying.store(true);

    //Play
    SDL_PauseAudio(0);

    std::unique_lock<std::mutex> lk(_mtxStatus);
    _status = PLAYING;
    lk.unlock();
}

void MusicPlayer::stop()
{
    SDL_PauseAudio(1);

    std::unique_lock<std::mutex> lk(_mtxStatus);
    _status = PAUSE;
    lk.unlock();
}

void MusicPlayer::nextSong()
{
    stop();

    SDL_CloseAudio();


    _aIsPlaying.store(false);
    if(_threadProducePacket.joinable())
        _threadProducePacket.join();


    avformat_free_context(_pFormatCtx);
    _pFormatCtx = nullptr;

    _pCodec = nullptr;
    _pCodecCtx = nullptr;
    _playQueue.clear();
    _pWantedFrame = nullptr;
    _audioIndex = -1;


    openMusicFile("D:/CloudMusic/test.mp3");

    threadProducePacketBegin();

    play();

}

void MusicPlayer::preSong()
{

}














