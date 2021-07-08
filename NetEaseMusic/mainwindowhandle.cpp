#include "mainwindowhandle.h"
#include "skin_config.h"
#include "mainwindow.h"

MainWindowHandle::MainWindowHandle()
{

}

MainWindowHandle& MainWindowHandle::getInstance()
{
    static MainWindowHandle mainWinHandIns;
    return mainWinHandIns;
}

void MainWindowHandle::setHandle(MainWindow* handle)
{
    _mainWinHand = handle;
}

MainWindow* MainWindowHandle::getHandle()
{
    return _mainWinHand;
}

bool MainWindowHandle::isSetHandle()
{
    return !(nullptr == _mainWinHand);
}

void MainWindowHandle::setSkin(const QString skinName)
{
    if(!isSetHandle()){
        return;
    }

    QString skinConfig = SkinConfig::getInstance().getConfig();

    QFile file(QString(":/res/css/black.css")); // todo:判读文件是否存在
    if(true == file.open(QFile::ReadOnly))
    {
        QTextStream textStream(&file);
        QString styleSheet = textStream.readAll();
        file.close();
        _mainWinHand->setStyleSheet(styleSheet);
        qDebug()<<"set skin"<<endl;
    }

}
