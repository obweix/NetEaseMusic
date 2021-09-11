#include "middle_widget.h"
#include "play_widget.h"
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

    _sl = new QStackedLayout(this);
    _sl->setStackingMode(QStackedLayout::StackAll);

    _playWidget = new PlayWidget(this);


    _playWidget->setObjectName("playWidget");

    _sl->addWidget(_playTableWidget);
    _sl->addWidget(_playWidget);
    _sl->setCurrentIndex(1);

    setLayout(_sl);
}

PlayWidget* MiddleWidget::getPlayWidget()
{
    return _playWidget;
}


play_table_widget* MiddleWidget::getTableWidget()
{
    return _playTableWidget;
}

 void MiddleWidget::setCurrentIndex(int i)
 {
     _sl->setCurrentIndex(i);
 }
