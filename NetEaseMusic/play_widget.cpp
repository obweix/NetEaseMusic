#include "play_widget.h"
#include "phonograph_widget.h"
#include "image_filter.h"

#include <QLabel>
#include <QtWidgets>

ThreadCalcBackgroundImage::~ThreadCalcBackgroundImage()
{
    // 请求终止
    requestInterruption();
    quit();
    wait();
}


void ThreadCalcBackgroundImage::run()
{
    // 是否请求终止
    while (!isInterruptionRequested())
    {
        bool bPicFound = false;
        QPixmap pixmapToDeal;

        {
            QMutexLocker locker(&_mutex);
            if(!_vecPic.empty())
            {
                bPicFound = true;
                pixmapToDeal = _vecPic.back();
                _vecPic.clear();
            }
        }
        // locker超出范围并释放互斥锁

        if(bPicFound)
        {
            QPixmap newPixmap = ImageFilter::BlurImage(pixmapToDeal, 50, 80);

            bPicFound = false;
            {
                QMutexLocker locker(&_mutex);
                if(_vecPic.empty())      //在没有新图片需要计算时才发出图片,保证发出的总是最后一次计算
                    emit(ready(newPixmap));
            }
            // locker超出范围并释放互斥锁
        }
        else
            msleep(2000);
    }
}

void ThreadCalcBackgroundImage::showPic(QPixmap pic)
{
    QMutexLocker locker(&_mutex);
    _vecPic.push_back(pic);
}




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
    //_playWidgetContainer = new QWidget(this);

    QHBoxLayout* hl = new QHBoxLayout();
    QVBoxLayout* vl = new QVBoxLayout();

    _phnographWidget = new PhonographWidget();
    _songNameTitle = new QLabel(tr("歌曲名:"));
    _songName = new QLabel(tr("告白气球"));

    hl->addWidget(_phnographWidget);

    vl->addWidget(_songNameTitle);
    vl->addWidget(_songName);
    vl->addStretch(1);
    hl->addLayout(vl);


    setLayout(hl);
}

void PlayWidget::initConnect()
{
   //connect(calPicThread, &ThreadCalcBackgroundImage::ready, this, &PlayWidget::setNewBackgroundPixmap);
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
