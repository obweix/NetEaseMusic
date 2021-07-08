#ifndef MAINWINDOWHANDLE_H
#define MAINWINDOWHANDLE_H

#include<QtWidgets>

class MainWindow;
class MainWindowHandle
{
public:
    static MainWindowHandle& getInstance();

    void setHandle(MainWindow* handle);

    MainWindow* getHandle();

    void setSkin(const QString skinName="");

private:
    MainWindow* _mainWinHand;

private:
    MainWindowHandle();
    MainWindowHandle(const MainWindowHandle& wndHand) = delete ;
    MainWindowHandle& operator=(const MainWindowHandle& wndHand) = delete;

    bool isSetHandle();
};

#endif // MAINWINDOWHANDLE_H
