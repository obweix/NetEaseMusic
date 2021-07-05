#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class TopWidget;
class MiddleWidget;
class BottomWidget;
class MainWindow;
class MainWindowHandle; //单例，获取主窗口句柄
QT_END_NAMESPACE

class MainWindowHandle
{
public:
    static MainWindowHandle& getInstance()
    {
        static MainWindowHandle mainWinHandIns;
        return mainWinHandIns;
    }

    void setHandle(MainWindow* handle){
        _mainWinHand = handle;
    }

    MainWindow* getHandle(){
        if(_mainWinHand){
            return  _mainWinHand;
        }
        return nullptr;
    }
private:
    MainWindow* _mainWinHand;

private:
    MainWindowHandle(){};
    MainWindowHandle(const MainWindowHandle& wndHand) = delete ;
    MainWindowHandle& operator=(const MainWindowHandle& wndHand) = delete;
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    TopWidget* _topWidget;
    MiddleWidget* _middleWidget;
    BottomWidget* _bottomWidget;
};
#endif // MAINWINDOW_H
