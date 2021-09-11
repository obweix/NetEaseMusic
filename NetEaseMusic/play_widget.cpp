#include "play_widget.h"
#include "phonograph_widget.h"
#include "music_player.h"

#include <QLabel>
#include <QtWidgets>

//////////////////////////////////////////////////////
/// PlayWidget
//////////////////////////////////////////////////////
PlayWidget::PlayWidget(QWidget *parent) : QWidget(parent)
{
    // todo:读取歌曲封面
    _albumCover = QPixmap(":/res/image/AlbumCover1.jpg");

    _blurbackgroudImage = QPixmap(":/res/image/default_preview_background.png");
    _whiteMaskImage = QPixmap(":/res/image/album_background_white_mask.png");
    _blackMaskImage = QPixmap(":/res/image/album_background_black_mask.png");

    init();

    initConnect();
}

void PlayWidget::init()
{
    QHBoxLayout* hl = new QHBoxLayout();
    QVBoxLayout* vl = new QVBoxLayout();

    _phnographWidget = new PhonographWidget();
    _songNameTitle = new QLabel(tr("歌曲名:"));
    _songName = new QLabel();

    hl->addWidget(_phnographWidget);

    vl->addWidget(_songNameTitle);
    vl->addWidget(_songName);
    vl->addStretch(1);
    hl->addLayout(vl);
    setLayout(hl);
}

void PlayWidget::initConnect()
{
    connect(&MusicPlayer::getSingleton(),&MusicPlayer::signalCurSongName,[&](QString songName){
        _songName->setText(songName);
    });
}

void PlayWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QRect outerRect(0,0,this->width()-1,this->height()-1);
    painter.fillRect(outerRect, QBrush("#ffffff"));                   //绘制背景颜色

    painter.drawPixmap(this->rect(),_blurbackgroudImage);
    painter.drawPixmap(this->rect(),_blackMaskImage);
    QWidget::paintEvent(event);
}
