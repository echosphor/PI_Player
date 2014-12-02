#include "myplayer.h"
myplayer::myplayer(QWidget *parent,QWidget *top)
{
    topWindow=top;
    playercore=new Phonon::MediaObject(parent);
    vwidget=new myVideoWidget(parent);
    vwidget->setPlayer(this);
    audio=new Phonon::AudioOutput(Phonon::VideoCategory,parent);
    Phonon::createPath(playercore,vwidget);
    Phonon::createPath(playercore,audio);
    vwidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
    volumeSlider = new Phonon::VolumeSlider;
    volumeSlider->setAudioOutput(audio);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    seekSlider = new Phonon::SeekSlider;
    seekSlider->setMediaObject(playercore);
    seekSlider->setMaximumHeight(20);

    playButton=new QPushButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playButton->setMaximumWidth(30);
    stopButton=new QPushButton(this);
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setMaximumWidth(30);
    openFileButton=new QPushButton(tr("打开"),this);
    openFileButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    openFileButton->setMaximumWidth(60);

    timeLabel=new QLabel("00:00/00:00",this);

    screenLabel=new QLabel(this);
    screenLabel->setMinimumSize(500,281);
    screenLabel->setMargin(2);
    screenLabel->setFrameStyle(QFrame::StyledPanel|QFrame::Sunken);
    screenLabel->setLineWidth(2);
    screenLabel->setAutoFillBackground(true);
    QPalette pal;
    pal.setBrush(QPalette::WindowText,Qt::white);
    screenLabel->setStyleSheet("border-image:url(:/images/screen.png) ; border-width:3px");
    screenLabel->setPalette(pal);

    /*加载GIF for Label
    QMovie *movie = new QMovie(":/images/test.gif");
    screenLabel->setMovie(movie);
    movie->start();
    screenLabel->setScaledContents(true);
    */

    hLayout=new QHBoxLayout;
    vLayout=new QVBoxLayout;
    vLayout->addWidget(vwidget);
    vLayout->addWidget(screenLabel);
    vLayout->addWidget(seekSlider);
    hLayout->addWidget(openFileButton);
    hLayout->addWidget(playButton);
    hLayout->addWidget(stopButton);
    hLayout->addStretch();
    hLayout->addWidget(timeLabel);
    hLayout->addWidget(volumeSlider);
    vLayout->addLayout(hLayout);
    vLayout->setContentsMargins(8,8,8,8);
    parent->setLayout(vLayout);

    vwidget->setVisible(false);
    screenLabel->setVisible(true);

    createFloatingWidget();

    connect(openFileButton,SIGNAL(clicked()),this,SLOT(getFile()));
    connect(playButton,SIGNAL(clicked()),this,SLOT(pause()));
    connect(stopButton,SIGNAL(clicked()),this,SLOT(stop()));
    connect(playercore,SIGNAL(finished()),this,SLOT(stop()));
    connect(playercore,SIGNAL(tick(qint64)),this,SLOT(upDateTime(qint64)));
    connect(this,SIGNAL(enableFullScreen(bool)),vwidget,SLOT(setEnableFullScreen(bool)));
    /*control floating widget*/
    connect(vwidget,SIGNAL(cursorNearBottom()),this,SLOT(showFloatingWidget()));
    connect(vwidget,SIGNAL(cursorFarEdges()),this,SLOT(hideFloatingWidget()));

    parent->show();
    isplay=false;
}
myplayer::~myplayer()
{
    stop();
}
void myplayer::getFile()
{
    QString file=QFileDialog::getOpenFileName(this,tr("video file"),"",tr("video File (*.mkv *.mp4 *.m4v *.rmvb *.rm *.avi *.flv *.wmv *.asf *.asx *.mpg *.mpeg *.3pg *.mov);;audio file (*.mp3 *.ogg *.wma *.mp3 *.acc *.ape);;All Files(*.*)"));
    filePath=file;
    play();
}
void myplayer::play()
{
    filename=filePath.right(filePath.length()-filePath.lastIndexOf('/') - 1);
    if(filePath.isEmpty())
        return ;
    topWindow->setWindowTitle(filename);
    screenLabel->setVisible(false);
    vwidget->setVisible(true);
    playercore->setCurrentSource(Phonon::MediaSource(filePath));
    playercore->play();
    isplay=true;
    emit enableFullScreen(true);
}
void myplayer::pause()
{
    if(playercore->state()== Phonon::PlayingState||playercore->state()== Phonon::PausedState)
    {
        if(isplay)
        {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            playercore->pause();
            topWindow->setWindowTitle(filename+tr(" - 暂停"));
        }
        else
        {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            playercore->play();
            topWindow->setWindowTitle(filename);
        }
        isplay=!isplay;
    }
}
void myplayer::stop()
{
    if(vwidget->isFullScreen())
        vwidget->setFullScreen(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    filePath="";
    filename="";
    isplay=false;
    playercore->setCurrentSource(Phonon::MediaSource(filePath));
    timeLabel->setText("00:00/00:00");
    topWindow->setWindowTitle(tr("PI_Player"));
    emit enableFullScreen(false);
    playercore->stop();
    vwidget->setVisible(false);
    screenLabel->setVisible(true);
}
void myplayer::upDateTime(qint64 msec)
{
    QString timeString;
    long len = playercore->totalTime();
    long pos = playercore->currentTime();
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    timeLabel->setText(timeString);
    if(vwidget->isFullScreen())
    {
        floatingTimeLabel->setText(timeString);
        floatingTopTimeLabel->setText(QTime::currentTime().toString(Qt::SystemLocaleLongDate));
    }
}
void myplayer::setFilePath(QString url)
{
    filePath=url;
}
QString myplayer::getFilePath()
{
    return filePath;
}
bool myplayer::IsPlaying()
{
    return isplay;
}
myVideoWidget* myplayer::getVideoWidget()
{
    return vwidget;
}

void myplayer::createFloatingWidget()
{
    floatingWidget=new QWidget(this,Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    fWidgetLayout=new QHBoxLayout;
    floatingSeekSlider=new Phonon::SeekSlider;
    floatingSeekSlider->setMediaObject(playercore);
    floatingVolumeSlider=new Phonon::VolumeSlider;
    floatingVolumeSlider->setAudioOutput(audio);
    floatingVolumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    floatingTimeLabel=new QLabel(floatingWidget);
    fWidgetLayout->addWidget(floatingSeekSlider);
    fWidgetLayout->addWidget(floatingTimeLabel);
    fWidgetLayout->addWidget(floatingVolumeSlider);
    floatingWidget->setLayout(fWidgetLayout);

    floatingTopWidget=new QWidget(this,Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    floatingTopLabel=new QLabel(floatingTopWidget);
    floatingTopTimeLabel=new QLabel(floatingTopWidget);
    floatingTopTimeLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    fTopWidgetLayout=new QHBoxLayout;
    fTopWidgetLayout->addWidget(floatingTopLabel);
    fTopWidgetLayout->addWidget(floatingTopTimeLabel);
    floatingTopWidget->setLayout(fTopWidgetLayout);
}

void myplayer::showFloatingWidget()
{
    #define MARGIN 50
    floatingWidget->show();
    floatingWidget->resize(vwidget->width(),MARGIN);
    QPropertyAnimation * animation;
    animation = new QPropertyAnimation(floatingWidget, "pos");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->setEndValue(QPoint(0,vwidget->height()-MARGIN));
    animation->setStartValue(QPoint(0,vwidget->height()));
    animation->start();

    #define MARGINTOP 20
    floatingTopWidget->show();
    floatingTopWidget->resize(vwidget->width(),MARGINTOP);
    floatingTopLabel->setText(filename);
    floatingTopLabel->setAlignment(Qt::AlignHCenter);
    QPropertyAnimation * animationTop;
    animationTop = new QPropertyAnimation(floatingTopWidget, "pos");
    animationTop->setDuration(300);
    animationTop->setEasingCurve(QEasingCurve::InBounce);
    animationTop->setEndValue(QPoint(0,0));
    animationTop->setStartValue(QPoint(0,-MARGINTOP));
    animationTop->start();
}
void myplayer::hideFloatingWidget()
{
    floatingWidget->hide();
    floatingTopWidget->hide();
}
