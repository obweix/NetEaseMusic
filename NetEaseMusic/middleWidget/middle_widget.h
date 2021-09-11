#ifndef MIDDLEWIDGET_H
#define MIDDLEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
//class PlaylistWidget;
class PlayWidget;
class play_table_widget;
class QStackedLayout;
QT_END_NAMESPACE


class MiddleWidget:public QWidget
{
    Q_OBJECT
public:
    explicit MiddleWidget(QWidget *parent=nullptr);

    PlayWidget* getPlayWidget();
    play_table_widget* getTableWidget();

    void setCurrentIndex(int i);

private:
    void init();

private:
    PlayWidget* _playWidget;

    play_table_widget* _playTableWidget;

    QStackedLayout* _sl;

};



#endif // MIDDLEWIDGET_H
