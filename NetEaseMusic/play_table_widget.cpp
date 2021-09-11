#include "play_table_widget.h"
#include "ui_play_table_widget.h"
#include "music_player.h"

#include<QFileDialog>

play_table_widget::play_table_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::play_table_widget)
{
    ui->setupUi(this);



    ui->btn_scan_music->setStyleSheet("text-align:left;"
                                      "color:white;"
                                      " margin-left: 10px;");

    ui->lb_save_dir->setStyleSheet("background-color:#2b2b2b;");
    ui->lb_save_dir->setText(QString(tr("储存目录：")) + DEFAULT_MUSIC_PATH);

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

    initTable();

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
        MusicPlayer::getSingleton().play(item->row());
    });

    // 选择文件夹并扫描
    connect(ui->btn_scan_music,&QPushButton::clicked,[&]{
        QString path = QFileDialog::getExistingDirectory();
        MusicPlayer::getSingleton().scanDir(path);
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->lb_save_dir->setText(QString(tr("储存目录：")) + path);
        initTable();
    });

}

void play_table_widget::initTable()
{
    _vSongPath = MusicPlayer::getSingleton().getMusicFilePath();
    ui->tableWidget->setRowCount(_vSongPath.size());
    for (int i = 0; i != _vSongPath.size(); i++) {
        ui->tableWidget->setRowHeight(i,24);

        QString songName;
        QString singer;
        QString spName;
        QString album;
        QString songLen;
        QString fileSize;
        spName = _vSongPath[i].split("/").last();
        singer = spName.split("-").first();
        songName = spName.split("-").last().split(".").first();

        QTableWidgetItem *itemTitle = new QTableWidgetItem(songName);
        QTableWidgetItem *itemSinger = new QTableWidgetItem(singer);
        QTableWidgetItem *itemAlbum = new QTableWidgetItem(album);
        QTableWidgetItem *itemSongLen = new QTableWidgetItem(songLen);
        QTableWidgetItem *itemFileSize = new QTableWidgetItem(fileSize);
        itemTitle->setTextAlignment(Qt::AlignLeft);
        itemSinger->setTextAlignment(Qt::AlignLeft);
        ui->tableWidget->setItem(i,0,itemTitle);
        ui->tableWidget->setItem(i,1,itemSinger);
        ui->tableWidget->setItem(i,2,itemAlbum);
        ui->tableWidget->setItem(i,3,itemSongLen);
        ui->tableWidget->setItem(i,4,itemFileSize);
    }
}

play_table_widget::~play_table_widget()
{
    delete ui;
}
