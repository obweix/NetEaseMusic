#include "playlist_widget.h"
#include "music_player.h"
#include <QtWidgets>

PlaylistWidget::PlaylistWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void PlaylistWidget::init()
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    QHBoxLayout *hlPlayList = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QStackedLayout *stackedLayout = new QStackedLayout();
    stackedLayout->setSpacing(0);
    stackedLayout->setMargin(0);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    hlPlayList->setSpacing(0);
    hlPlayList->setMargin(0);


    _btnLocalMusic = new QPushButton(tr("本地音乐"));
    _btnLikedMusic = new QPushButton(tr("我喜欢的音乐"));
    _btnLocalMusic->setMaximumWidth(200);
    _btnLocalMusic->setMinimumWidth(200);
    _btnLikedMusic->setMaximumWidth(200);
    _btnLikedMusic->setMinimumWidth(200);
    vLayout->addWidget(_btnLocalMusic);
    vLayout->addWidget(_btnLikedMusic);
    vLayout->addStretch(1);


    _vSongPath = MusicPlayer::getSingleton().getMusicFilePath();

    _playTable = new QTableWidget(_vSongPath.size(),5,this);
    QStringList tableHeadList;
    tableHeadList.push_back(tr("音乐标题"));
    tableHeadList.push_back(tr("歌手"));
    tableHeadList.push_back(tr("专辑"));
    tableHeadList.push_back(tr("时长"));
    tableHeadList.push_back(tr("大小"));
    _playTable->setHorizontalHeaderLabels(tableHeadList);
    _playTable->horizontalHeader()->setStretchLastSection(true);
    _playTable->verticalHeader()->setVisible(false);
    _playTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    for (int i = 0; i != _vSongPath.size(); i++) {
        _playTable->setRowHeight(i,24);

        QString songName;
        QString singer;
        QString spName;
        spName = _vSongPath[i].split("/").last();
        singer = spName.split("-").first();
        songName = spName.split("-").last().split(".").first();

        QTableWidgetItem *itemTitle = new QTableWidgetItem(songName);
        QTableWidgetItem *iterSinger = new QTableWidgetItem(singer);
        _playTable->setItem(i,0,itemTitle);
        _playTable->setItem(i,1,iterSinger);
    }

    hlPlayList->addWidget(_playTable,Qt::AlignHCenter);

    hLayout->addLayout(vLayout,0);
    hLayout->addLayout(hlPlayList,1);
    setLayout(hLayout);

    connect(_playTable,&QTableWidget::itemDoubleClicked,[=](QTableWidgetItem *item){
        MusicPlayer::getSingleton().play(_vSongPath[item->row()].toStdString());
    });
}
