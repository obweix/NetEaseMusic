#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QtWidgets>

#include "top_widget.h"
#include "middle_widget.h"
#include "bottom_widget.h"

#include "mainwindowhandle.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(1022,670);
    setWindowTitle(tr("NetEaseMusic"));


    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->setSpacing(0);

    _topWidget = new TopWidget();
    _topWidget->setMinimumHeight(58);
    vLayout->addWidget(_topWidget);

    _middleWidget = new MiddleWidget();
    vLayout->addWidget(_middleWidget,1);

    _bottomWidget = new BottomWidget(nullptr,_middleWidget,this);
    _bottomWidget->setMinimumHeight(70);
    vLayout->addWidget(_bottomWidget);

    MainWindowHandle::getInstance().setHandle(this);
    MainWindowHandle::getInstance().setSkin();

    setLayout(vLayout);
}


MainWindow::~MainWindow()
{
    delete ui;
}

