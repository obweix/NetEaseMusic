#ifndef MIDDLEWIDGET_H
#define MIDDLEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class PlaylistWidget;
class PlayWidget;
QT_END_NAMESPACE


class MiddleWidget:public QWidget
{
    Q_OBJECT
public:
    explicit MiddleWidget(QWidget *parent=nullptr);

    PlayWidget* getPlayWidget();
    PlaylistWidget* getPlaylistWidget();

private:
    void init();

private:
    PlaylistWidget* _playlistWidget;
    PlayWidget* _playWidget;

};



#endif // MIDDLEWIDGET_H
