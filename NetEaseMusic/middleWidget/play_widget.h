#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>

#include <QThread>
#include <QMutex>


QT_BEGIN_NAMESPACE
class QLabel;
class PhonographWidget;
QT_END_NAMESPACE


//////////////////////////////////////////////////////
/// PlayWidget
//////////////////////////////////////////////////////
class PlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayWidget(QWidget *parent = nullptr);

    void calcNewBackgroundImage(QPixmap pixmap);


private:
    void init();

    void initConnect();

private:
    PhonographWidget* _phnographWidget;

    QWidget*  _playWidgetContainer;

    QLabel* _songNameTitle;
    QLabel* _songName;

    QPixmap _albumCover;

    QPixmap _blurbackgroudImage;
    QPixmap _whiteMaskImage;
    QPixmap _blackMaskImage;

signals:


protected:
    virtual void paintEvent(QPaintEvent* event);

};

#endif // PLAYWIDGET_H
