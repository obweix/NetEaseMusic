#include "phonograph_widget.h"
#include "music_player.h"
#include <QPainter>
#include<QTimer>
#include <QDebug>

//////////////////////////////////////////////////////
/// 留声机控件
//////////////////////////////////////////////////////
PhonographWidget::PhonographWidget(QWidget *parent):QWidget(parent),
    _YoffsetOfDisk(76),
    _maxAngleOfArm(36),
    _currentDiskAngle(0),
    _currentArmAngle(0),
    _isPlaying(false),
    _albumCover(":/res/image/AlbumCover1.jpg"),
    _disk(":/res/image/netease_disk.png"),
    _toneArm(":/res/image/netease_tone_arm.png")
{
    setMouseTracking(true);
    _timerDisk = new QTimer(this);
    _timerArm = new QTimer(this);

    _timerDisk->setInterval(33);
    _timerArm->setInterval(25);

    connect(_timerDisk, &QTimer::timeout, this, &PhonographWidget::updateAngleOfDisk);
    connect(_timerArm, &QTimer::timeout, this, &PhonographWidget::updateAngleOfArm);

    initPaintingData(true);

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalIsPause,[&](){
        qDebug()<<"stop.,,,,,,"<<endl;
         stop();
    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalIsPlaying,[&](){
        qDebug()<<"play.,,,,,,,,"<<endl;
       play();
    });
}

void PhonographWidget::initPaintingData(bool firstTime)
{
   if(firstTime)
   {
        _albumWidth = 400;
        _albumHeight = 400;
        _toneArmWidth = 200;
        _toneArmHeight = 100;

        _picDisk = _disk.scaled(_albumWidth, _albumHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        _picToneArm = _toneArm.scaled(_toneArmWidth, _toneArmHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        _albumCoverWidth = _albumWidth * 0.66;
        _albumCoverHeight = _albumHeight * 0.66;
   }

   _picAlumCover = _albumCover.scaled(_albumCoverWidth, _albumCoverHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void PhonographWidget::setAlbumCover(QPixmap cover)
{
    _albumCover = cover;
    initPaintingData(false);
}

void PhonographWidget::play()
{
    _isPlaying = true;

    if(!_timerDisk->isActive())
        _timerDisk->start();

    if(!_timerArm->isActive())
        _timerArm->start();
}

void PhonographWidget::stop()
{
    _isPlaying = false;

    if(!_timerArm->isActive())
        _timerArm->start();
}


bool PhonographWidget::isPlaying()
{
    return _isPlaying;
}

void PhonographWidget::updateAngleOfDisk()
{
    if(_isPlaying)
    {
        _currentDiskAngle += 3;

        _currentDiskAngle = _currentDiskAngle % 3600;

        update();
    }
    else
        _timerDisk->stop();      //已经被要求停止，停止定时器
}

void PhonographWidget::updateAngleOfArm()
{
    if(_isPlaying && _currentArmAngle != _maxAngleOfArm)
    {
        _currentArmAngle+=4;

        if(_currentArmAngle > _maxAngleOfArm)
            _currentArmAngle = _maxAngleOfArm;

        update();
    }

    if(!_isPlaying && _currentArmAngle != 0)
    {
        _currentArmAngle-=4;

        if(_currentArmAngle < 0)
            _currentArmAngle = 0;
        update();
    }

    if((_isPlaying && _currentArmAngle == _maxAngleOfArm) ||
            (!_isPlaying && _currentArmAngle == 0))
    {
        if(_timerArm->isActive())
            _timerArm->stop();
    }

}

void PhonographWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    QRect outerRect(0,0,this->width()-1,this->height()-1);
    painter.fillRect(outerRect, QBrush("#00000000"));
    QWidget::paintEvent(event);


    QPoint albumBase(this->width()/2-_albumWidth/2, _YoffsetOfDisk);
    QPoint albumCenter(albumBase.x() + _albumWidth/2, albumBase.y() + _albumHeight/2);

    QPoint rotatePointOfArm(20,20);
    QPoint ArmRotatePoint(this->width()/2, 0);

    painter.save();

    //绘制唱片
    painter.translate(albumCenter);
    painter.rotate(_currentDiskAngle * 1.0 / 10);

    painter.drawPixmap(- _albumCoverWidth/2, - _albumCoverHeight/2,_albumCoverWidth, _albumCoverHeight,_picAlumCover);
    painter.drawPixmap(- _albumWidth/2, - _albumHeight/2,_albumWidth, _albumHeight, _picDisk);

    painter.restore();

    painter.save();

    painter.translate(ArmRotatePoint);
    painter.rotate(_currentArmAngle);

    painter.drawPixmap( - rotatePointOfArm.x(), - rotatePointOfArm.y(), _toneArmWidth, _toneArmHeight, _picToneArm);

    painter.restore();
}
