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


    QHBoxLayout* hl = new QHBoxLayout();
    _playWidget = new PlayWidget(this);
    _playlistWidget = new PlaylistWidget(this);

    //_playWidget->hide();
    _playlistWidget->hide();


    hl->addWidget(_playlistWidget);
    hl->addWidget(_playWidget);

    setLayout(hl);
}

PlayWidget* MiddleWidget::getPlayWidget()
{
    return _playWidget;
}

PlaylistWidget* MiddleWidget::getPlaylistWidget()
{
    return _playlistWidget;
}



