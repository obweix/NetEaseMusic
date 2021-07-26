#include "middle_widget.h"
#include "play_widget.h"
#include "playlist_widget.h"

#include <QtWidgets>
#include<QtDebug>

MiddleWidget::MiddleWidget(QWidget* parent):QWidget(parent)
{
    init();
}

void MiddleWidget::init()
{


    QHBoxLayout* hl = new QHBoxLayout(this);

    hl->setObjectName("middleWidgetLayout");
    hl->setMargin(0);
    hl->setSpacing(0);
    _playlistWidget = new PlaylistWidget(this);
    _playlistWidget->setObjectName("playlistWidget");
    _playWidget = new PlayWidget(this);
    _playWidget->setObjectName("playWidget");


    //_playWidget->hide();

    hl->addWidget(_playlistWidget);
    hl->addWidget(_playWidget);
    _playWidget->show();
    _playWidget->raise();

    setLayout(hl);

    _playlistWidget->hide();

}

PlayWidget* MiddleWidget::getPlayWidget()
{
    return _playWidget;
}

PlaylistWidget* MiddleWidget::getPlaylistWidget()
{
    return _playlistWidget;
}



