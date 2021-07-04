#include "top_widget.h"
#include <QtWidgets>
TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void TopWidget::init()
{
    QHBoxLayout *hLayout = new QHBoxLayout();

    _btnLogo = new QPushButton("logo");
    _btnSkinBox = new QPushButton("skin");
    _btnSetting = new QPushButton("setting");
    _btnMin = new QPushButton("min");
    _btnMax = new QPushButton("max");
    _btnClose = new QPushButton("close");

    hLayout->addWidget(_btnLogo);

    hLayout->addStretch(1);

    hLayout->addWidget(_btnSkinBox);
    hLayout->addWidget(_btnMin);
    hLayout->addWidget(_btnMax);
    hLayout->addWidget(_btnClose);

    hLayout->setMargin(0);

    setLayout(hLayout);
}
