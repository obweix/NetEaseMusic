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

//    QFile testfile(QString(":/res/txt/music_file_path.txt"));
//    if(!testfile.exists())
//        return;
//    if(!testfile.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;
//    QTextStream stream(&testfile);
//    stream.setAutoDetectUnicode(true);
//    stream.setCodec("UTF-8");
//    qDebug()<<stream.readAll();
//    testfile.close();

}

void MainWindowHandle::close()
{
    _mainWinHand->close();
}

void MainWindowHandle::max()
{
    if(_isMax){
        _mainWinHand->showNormal();
        _isMax = false;
    }else{
        _mainWinHand->showMaximized();
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

void MainWindowHandle::resize()
{
    _mainWinHand->resize(1022,670);
}
