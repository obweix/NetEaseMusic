#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>

#include <QThread>
#include <QMutex>


QT_BEGIN_NAMESPACE
class QLabel;
class PhonographWidget;
QT_END_NAMESPACE


//计算背景的线程
class ThreadCalcBackgroundImage:public QThread
{
    Q_OBJECT
    public:
        ThreadCalcBackgroundImage(QObject* parent):QThread(parent){}
        ~ThreadCalcBackgroundImage();
        virtual void run();

        void showPic(QPixmap pic);
    signals:
        void ready(QPixmap pixmap);

private:
        QMutex _mutex;
        QVector<QPixmap> _vecPic;
};


//////////////////////////////////////////////////////
/// PlayWidget
//////////////////////////////////////////////////////
class PlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayWidget(QWidget *parent = nullptr);

    void calcNewBackgroundImage(QPixmap pixmap);         //开线程计算并设置新的背景图片


private:
    void init();

    void initConnect();

private:
    PhonographWidget* _phnographWidget;

    QWidget*  _playWidgetContainer;

    QLabel* _songNameTitle;
    QLabel* _songName;

    ThreadCalcBackgroundImage* calPicThread;  //计算图片的线程

    QPixmap _albumCover;             //专辑封面

    QPixmap _blurbackgroudImage;
    QPixmap _whiteMaskImage;
    QPixmap _blackMaskImage;

signals:


protected:
//    virtual void mousePressEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* event);

};

#endif // PLAYWIDGET_H
