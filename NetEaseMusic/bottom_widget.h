#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>

#include "music_player.h"

QT_BEGIN_NAMESPACE
class PlayCtrlWidget;
class VolumeCtrlWidget;
class QPushButton;
class PlayWidget;
class PlaylistWidget;
class QSlider;
class QLabel;
class MiddleWidget;
class QPropertyAnimation;
class MainWindow;
class MusicPlayer;
QT_END_NAMESPACE

/**
 * @brief The PlayCtrlWidget class
 */
class PlayCtrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayCtrlWidget(QWidget *parent = nullptr);

private:
    void init();

    void setTime2Label(QLabel* label,qint64 time);

    void onSliderPressed();

    void onSliderReleased();

    void onSliderMove(int value);

private:
    QPushButton* _btnPrevSong;
    QPushButton* _btnPauseOrPlay;
    QPushButton* _btnNextSong;

    QSlider* _sldProgressBar;
    QLabel* _lbCurrentProgress;
    QLabel* _lbSongLength;


    QWidget* _playCtrlWidgetContainer;

    int _posSeek2;

    bool _sliderPressed;

public slots:

    // 播放进度改变
    void slotPositionChanged(qint64 progress);

    //void slotSongLen(qint64 len);

signals:


};

/**
 * @brief The VolumeCtrlWidget class
 */
class VolumeCtrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VolumeCtrlWidget(QWidget *parent = nullptr);

private:
    void init();

    void onSliderMove(int value);

private:
    QPushButton* _btnMute;

    QSlider* _sldVolume;


signals:



};

/**
 * @brief The AlbumWidget class
 */
class AlbumWidget:public QWidget
{
    Q_OBJECT
public:
    explicit AlbumWidget(QWidget *parent = nullptr,MiddleWidget *mdiileWidgetHandle = nullptr,MainWindow* mainWindowHandle=nullptr);

private:
    void init();

    void initConnect();

    void moveWidget(QPropertyAnimation& animation,
                    QWidget* widget,
                    QRect from,
                    QRect to);

    void showPlayWidget();
    void showPlaylistWidget();


private:
    QPushButton* _btnAlbumCover;
    QLabel* _lbSongName;
    QLabel* _lbSingerName;

    MiddleWidget* _middleWidgetHandle;
    MainWindow* _mainWindowHandle;

    PlayWidget* _playWidget;
    PlaylistWidget* _playlistWidget;




    bool _isShowPlaylist;

    QPropertyAnimation _animationShowPlayWidget;
    QPropertyAnimation _animationShowPlaylistWidget;


};



/**
 * @brief The BottomWidget class
 */
class BottomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BottomWidget(QWidget *parent = nullptr,MiddleWidget *mdiileWidgetHandle = nullptr,MainWindow* mainWindowHandle = nullptr);

private:
    void init();


private:
    AlbumWidget* _albumWidget;
    PlayCtrlWidget* _playCtrlWidget;
    VolumeCtrlWidget* _volumeCtrlWidget;

    PlayWidget* _playWidget;

    MainWindow* _mainWindowHandle;

    MiddleWidget* _middleWidgetHandle;

signals:

};

#endif // BOTTOMWIDGET_H
