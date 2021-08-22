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

    QFile file(QString(":/res/css/black.css"));
    if(true == file.open(QFile::ReadOnly))
    {
        QTextStream textStream(&file);
        QString styleSheet = textStream.readAll();
        //qDebug()<<styleSheet<<endl;
        file.close();
        _mainWinHand->setStyleSheet(styleSheet);
        qDebug()<<"set skin"<<endl;
    }

}

void MainWindowHandle::close()
{
    _mainWinHand->close();
}

void MainWindowHandle::max()
{
    if(_isMax){
        //_mainWinHand->resize(1022,670);
        _mainWinHand->showNormal();
        _isMax = false;
    }else{
        _mainWinHand->showMaximized();

//        QRect availableGeometry(QGuiApplication::screens().first()->availableGeometry());
//        _mainWinHand->setGeometry(availableGeometry);
        _isMax = true;
    }

}

void MainWindowHandle::min()
{
    if(!isSetHandle()){
        return;
    }

    _mainWinHand->showMinimized();

}
