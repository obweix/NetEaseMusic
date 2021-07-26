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
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(1022,670);
    setWindowTitle(tr("QQ音乐"));


    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->setSpacing(0);

    _topWidget = new TopWidget();
    _topWidget->setMinimumHeight(58);
    //_topWidget->setProperty("margin",0);
    vLayout->addWidget(_topWidget);

    _middleWidget = new MiddleWidget();
    //_middleWidget->setStyleSheet("background:gray;");
    vLayout->addWidget(_middleWidget,1);



    _bottomWidget = new BottomWidget(nullptr,_middleWidget,this);
    _bottomWidget->setMinimumHeight(70);
    vLayout->addWidget(_bottomWidget);


    //this->setStyleSheet("background:gray;");



    MainWindowHandle::getInstance().setHandle(this);
    MainWindowHandle::getInstance().setSkin();

    setLayout(vLayout);

}


MainWindow::~MainWindow()
{
    delete ui;
}

