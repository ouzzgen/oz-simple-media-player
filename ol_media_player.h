#ifndef OL_MEDIA_PLAYER_H
#define OL_MEDIA_PLAYER_H

#include <QtGui/QtGui>
#include <QMainWindow>
#include <QtWidgets>
#include <QtMultimedia/QtMultimedia>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimedia/QMediaPlayer>
#include <QPushButton>
#include <QBoxLayout>
#include <QSlider>


class OL_Media_Player : public QMainWindow
{
    Q_OBJECT

public:
    OL_Media_Player(QWidget *parent = 0);
    ~OL_Media_Player();

private slots:
    void open_media();
    void play_media();
    void stop_media();
    void pause_media();
    void show_about_dialog();
    void mp_slider_moved(int position);
    void media_position_changed(qint64 medya_position);
    void media_duration_changed(qint64);

    void seek(int seconds);
    void positionChanged(qint64 progress);
    void durationChanged(qint64 duration);
    void updateDurationInfo(qint64 currentInfo);
private:
    QMenu *fileMenu;
    QMenu *editMenu, *helpMenu;

    // QMenuItem *openItem;
    QAction *openAction, *helpAction;
    //QFileDialog fd;
    QFrame *mainFrame;
    QVBoxLayout *mainVBox;
    QVBoxLayout *controlsVLayout;
    QHBoxLayout *controlsHLayout;
    QSlider *mediaSlider = nullptr;
    QMediaPlayer *mediaPlayer;
    QVideoProbe *videoProbe;
    QVideoWidget *videoWidget;
    QString *sourceFile;
    QUrl url;
    QMediaPlaylist *playList;
    QPushButton *playButton, *pauseButton, *stopButton;
    QTime tm;
    QTimer *tmr;
    QVariant timeVar;
    QTimeEdit *timeEdit;
    QLabel *durationLabel, *posLabel;
    int pos, sliderPos, sliderMaxRange;
    qint64 m_duration;
};

#endif // OL_MEDIA_PLAYER_H
