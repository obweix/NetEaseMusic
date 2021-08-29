#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>

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


class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    static MusicPlayer& getSingleton(){
        static MusicPlayer musicPlayer;
        return musicPlayer;
    }

    MusicPlayer(const MusicPlayer& musicPlayer)=delete ;

    MusicPlayer& operator=(const MusicPlayer& musicPlayer)=delete ;

    void openMusicFile(std::string path);       // todo: 中文字符问题

    /**
     * @brief 暂停后，调用后从暂停位置播放
     */
    void play();

    /**
     * @brief 播放指定路径下的音乐
     * @param path 音乐文件所在路径和文件名
     */
    void play(std::string path);

    /**
     * @brief 暂停播放，调用play()恢复播放
     */
    void stop();

    void preSong();

    void threadProducePacketBegin();

    int getVolume();

    /**
     * @brief 设置音量
     * @param v 范围 0~128
     */
    void setVolume(int v);

    void mute();

    void unMute();

    int64_t getSongLength();

    /**
     * @brief  进或快退
     * @param millisecond 正数为快进，负数为快退
     */
    void fastForwardOrBack(int64_t millisecond);

    /**
     * @brief 跳转到指定进度
     * @param millisecond 进度 大于0
     */
    void seek(int64_t millisecond);

    /**
     * @brief 扫描path下音乐文件
     * @param path 文件夹绝对路径
     */
    void scanDir(QString path = "D:/CloudMusic");

    /**
     * @brief 逐行读取音乐文件路径
     */
    QVector<QString> getMusicFilePath();


public:
    static AVFormatContext* _pFormatCtx;

    static AVCodecContext* _pCodecCtx;

    static AVPacketQueue _playQueue;

    static AVFrame* _pWantedFrame;

    static int _volume;

    static int _audioIndex;

    void initSDL();


private:
    MusicPlayer(QObject* parent = nullptr);

    void producePakcet();

    static int decode(uint8_t* audio_buf);

    static void fillAudio(void* udata,Uint8* stream,int len);

    void emitProgressChange(qint64 value);

    void initSongNameAndSinger(std::string path,QString& songName,QString& singer);


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

    static int64_t _progress;         // 播放进度(毫秒)

    enum PlayStatus
    {
        PLAYING,
        SEEK,   // 调整播放进度
        FINISH,
        PAUSE
    };

    std::mutex _mtxStatus;
    PlayStatus _status = FINISH;

    int _volumeBeforeMute;

    QString _curSongName;
    QString _curSinger;

signals:
    void signalProgressChanged(qint64);

    void signalSongLen(qint64);

    void signalCurSongName(QString);

    void signalCurSinger(QString);

};



#endif // MUSICPLAYER_H
