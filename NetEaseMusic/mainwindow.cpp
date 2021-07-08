#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

#include "top_widget.h"
#include "middle_widget.h"
#include "bottom_widget.h"

#include "mainwindowhandle.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(1022,670);
    setWindowTitle(tr("QQ音乐"));

    QVBoxLayout* vLayout = new QVBoxLayout();

    _topWidget = new TopWidget();
    vLayout->addWidget(_topWidget,1);

    _middleWidget = new MiddleWidget();
    vLayout->addWidget(_middleWidget,30);

    _bottomWidget = new BottomWidget(nullptr,_middleWidget,this);
    vLayout->addWidget(_bottomWidget,2);

    //this->setStyleSheet("background:gray;");

    setLayout(vLayout);

    MainWindowHandle::getInstance().setHandle(this);
    MainWindowHandle::getInstance().setSkin();

}


MainWindow::~MainWindow()
{
    delete ui;
}

