#ifndef MYPLAYER_H
#define MYPLAYER_H
#include <QObject>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QStyle>
#include <QTextCodec>
#include <QTime>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/seekslider.h>
#include <phonon/volumeslider.h>
#include <QDropEvent>
#include <QMovie>
#include <QTimer>
#include <QPropertyAnimation>
#include "myvideowidget.h"

class myVideoWidget;

class myplayer : public QWidget
{
    Q_OBJECT
public:
    myplayer(QWidget *parent,QWidget *top);
    ~myplayer();
    QString filePath;
    QString filename;
    void setFilePath(QString url);
    QString getFilePath();
    bool IsPlaying();
    myVideoWidget* getVideoWidget();
public slots:
    void getFile();
    void play();
    void pause();
    void stop();
    void showFloatingWidget();
    void hideFloatingWidget();
signals:
    void enableFullScreen(bool yes);
protected:

private:
    void createFloatingWidget();
    QLabel *floatingTopLabel;
    QLabel *floatingTopTimeLabel;
    QHBoxLayout *fTopWidgetLayout;
    QWidget *floatingTopWidget;
    QWidget *floatingWidget;
    QHBoxLayout *fWidgetLayout;
    Phonon::SeekSlider *floatingSeekSlider;
    Phonon::VolumeSlider *floatingVolumeSlider;
    QLabel *floatingTimeLabel;
    bool isplay;
    QWidget *topWindow;
    QLabel *screenLabel;
    myVideoWidget *vwidget;
    Phonon::MediaObject *playercore;
    Phonon::AudioOutput *audio;
    Phonon::VolumeSlider *volumeSlider;
    Phonon::SeekSlider *seekSlider;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    QLabel *timeLabel;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *openFileButton;
private slots:
    void upDateTime(qint64 msec);
};
#endif // MYPLAYER_H

