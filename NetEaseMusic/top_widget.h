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

private:
    QPushButton* _btnLogo;

    QPushButton* _btnSkinBox;
    QPushButton* _btnSetting;

    QPushButton* _btnMin;
    QPushButton* _btnMax;
    QPushButton* _btnClose;





signals:

};

#endif // TOPWIDGET_H
