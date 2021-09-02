#ifndef MIDDLEWIDGET_H
#define MIDDLEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class PlaylistWidget;
class PlayWidget;
class play_table_widget;
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

    play_table_widget* _playTableWidget;

};



#endif // MIDDLEWIDGET_H
