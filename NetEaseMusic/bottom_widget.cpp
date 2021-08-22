#include "bottom_widget.h"
#include "play_widget.h"
#include "playlist_widget.h"
#include "middle_widget.h"
#include "mainwindowhandle.h"
#include "mainwindow.h"


#include <QPushButton>
#include <QSlider>
#include <QtWidgets>

#include<QtDebug>

AVFormatContext* MusicPlayer::_pFormatCtx = nullptr;
AVCodecContext* MusicPlayer::_pCodecCtx = nullptr;
AVPacketQueue MusicPlayer::_playQueue;
AVFrame* MusicPlayer::_pWantedFrame = nullptr;
int MusicPlayer::_volume = 128;
int MusicPlayer::_audioIndex = -1;

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

//    _btnPrevSong->setMaximumWidth(32);
//    _btnPauseOrPlay->setMaximumWidth(40);
//    _btnNextSong->setMaximumWidth(32);

    _sldProgressBar = new QSlider(Qt::Horizontal);
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
    _lbCurrentProgress = new QLabel("02:12");
    _lbSongLength = new QLabel("04:28");



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

    connect(_btnPauseOrPlay,&QPushButton::clicked,[](){
        MusicPlayer::getSingleton().init("D:/CloudMusic/还是要幸福.mp3");

        MusicPlayer::getSingleton().threadProducePacketBegin();

        MusicPlayer::getSingleton().play();
    });

}

//////////////////////////////////////////////////////
/// VolumeCtrlWidget
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
}

////////////////////////////////////////////////
/// class AlbumWidget;
////////////////////////////////////////////////
AlbumWidget::AlbumWidget(QWidget *parent,MiddleWidget *mdiileWidgetHandle,MainWindow* mainWindowHandle) : QWidget(parent),_middleWidgetHandle(mdiileWidgetHandle),_mainWindowHandle(mainWindowHandle)
{
    _playWidget = _middleWidgetHandle->getPlayWidget();
    _playlistWidget = _middleWidgetHandle->getPlaylistWidget();

    _isShowPlaylist = false;

    init();
}

void AlbumWidget::init()
{

    QVBoxLayout* vl = new QVBoxLayout();
    QHBoxLayout* hl = new QHBoxLayout();

    _lbSongName  = new QLabel("my love");
    _lbSingerName = new QLabel("田馥甄");
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
    _playWidget->show();
    //_playWidget->raise();

    QPoint gp;
    gp = _btnAlbumCover->mapTo(_mainWindowHandle,QPoint(0,0));
    QRect from(gp.x(),gp.y(),_btnAlbumCover->width(),_btnAlbumCover->height());

    QRect to = QRect(0,
                     0,
                     _middleWidgetHandle->width(),
                     _middleWidgetHandle->height());

    qDebug()<<"1from:"<<from.x()<<from.y()<<from.width()<<from.height();
    qDebug()<<"1to:"<<to.x()<<to.y()<<to.width()<<to.height();


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

    _playlistWidget->show();
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
