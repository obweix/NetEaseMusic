#include "playlist_widget.h"
#include <QtWidgets>

PlaylistWidget::PlaylistWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void PlaylistWidget::init()
{
    _lhsWidgetContainer = new QWidget(this);
    _rhsWidgetContainer = new QWidget(this);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    QVBoxLayout *vLayout = new QVBoxLayout(_lhsWidgetContainer);
    vLayout->setMargin(0);
    QStackedLayout *stackedLayout = new QStackedLayout(_rhsWidgetContainer);

    _btnLocalMusic = new QPushButton(tr("本地音乐"),_lhsWidgetContainer);
    _btnLikedMusic = new QPushButton(tr("我喜欢的音乐"),_lhsWidgetContainer);
    _btnLocalMusic->setMaximumWidth(200);
    _btnLocalMusic->setMinimumWidth(200);
    _btnLikedMusic->setMaximumWidth(200);
    _btnLikedMusic->setMinimumWidth(200);
    vLayout->addWidget(_btnLocalMusic);
    vLayout->addWidget(_btnLikedMusic);
    vLayout->addStretch(1);

    _playList = new QListWidget(_rhsWidgetContainer);
    _playList->addItem(tr("小さな恋のうた"));
    _playList->addItem("告白气球");
    stackedLayout->addWidget(_playList);

    hLayout->addWidget(_lhsWidgetContainer,0);
    hLayout->addWidget(_rhsWidgetContainer,1);
    setLayout(hLayout);
}
