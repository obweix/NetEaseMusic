#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE
class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = nullptr);

private:
    void init();
    void initConnect();

protected:
       void mousePressEvent(QMouseEvent *event);
       void mouseMoveEvent(QMouseEvent *event);
       void mouseReleaseEvent(QMouseEvent *event);

       /**
        * @brief 双击全屏显示
        */
       void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QPushButton* _btnLogo;

    QPushButton* _btnSkinBox;
    QPushButton* _btnSetting;

    QPushButton* _btnMin;
    QPushButton* _btnMax;
    QPushButton* _btnClose;

    // 移动窗口的变量;
    bool _isPressed;
    QPoint _startMovePos;

    // 区分当前窗口是否为最大
    bool _isMax;


signals:

};

#endif // TOPWIDGET_H
