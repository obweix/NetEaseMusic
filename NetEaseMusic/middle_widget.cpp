#include "middle_widget.h"
#include "play_widget.h"
#include "playlist_widget.h"
#include "play_table_widget.h"

#include <QtWidgets>
#include<QtDebug>

MiddleWidget::MiddleWidget(QWidget* parent):QWidget(parent)
{
    init();
}

void MiddleWidget::init()
{

    _playTableWidget = new play_table_widget(this);

    QStackedLayout* sl = new QStackedLayout(this);

    //_playlistWidget = new PlaylistWidget(this);
    _playWidget = new PlayWidget(this);


    _playWidget->setObjectName("playWidget");

    sl->addWidget(_playTableWidget);
    sl->addWidget(_playWidget);
    sl->setCurrentIndex(0);

    setLayout(sl);



//    QHBoxLayout* hl = new QHBoxLayout(this);

//    hl->setObjectName("middleWidgetLayout");
//    hl->setMargin(0);
//    hl->setSpacing(0);
//    _playlistWidget = new PlaylistWidget(this);
//    //_playlistWidget->setStyleSheet("background-color:gray");
//    _playWidget = new PlayWidget(this);
//    _playWidget->setObjectName("playWidget");


//    //_playWidget->hide();

//    hl->addWidget(_playlistWidget);
//    hl->addWidget(_playWidget);
//    _playWidget->show();
//    _playWidget->raise();

//    setLayout(hl);

//    _playlistWidget->hide();
}

PlayWidget* MiddleWidget::getPlayWidget()
{
    return _playWidget;
}

PlaylistWidget* MiddleWidget::getPlaylistWidget()
{
    return _playlistWidget;
}



