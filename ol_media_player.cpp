#include "ol_media_player.h"
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <iostream>

using namespace std;

OL_Media_Player::OL_Media_Player(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(500, 600);
    this->setWindowTitle("Oguz Media Player");

    durationLabel = new QLabel(this);
    durationLabel->setText("00:00");
    posLabel = new QLabel(this);
    posLabel->setText("00");

    mainFrame = new QFrame();
    mainVBox = new QVBoxLayout();
    mainFrame->setLayout(mainVBox);

    setCentralWidget(mainFrame);

    controlsVLayout = new QVBoxLayout();
    controlsHLayout = new QHBoxLayout();
    controlsVLayout->addLayout(controlsHLayout);

    mediaPlayer = new QMediaPlayer(this);
    videoProbe = new QVideoProbe(this);
    videoProbe->setSource(mediaPlayer);
    playList = new QMediaPlaylist();
    videoWidget = new QVideoWidget;
    mediaPlayer->setVideoOutput(videoWidget);
    mainVBox->addWidget(videoWidget);
    mainVBox->addLayout(controlsVLayout);
    fileMenu = new QMenu("File");
    editMenu = new QMenu("Edit");
    helpMenu = new QMenu("Help");
    // openItem = new QMenuItem(fileMenu);


    openAction = new QAction(this);
    openAction->setText("Open");
    QAction* action = new QAction(this);
    action->setText( "Quit" );
    helpAction = new QAction(this);
    helpAction->setText("Help");
    QAction *aboutAction = new QAction(this);
    aboutAction->setText("About");

    // menuBar()->addMenu( "File" )->addAction( action );
    fileMenu->addAction(openAction);
    fileMenu->addAction(action);
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
    menuBar()->addMenu(fileMenu);

    // menuBar()->addAction(openAction);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(helpMenu);
    mediaSlider = new QSlider(Qt::Horizontal);
    // mediaSlider->setRange(0, 100);

    playButton = new QPushButton("Play");
    stopButton = new QPushButton("Stop");
    pauseButton = new QPushButton("Pause");

    controlsVLayout->addWidget(mediaSlider);
    controlsHLayout->addWidget(playButton);
    controlsHLayout->addWidget(stopButton);
    controlsHLayout->addWidget(pauseButton);
    controlsHLayout->addWidget(durationLabel);
    controlsHLayout->addWidget(posLabel);

    connect(action, SIGNAL(triggered()), SLOT(close()) );
    connect(openAction, SIGNAL(triggered()), this, SLOT(open_media()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(show_about_dialog()));
    connect(playButton, SIGNAL(clicked(bool)), this, SLOT(play_media()));
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stop_media()));
    connect(pauseButton, SIGNAL(clicked(bool)), this, SLOT(pause_media()));
    // connect(mediaSlider, SIGNAL(valueChanged(int)), this, SLOT(mp_slider_moved(int)));
    connect(mediaSlider, &QSlider::sliderMoved, this, &OL_Media_Player::seek);
    // connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(media_position_changed(qint64)));
    // connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(media_duration_changed(qint64)));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

    // mediaSlider->setTickInterval(1);
    // mediaPlayer->bind(mediaSlider);
    pos = 0;
}

OL_Media_Player::~OL_Media_Player()
{

}

// orig.
void OL_Media_Player::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    mediaSlider->setMaximum(m_duration);
}

// orig.
void OL_Media_Player::positionChanged(qint64 progress)
{
    if (!mediaSlider->isSliderDown()) {
        mediaSlider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}


// orig.
void OL_Media_Player::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
                        m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    durationLabel->setText(tStr);
}


// orig.
void OL_Media_Player::seek(int seconds)
{
    mediaPlayer->setPosition(seconds * 1000);
    pos = (seconds * 1000);
}


void OL_Media_Player::open_media()
{
    QFileDialog *fd = new QFileDialog(this, tr("Open files"));
    //fd->show();
    QString sourceFile = fd->getOpenFileName();
    // QString mediaFile = "/home/oguz/Videos/Filmler/cilgin_hirsiz_II_(2).mp4";
    QUrl url = QUrl::fromLocalFile(sourceFile);
    QList<QMediaContent> content;
    // content.push_back(url); 1. örnek alıntı
    // content.append(url); 2. örnek alıntı
    // playList->addMedia(content); örnek
    playList->addMedia(url);
    // playList->load(url);
    fd->close();
    delete fd;
}

void OL_Media_Player::play_media()
{
    mediaPlayer->setPosition(pos);
    mediaPlayer->setPlaylist(playList);
    if (mediaPlayer->state() == QMediaPlayer::PausedState) {
        pauseButton->setText("Pause");
    }
    mediaPlayer->play();
}


void OL_Media_Player::stop_media()
{
    mediaPlayer->stop();
}


void OL_Media_Player::pause_media()
{
    if(mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        pauseButton->setText("Paused");
        pos = mediaPlayer->position();
    }
    else {
        pauseButton->setText("Pause");
        mediaPlayer->setPosition(pos);
        mediaPlayer->play();
    }

}


// my old func
void OL_Media_Player::mp_slider_moved(int position)
{
    // sliderMaxRange = mediaPlayer->duration() / 1000;
    // mediaSlider->setRange(0, sliderMaxRange);
    position = mediaSlider->value();
    sliderPos = position;

    mediaPlayer->setPosition(position * 1000);
    pos = mediaPlayer->position();
    // attention
    pos = mediaSlider->value() * 1000;
    QVariant posVar = pos;
    posLabel->setText(posVar.toString());
}


// my old func
void OL_Media_Player::media_position_changed(qint64 medya_position)
{
 //   sliderMaxRange = mediaPlayer->duration();
 //   mediaSlider->setRange(0, sliderMaxRange);
 /*
    for(int i = 0; i < sliderMaxRange; i++) {
        mediaSlider->setValue(i);
        QThread::sleep(1);
    }
    */
}

 // my old func
void OL_Media_Player::media_duration_changed(qint64)
{
    timeVar = mediaPlayer->duration();
    int ms = mediaPlayer->duration();
    int mnt = ms / (1000*60);
    int hour = mnt / 60;

    QTime tm = timeVar.toTime();
    tm.setHMS(hour, mnt, ms);
    QVariant labelVar = ms / 1000;
}


void OL_Media_Player::show_about_dialog()
{
    QDialog *aboutDialog = new QDialog();
    aboutDialog->resize(200, 200);
    QLabel *aboutLabel = new QLabel(aboutDialog);
    aboutLabel->setText("Developer:\nOğuz Özözgen");
    aboutDialog->show();
}
