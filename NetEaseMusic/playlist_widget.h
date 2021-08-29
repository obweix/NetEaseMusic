#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QWidget;
class QPushButton;
class QListWidget;
class QLabel;
class QTableWidget;
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


    //QListWidget* _playList;
    QTableWidget* _playTable;

    QVector<QString> _vSongPath;



signals:


};

#endif // PLAYLISTWIDGET_H
