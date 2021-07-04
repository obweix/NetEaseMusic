#include "play_widget.h"

#include "phonograph_widget.h"
#include <QLabel>
#include <QtWidgets>

//////////////////////////////////////////////////////
/// PlayWidget
//////////////////////////////////////////////////////
PlayWidget::PlayWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void PlayWidget::init()
{
    _playWidgetContainer = new QWidget(this);

    QHBoxLayout* hl = new QHBoxLayout(_playWidgetContainer);
    QVBoxLayout* vl = new QVBoxLayout(_playWidgetContainer);

    _phnographWidget = new PhonographWidget(_playWidgetContainer); 
    _songNameTitle = new QLabel(tr("歌曲名"));
    _songName = new QLabel(tr("告白气球"));

    hl->addWidget(_phnographWidget);

    vl->addWidget(_songNameTitle);
    vl->addWidget(_songName);
    hl->addLayout(vl);

    setLayout(hl);
}
