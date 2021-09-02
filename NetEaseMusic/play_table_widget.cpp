#include "play_table_widget.h"
#include "ui_play_table_widget.h"
#include "music_player.h"

play_table_widget::play_table_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::play_table_widget)
{
    ui->setupUi(this);

    //this->setStyleSheet("background:#434343;");

    _vSongPath = MusicPlayer::getSingleton().getMusicFilePath();

    ui->btn_scan_music->setStyleSheet("text-align:left");
    ui->btn_local_music->setStyleSheet("text-align:left");


    //ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(_vSongPath.size());

    //设置列数和列宽
    int width = 1920;
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnWidth(0, width * 0.20);
    ui->tableWidget->setColumnWidth(1, width * 0.10);
    ui->tableWidget->setColumnWidth(2, width * 0.06);
    ui->tableWidget->setColumnWidth(3, width * 0.10);
    ui->tableWidget->setColumnWidth(4, width * 0.20);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

    QStringList tableHeadList;
    tableHeadList.push_back(tr("音乐标题"));
    tableHeadList.push_back(tr("歌手"));
    tableHeadList.push_back(tr("专辑"));
    tableHeadList.push_back(tr("时长"));
    tableHeadList.push_back(tr("大小"));


    ui->tableWidget->setHorizontalHeaderLabels(tableHeadList);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);



    for (int i = 0; i != _vSongPath.size(); i++) {
        ui->tableWidget->setRowHeight(i,24);

        QString songName;
        QString singer;
        QString spName;
        spName = _vSongPath[i].split("/").last();
        singer = spName.split("-").first();
        songName = spName.split("-").last().split(".").first();

        QTableWidgetItem *itemTitle = new QTableWidgetItem(songName);
        QTableWidgetItem *iterSinger = new QTableWidgetItem(singer);
        itemTitle->setTextAlignment(Qt::AlignLeft);
        iterSinger->setTextAlignment(Qt::AlignLeft);
        ui->tableWidget->setItem(i,0,itemTitle);
        ui->tableWidget->setItem(i,1,iterSinger);
    }

    ui->tableWidget->setStyleSheet(
                "QHeaderView{"
                "   background:#464646;"
                "}"
                "QHeaderView::section{"
                "   color:#ffffff;"
                "   background:#2b2b2b;"
                "}"
                "QTableWidget{"
                "   background:#2f2f2f;"
                "   alternate-background-color:#2b2b2b;"
                "   border:none;"
                "   color:#ffffff;"
                "}"
                "QTableWidget::item::selected{"
                "   color:#ffffff;"
                "   background:#373737;"
                "}"
                );


    connect(ui->tableWidget,&QTableWidget::itemDoubleClicked,[=](QTableWidgetItem *item){
        MusicPlayer::getSingleton().play(_vSongPath[item->row()].toStdString());
    });

}

play_table_widget::~play_table_widget()
{
    delete ui;
}
