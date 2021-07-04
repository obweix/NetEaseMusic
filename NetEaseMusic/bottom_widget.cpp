#include "bottom_widget.h"
#include "play_widget.h"
#include "playlist_widget.h"
#include "middle_widget.h"
#include "mainwindow.h"

#include <QPushButton>
#include <QSlider>
#include <QtWidgets>

#include<QtDebug>


//////////////////////////////////////////////////////
/// PlayCtrlWidget
//////////////////////////////////////////////////////

PlayCtrlWidget::PlayCtrlWidget(QWidget *parent):QWidget(parent)
{
    init();
}

void PlayCtrlWidget::init()
{
    _playCtrlWidgetContainer = new QWidget(this);

    QVBoxLayout *vl = new QVBoxLayout(_playCtrlWidgetContainer);
    QHBoxLayout *hlCtrlBtn = new QHBoxLayout(_playCtrlWidgetContainer);
    QHBoxLayout *hlProgressBar = new QHBoxLayout(_playCtrlWidgetContainer);

    _btnPrevSong = new QPushButton("<",_playCtrlWidgetContainer);
    _btnPauseOrPlay = new QPushButton("||",_playCtrlWidgetContainer);
    _btnNextSong = new QPushButton(">",_playCtrlWidgetContainer);

    _btnPrevSong->setMaximumWidth(30);
    _btnPauseOrPlay->setMaximumWidth(30);
    _btnNextSong->setMaximumWidth(30);

    _sldProgressBar = new QSlider(Qt::Horizontal,_playCtrlWidgetContainer);
    _lbCurrentProgress = new QLabel("02:12",_playCtrlWidgetContainer);
    _lbSongLength = new QLabel("04:28",_playCtrlWidgetContainer);



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

    _btnMute = new QPushButton("mute");
    _sldVolume = new QSlider(Qt::Horizontal);

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

    vl->addWidget(_lbSongName);
    vl->addWidget(_lbSingerName);

    hl->addWidget(_btnAlbumCover);
    hl->addLayout(vl);

    setLayout(hl);

    connect(_btnAlbumCover,&QPushButton::clicked,[=](){
        if(_isShowPlaylist)
        {         
           showPlayWidget();
           _playlistWidget->hide();

           _isShowPlaylist = false;
        }
        else{           
            showPlaylistWidget();
            _playWidget->hide();

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
    _playlistWidget->show();

    QPoint gp;
    gp = _btnAlbumCover->mapTo(_mainWindowHandle,QPoint(0,0));
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
    QHBoxLayout* hl = new QHBoxLayout();

    _albumWidget = new AlbumWidget(this,_middleWidgetHandle,_mainWindowHandle);
    _playCtrlWidget = new PlayCtrlWidget();
    _volumeCtrlWidget = new VolumeCtrlWidget();


    hl->addWidget(_albumWidget);
    hl->addStretch(1);
    hl->addWidget(_playCtrlWidget,Qt::AlignHCenter);
    hl->addStretch(1);
    hl->addWidget(_volumeCtrlWidget);

    setLayout(hl);
}
