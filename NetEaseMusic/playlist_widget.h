#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QWidget;
class QPushButton;
class QListWidget;
class QLabel;
QT_END_NAMESPACE
class PlaylistWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistWidget(QWidget *parent = nullptr);

private:
    void init();

private:
    QPushButton* _btnLocalMusic;
    QPushButton* _btnLikedMusic;

    QWidget* _lhsWidgetContainer;
    QWidget* _rhsWidgetContainer;

    QListWidget* _playList;



signals:


};

#endif // PLAYLISTWIDGET_H
