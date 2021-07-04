#ifndef PHONOGRAPHWIDGET_H
#define PHONOGRAPHWIDGET_H

#include <QWidget>

//////////////////////////////////////////////////////
/// 留声机控件
//////////////////////////////////////////////////////
class PhonographWidget : public QWidget

{
    Q_OBJECT
public:
   explicit PhonographWidget(QWidget *parent = nullptr);

    void setAlbumCover(QPixmap cover);      //设置专辑封面

    void play();                            //播放

    void stop();                            //暂停

    bool isPlaying();                       //查询是否在播放

    void setBackgroundColor();              //设置背景颜色


protected:
    virtual void paintEvent(QPaintEvent* event);

protected:

    QTimer* _timerDisk;              //唱片转动定时器
    QTimer* _timerArm;               //唱臂转动定时器

    int _YoffsetOfDisk;              //唱片y方向上离上边缘的偏移量
    int _maxAngleOfArm;              //唱臂最大旋转的角度
    int _currentDiskAngle;           //唱片的角度在0~360之间,这里为了便于避免不必要的浮点计算误差（这里每一帧角度要小于1度），单位为0.1度
    int _currentArmAngle;

    bool _isPlaying;                   //是否正在播放

    QPixmap _albumCover;             //专辑封面
    QPixmap _disk;                   //唱片
    QPixmap _toneArm;                //唱臂

private slots:
    void updateAngleOfDisk();
    void updateAngleOfArm();

private:
    void initPaintingData(bool firstTime);

    int _albumWidth;
    int _albumHeight;
    int _toneArmWidth;
    int _toneArmHeight;

    int _albumCoverWidth;
    int _albumCoverHeight;

    QPixmap _picAlumCover;
    QPixmap _picDisk;
    QPixmap _picToneArm;


    QRect _outerRect;
    QBrush _brushBackground;         //背景画刷

};

#endif // PHONOGRAPHWIDGET_H
