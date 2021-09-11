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

    void setAlbumCover(QPixmap cover);

    void play();

    void stop();

    bool isPlaying();

    void setBackgroundColor();


protected:
    virtual void paintEvent(QPaintEvent* event);

protected:

    QTimer* _timerDisk;
    QTimer* _timerArm;

    int _YoffsetOfDisk;
    int _maxAngleOfArm;
    int _currentDiskAngle;
    int _currentArmAngle;

    bool _isPlaying;

    QPixmap _albumCover;
    QPixmap _disk;
    QPixmap _toneArm;

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
