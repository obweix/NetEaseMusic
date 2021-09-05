#include "top_widget.h"
#include "mainwindowhandle.h"
#include <QtWidgets>

TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    _isMax = false;
    init();
}

void TopWidget::init()
{
    QWidget* topWidgetContainer = new QWidget(this);
    topWidgetContainer->setObjectName("topWidgetContainer");

    QHBoxLayout *hLayout = new QHBoxLayout(topWidgetContainer);
    hLayout->setObjectName("topWidgetHLayout");
    hLayout->setMargin(0);
    hLayout->setSpacing(10);

    _btnLogo = new QPushButton(topWidgetContainer);
    _btnSkinBox = new QPushButton(topWidgetContainer);
    _btnMin = new QPushButton(topWidgetContainer);
    _btnMax = new QPushButton(topWidgetContainer);
    _btnClose = new QPushButton(topWidgetContainer);
    _btnLogo->setObjectName("btnIcon"); 
    _btnSkinBox->setObjectName("btnSkinBox");
    _btnMin->setObjectName("btnMini");
    _btnMax->setObjectName("btnMax");   
    _btnClose->setObjectName("btnClose");

    hLayout->addWidget(_btnLogo);
    hLayout->addStretch(1);
    hLayout->addWidget(_btnSkinBox);
    hLayout->addWidget(_btnMin);

    hLayout->addWidget(_btnMax);
    hLayout->addWidget(_btnClose);
    hLayout->addSpacing(10);

    QHBoxLayout *hlContainer = new QHBoxLayout(this);
    hlContainer->setMargin(0);
    hlContainer->setSpacing(0);
    hlContainer->addWidget(topWidgetContainer);
    setLayout(hlContainer);

    initConnect();


}

void TopWidget::initConnect()
{
    connect(_btnClose,&QPushButton::clicked,[](){
        MainWindowHandle::getInstance().close();
    });

    connect(_btnMax,&QPushButton::clicked,[&](){
        MainWindowHandle::getInstance().max();
        _isMax = !_isMax;
    });

    connect(_btnMin,&QPushButton::clicked,[&](){
        MainWindowHandle::getInstance().min();
        _isMax = false;
    });

}


void TopWidget::mousePressEvent(QMouseEvent *event)
{
    if(_isMax)
    {
        return;
    }

    _startMovePos = event->globalPos();
    _isPressed = true;

    return QWidget::mousePressEvent(event);
}

void TopWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_isPressed)
    {
        QPoint movePoint = event->globalPos() - _startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        _startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());

    }
    return QWidget::mouseMoveEvent(event);
}

void TopWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _isPressed = false;

    return QWidget::mouseReleaseEvent(event);
}


void TopWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    MainWindowHandle::getInstance().max();
    _isMax = !_isMax;

    return QWidget::mouseDoubleClickEvent(event);
}







