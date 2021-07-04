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
        ~ThreadCalcBackgroundImage(){};
        //virtual void run();

        void showPic(QPixmap pic);
    signals:
        //void ready(QPixmap pixmap);

private:
        QMutex m_mutex;
        QVector<QPixmap> vecPic;
};


//////////////////////////////////////////////////////
/// PlayWidget
//////////////////////////////////////////////////////
class PlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayWidget(QWidget *parent = nullptr);

private:
    void init();

private:
    PhonographWidget* _phnographWidget;

    QWidget*  _playWidgetContainer;

    QLabel* _songNameTitle;
    QLabel* _songName;


signals:

};

#endif // PLAYWIDGET_H
