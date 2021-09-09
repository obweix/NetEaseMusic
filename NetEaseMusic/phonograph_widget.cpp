#include "phonograph_widget.h"
#include "music_player.h"
#include <QPainter>
#include<QTimer>

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
    setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    _timerDisk = new QTimer(this);              //唱片转动定时器
    _timerArm = new QTimer(this);               //唱臂转动定时器

    _timerDisk->setInterval(33);                 //人眼能分辨的每秒帧率一般在24-30帧
    _timerArm->setInterval(25);

    connect(_timerDisk, &QTimer::timeout, this, &PhonographWidget::updateAngleOfDisk);
    connect(_timerArm, &QTimer::timeout, this, &PhonographWidget::updateAngleOfArm);

    initPaintingData(true);

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalIsPause,[&](){
         stop();
    });

    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalIsPlaying,[&](){
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

    if(!_timerDisk->isActive())    //未被启动时才启动
        _timerDisk->start();

    if(!_timerArm->isActive())     //未被启动时才启动
        _timerArm->start();
}

void PhonographWidget::stop()
{
    _isPlaying = false;

    if(!_timerArm->isActive())   //如果唱臂定时器已经关闭，启动定时器
        _timerArm->start();
}

//查询是否在播放
bool PhonographWidget::isPlaying()
{
    return _isPlaying;
}

void PhonographWidget::updateAngleOfDisk()
{
    if(_isPlaying)
    {
        _currentDiskAngle += 3; //每帧转动 0.3 度

        _currentDiskAngle = _currentDiskAngle % 3600;  // 0.1 * 3600 = 360

        update();
    }
    else
        _timerDisk->stop();      //已经被要求停止，停止定时器
}

void PhonographWidget::updateAngleOfArm()
{
    if(_isPlaying && _currentArmAngle != _maxAngleOfArm) //播放中，但是磁头没到位，则转动一下
    {
        _currentArmAngle+=4;  // 每帧转动4度

        if(_currentArmAngle > _maxAngleOfArm)
            _currentArmAngle = _maxAngleOfArm;

        update();
    }

    if(!_isPlaying && _currentArmAngle != 0)  //已经没在播放，却没有复位，则转动一下
    {
        _currentArmAngle-=4;

        if(_currentArmAngle < 0)
            _currentArmAngle = 0;
        update();
    }

    if((_isPlaying && _currentArmAngle == _maxAngleOfArm) ||
            (!_isPlaying && _currentArmAngle == 0))
    {
        _timerArm->stop(); //唱臂到位，停止定时器
    }

}

void PhonographWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    QRect outerRect(0,0,this->width()-1,this->height()-1);
    painter.fillRect(outerRect, QBrush("#00000000"));                   //绘制背景颜色
    QWidget::paintEvent(event);

    //绘制相关参数计算
    QPoint albumBase(this->width()/2-_albumWidth/2, _YoffsetOfDisk);                      //唱片左上角位置
    QPoint albumCenter(albumBase.x() + _albumWidth/2, albumBase.y() + _albumHeight/2);    //唱片中心点位置

    QPoint rotatePointOfArm(20,20);                 //唱臂图片上旋转的点
    QPoint ArmRotatePoint(this->width()/2, 0);      //唱臂在整个控件中旋转的点

    painter.save();         //状态入栈

    //绘制唱片
    painter.translate(albumCenter);     //设置中心点到唱片要在整个控件上放置的中心点位置
    painter.rotate(_currentDiskAngle * 1.0 / 10);

    painter.drawPixmap(- _albumCoverWidth/2, - _albumCoverHeight/2,_albumCoverWidth, _albumCoverHeight,_picAlumCover);
    painter.drawPixmap(- _albumWidth/2, - _albumHeight/2,_albumWidth, _albumHeight, _picDisk);

    painter.restore();      //恢复之前状态

    painter.save();         //状态入栈
    //绘制唱臂
    painter.translate(ArmRotatePoint);     //设置在整个控件上旋转点
    painter.rotate(_currentArmAngle);

    painter.drawPixmap( - rotatePointOfArm.x(), - rotatePointOfArm.y(), _toneArmWidth, _toneArmHeight, _picToneArm);

    painter.restore();      //恢复之前状态
}
