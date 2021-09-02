#ifndef PLAY_TABLE_WIDGET_H
#define PLAY_TABLE_WIDGET_H

#include <QWidget>

namespace Ui {
class play_table_widget;
}

class play_table_widget : public QWidget
{
    Q_OBJECT

public:
    explicit play_table_widget(QWidget *parent = nullptr);
    ~play_table_widget();

private:
    Ui::play_table_widget *ui;

    QVector<QString> _vSongPath;
};

#endif // PLAY_TABLE_WIDGET_H
