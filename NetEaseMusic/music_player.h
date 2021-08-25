#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include"libswresample/swresample.h"
#include "libavutil/imgutils.h"
#include "SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>
#ifdef __cplusplus
};
#endif
#endif


#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <memory>


template <class T>
class ThreadSafeQueueTemp
{
public:
    ThreadSafeQueueTemp(){}
    ThreadSafeQueueTemp(ThreadSafeQueueTemp const& other)
    {
        std::lock_guard<std::mutex> lk(other._mutex);
        _queue = other._queue;
    }

    void push(T newData)
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _queue.push(newData);
        _dataCond.notify_one();

    }

    T waitAndPop()
    {
        std::lock_guard<std::mutex> lk(_mutex);
        _dataCond.wait(lk,[this]{return !_queue.empty();});
        T t = _queue.front();
        _queue.pop();
        return t;

    }

    T tryPop()
    {
        std::lock_guard<std::mutex> lk(_mutex);
        if(_queue.empty())
        {
            return T();
        }
        T t =  _queue.front();
        _queue.pop();
        return t;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lk(_mutex);
        while(!_queue.empty())
        {
            _queue.pop();
        }

    }

public:
    std::queue<T> _queue;

private:

    std::condition_variable _dataCond;
    std::mutex _mutex;
};

class AVPacketQueue:public ThreadSafeQueueTemp<AVPacket>{};


class MusicPlayer
{
public:
    static MusicPlayer& getSingleton(){
        static MusicPlayer musicPlayer;
        return musicPlayer;
    }

    MusicPlayer(const MusicPlayer& musicPlayer)=delete ;

    MusicPlayer& operator=(const MusicPlayer& musicPlayer)=delete ;

    void openMusicFile(std::string path);       // todo: 中文字符问题

    void play();

    void stop();

    void nextSong();

    void preSong();

    void threadProducePacketBegin();


public:
    static AVFormatContext* _pFormatCtx;

    static AVCodecContext* _pCodecCtx;

    static AVPacketQueue _playQueue;

    static AVFrame* _pWantedFrame;

    static int _volume;

    static int _audioIndex;

    void initSDL();


private:
    MusicPlayer();

    void producePakcet();



    static int decode(uint8_t* audio_buf);

    static void fillAudio(void* udata,Uint8* stream,int len);


private:

    std::string _musicFilePath;


    AVCodec* _pCodec;




    //Out Audio Param
    uint64_t _outChannelLayout;
    //nb_samples: AAC-1024 MP3-1152
    int _outNbSamples;
    AVSampleFormat _outSampleFmt;
    int _outSampleRate;
    int _outChannels;
    //Out Buffer Size
    int _outBufferSize;

    uint8_t* _pOutBuffer;

    SDL_AudioSpec _wantedSpec;

    std::thread _threadProducePacket;

    std::atomic<bool> _aIsPlaying;


    uint64_t _millisecondSeek;  // 跳转到某一播放进度

    uint64_t _progress;         // 播放进度

    enum PlayStatus
    {
        PLAYING,
        SEEK,   // 调整播放进度
        FINISH,
        PAUSE
    };

    std::mutex _mtxStatus;
    PlayStatus _status = FINISH;


};



#endif // MUSICPLAYER_H
