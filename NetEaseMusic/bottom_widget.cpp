#include "bottom_widget.h"
#include "play_widget.h"
#include "middle_widget.h"
#include "mainwindowhandle.h"
#include "mainwindow.h"
#include "play_table_widget.h"


#include <QPushButton>
#include <QSlider>
#include <QtWidgets>

#include<QtDebug>

#define DEFAULT_VOLUME 32

AVFormatContext* MusicPlayer::_pFormatCtx = nullptr;
AVCodecContext* MusicPlayer::_pCodecCtx = nullptr;
AVPacketQueue MusicPlayer::_playQueue;
AVFrame* MusicPlayer::_pWantedFrame = nullptr;
int MusicPlayer::_volume = DEFAULT_VOLUME;
int MusicPlayer::_audioIndex = -1;
int64_t MusicPlayer::_progress = 0;

//////////////////////////////////////////////////////
/// PlayCtrlWidget
//////////////////////////////////////////////////////

PlayCtrlWidget::PlayCtrlWidget(QWidget *parent):QWidget(parent)
{
    init();
}

void PlayCtrlWidget::init()
{
    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hlCtrlBtn = new QHBoxLayout();
    QHBoxLayout *hlProgressBar = new QHBoxLayout();

    _btnPrevSong = new QPushButton();
    _btnPauseOrPlay = new QPushButton();
    _btnNextSong = new QPushButton();
    _btnPrevSong->setObjectName("btnPreSong");
    _btnPauseOrPlay->setObjectName("btnPlayOrPause");
    _btnNextSong->setObjectName("btnNextSong");

    _sldProgressBar = new QSlider(Qt::Horizontal);
    _sldProgressBar->setRange(0,1000);
    _sldProgressBar->setStyleSheet(
                "QSlider{"
                    "background: #cccccc;"
                    "height: 6px;"
                     "border-radius: 2px;"
                "} "

                "QSlider::sub-page:horizontal {"
                    "height: 6px; "
                    "border-radius: 2px;"
                    "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ff3344, stop:1 #CC0033); "
                "}"

                "QSlider::handle:horizontal {"
                    "height: 10px;"
                    "width: 14px; "
                    "margin-top: -5px;"
                    "margin-bottom: -5px; "
                    "border-radius: 7px;"
                    "border: 1px solid #cccccc;"
                    "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.25 #CC0033, stop:0.4 #fefefe);"
                "}"
                "QSlider::handle:horizontal:hover {"
                    "border: 1px solid #aaaaaa;"
                    "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.3 #CC0033, stop:0.45 #fefefe);"
                "}"
                );
    _lbCurrentProgress = new QLabel("00:00");
    _lbSongLength = new QLabel("00:00");


    hlCtrlBtn->addStretch(1);
    hlCtrlBtn->addWidget(_btnPrevSong);
    hlCtrlBtn->addWidget(_btnPauseOrPlay);
    hlCtrlBtn->addWidget(_btnNextSong);
    hlCtrlBtn->addStretch(1);

    vl->addLayout(hlCtrlBtn);
    vl->addStretch(1);

    hlProgressBar->addWidget(_lbCurrentProgress);
    hlProgressBar->addWidget(_sldProgressBar);
    hlProgressBar->addWidget(_lbSongLength);

    vl->addLayout(hlProgressBar);

    setLayout(vl);

    connect(_btnPauseOrPlay,&QPushButton::clicked,[&](){
        if(_musicPlayer.isPlaying()){
            _musicPlayer.stop();

            return;
        }

        if(_musicPlayer.isPause()){
            _musicPlayer.play();

        }

        int64_t index = _musicPlayer.getPlayingSongIndex();
        _musicPlayer.play(index);
    });

    connect(_btnNextSong, &QPushButton::clicked,[&](){
        _musicPlayer.nextSong();
    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalNextSong,[&](){
        _musicPlayer.nextSong();
    });


    connect(_btnPrevSong,&QPushButton::clicked,[&](){
        _musicPlayer.preSong();
    });

    connect(_sldProgressBar,&QSlider::sliderPressed,this,&PlayCtrlWidget::onSliderPressed);
    connect(_sldProgressBar,&QSlider::sliderReleased,this,&PlayCtrlWidget::onSliderReleased);
    connect(_sldProgressBar,&QSlider::sliderMoved,this,&PlayCtrlWidget::onSliderMove);

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalProgressChanged,this,&PlayCtrlWidget::slotPositionChanged);

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalSongLen,[=](qint64 len){
        setTime2Label(_lbSongLength,len);
    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalIsPause,[&](){
         _btnPauseOrPlay->setStyleSheet("background-image:url(\":/res/image/btn_play.png\");");
    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalIsPlaying,[&](){
        _btnPauseOrPlay->setStyleSheet("background-image:url(\":/res/image/btn_pause.png\");");
    });

}

void PlayCtrlWidget::slotPositionChanged(qint64 progress)
{
    //qDebug()<<"progress:"<<progress<<endl;
    setTime2Label(_lbCurrentProgress,progress);

    if(!_sliderPressed){
         _sldProgressBar->setValue(1.0 * progress / MusicPlayer::getSingleton().getSongLength() * 1000);
    }

}

 void PlayCtrlWidget::setTime2Label(QLabel* label,qint64 time)
 {
     time = time / 1000;    // transform ms to s.

     qint64 sec = time % 60;
     qint64 min = time /60;

     QString s;
     s.sprintf("%.2lld:%.2lld",min,sec);
     label->setText(s);
 }

  void PlayCtrlWidget::onSliderPressed()
  {
      _sliderPressed = true;
  }

 void PlayCtrlWidget::onSliderReleased()
 {
     qint64 songLen = MusicPlayer::getSingleton().getSongLength();

     MusicPlayer::getSingleton().seek(_posSeek2 * songLen / 1000);

     _sliderPressed = false;
 }

 void PlayCtrlWidget::onSliderMove(int value)
 {
     _posSeek2 = value;
 }





//////////////////////////////////////////////////////
/// class VolumeCtrlWidget
//////////////////////////////////////////////////////

VolumeCtrlWidget::VolumeCtrlWidget(QWidget *parent):QWidget(parent)
{
    init();
}

void VolumeCtrlWidget::init()
{
    QHBoxLayout* hl = new QHBoxLayout();

    _btnMute = new QPushButton();
    _btnMute->setStyleSheet("QPushButton{"
                                "width:22px;"
                                "height:22px;"
                                "background-color:rgba(0, 0, 0, 0);"
                                "background-image:url(:/res/image/sound_normal_black.png);"
                                "background-repeat:no-repeat;"
                            "}"
                            "QPushButton:hover{"
                                    "width:22px;"
                                    "height:22px;"
                                    "background-color:rgba(0, 0, 0, 0);"
                                    "background-image:url(:/res/image/sound_hover_black.png);"
                                    "background-repeat:no-repeat;"
                            "}"
                            "");
    _sldVolume = new QSlider(Qt::Horizontal);
    _sldVolume->setRange(0,128);
    _sldVolume->setValue(DEFAULT_VOLUME);
    _sldVolume->setStyleSheet(
                "QSlider{"
                    "background: #cccccc;"
                    "height: 4px;"
                     "border-radius: 1px;"
                "} "

                "QSlider::sub-page:horizontal {"
                    "height: 4px; "
                    "border-radius: 1px;"
                    "background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ff3344, stop:1 #CC0033); "
                "}"

                "QSlider::handle:horizontal {"
                    "height: 10px;"
                    "width: 14px; "
                    "margin-top: -5px;"
                    "margin-bottom: -5px; "
                    "border-radius: 7px;"
                    "border: 1px solid #cccccc;"
                    "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.25 #CC0033, stop:0.4 #fefefe);"
                "}"
                "QSlider::handle:horizontal:hover {"
                    "border: 1px solid #aaaaaa;"
                    "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.3 #CC0033, stop:0.45 #fefefe);"
                "}"
                );

    hl->addWidget(_btnMute);
    hl->addWidget(_sldVolume);

    setLayout(hl);

    connect(_sldVolume,&QSlider::sliderMoved,this,&VolumeCtrlWidget::onSliderMove);
    connect(_sldVolume,&QSlider::valueChanged,this,&VolumeCtrlWidget::onSliderMove);
}

void VolumeCtrlWidget::onSliderMove(int value)
{
    MusicPlayer::getSingleton().setVolume(value);
}

////////////////////////////////////////////////
/// class AlbumWidget;
////////////////////////////////////////////////
AlbumWidget::AlbumWidget(QWidget *parent,MiddleWidget *mdiileWidgetHandle,MainWindow* mainWindowHandle) : QWidget(parent),_middleWidgetHandle(mdiileWidgetHandle),_mainWindowHandle(mainWindowHandle)
{
    _playWidget = _middleWidgetHandle->getPlayWidget();
    _playTableWidget = _middleWidgetHandle->getTableWidget();

    _isShowPlaylist = false;

    init();
}

void AlbumWidget::init()
{

    QVBoxLayout* vl = new QVBoxLayout();
    QHBoxLayout* hl = new QHBoxLayout();

    _lbSongName  = new QLabel();
    _lbSingerName = new QLabel();
    _btnAlbumCover = new QPushButton("cover");
    _btnAlbumCover->setObjectName("btnAlbumCover");

    vl->addWidget(_lbSongName);
    vl->addWidget(_lbSingerName);

    hl->addWidget(_btnAlbumCover);
    hl->addLayout(vl);

    setLayout(hl);

    initConnect();
}

void AlbumWidget::initConnect()
{
    connect(_btnAlbumCover,&QPushButton::clicked,[=](){
        if(_isShowPlaylist)
        {
           showPlayWidget();
           _isShowPlaylist = false;
        }
        else{
            showPlaylistWidget();
            _isShowPlaylist = true;
        }

    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalCurSinger,[&](QString singer){
        _lbSingerName->setText(singer);
    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalCurSongName,[&](QString songName){
        _lbSongName->setText(songName);
    });


    connect(&_animationShowPlayWidget,&QPropertyAnimation::finished,[&](){
        _middleWidgetHandle->setCurrentIndex(1);
    });

    connect(&_animationShowPlaylistWidget,&QPropertyAnimation::finished,[&](){
        _middleWidgetHandle->setCurrentIndex(0);
        _playTableWidget->raise();
    });
}

void AlbumWidget::moveWidget(QPropertyAnimation& animation,
                QWidget* widget,
                QRect from,
                QRect to)
{
    animation.setPropertyName("geometry");
    animation.setDuration(500);
    animation.setTargetObject(widget);
    animation.setStartValue(from);
    animation.setEndValue(to);
    animation.start();
}

void AlbumWidget::showPlayWidget()
{
    QPoint gp;
    gp = _btnAlbumCover->mapTo(_mainWindowHandle,QPoint(0,0));
    QRect from(gp.x(),gp.y(),_btnAlbumCover->width(),_btnAlbumCover->height());

    QRect to = QRect(0,
                     0,
                     _middleWidgetHandle->width(),
                     _middleWidgetHandle->height());

    qDebug()<<"1from:"<<from.x()<<from.y()<<from.width()<<from.height();
    qDebug()<<"1to:"<<to.x()<<to.y()<<to.width()<<to.height();


    _playWidget->raise();
    moveWidget(_animationShowPlayWidget,_playWidget,from,to);


}

void AlbumWidget::showPlaylistWidget()
{
    QPoint gp;
    gp = _btnAlbumCover->mapTo(MainWindowHandle::getInstance().getHandle(),QPoint(0,0));
    QRect to(gp.x(),gp.y(),_btnAlbumCover->width(),_btnAlbumCover->height());

    QRect from = QRect(0,
                     0,
                     _middleWidgetHandle->width(),
                     _middleWidgetHandle->height());

    qDebug()<<"2from:"<<from.x()<<from.y()<<from.width()<<from.height();
    qDebug()<<"2to:"<<to.x()<<to.y()<<to.width()<<to.height();


    moveWidget(_animationShowPlaylistWidget,_playWidget,from,to);



}



//////////////////////////////////////////////////////
/// BottomWidget
//////////////////////////////////////////////////////

BottomWidget::BottomWidget(QWidget *parent,MiddleWidget *middleWidgetHandle,MainWindow* mainWindowHandle) : QWidget(parent),_middleWidgetHandle(middleWidgetHandle),_mainWindowHandle(mainWindowHandle)
{
    init();
}

void BottomWidget::init()
{
    QWidget* bottomWidgetContainer = new QWidget(this);
    QHBoxLayout* hlContainer = new QHBoxLayout(this);
    hlContainer->setObjectName("bottomWidgetHlCon");
    bottomWidgetContainer->setObjectName("bottomWidgetContainer");
    bottomWidgetContainer->setStyleSheet("background-color:#212124;");
    hlContainer->setMargin(0);
    hlContainer->setSpacing(0);


    QHBoxLayout* hl = new QHBoxLayout(bottomWidgetContainer);
    hl->setObjectName("bottomWidgetHl");

    _albumWidget = new AlbumWidget(bottomWidgetContainer,_middleWidgetHandle,_mainWindowHandle);
    _playCtrlWidget = new PlayCtrlWidget(bottomWidgetContainer);
    _volumeCtrlWidget = new VolumeCtrlWidget(bottomWidgetContainer);
    _albumWidget->setObjectName("albumWidget");
    _playCtrlWidget->setObjectName("playCtrlWidget");
    _volumeCtrlWidget->setObjectName("volumeCtrlWidget");


    hl->addWidget(_albumWidget);
    hl->addStretch(1);
    hl->addWidget(_playCtrlWidget,Qt::AlignHCenter);
    hl->addStretch(1);
    hl->addWidget(_volumeCtrlWidget);

    hlContainer->addWidget(bottomWidgetContainer);
    setLayout(hlContainer);
    //setLayout(hl);
}
