#include "myvideowidget.h"

myVideoWidget::myVideoWidget(QWidget *parent)
    :Phonon::VideoWidget(parent)
{
    okToFullScreen=false;
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);//设置vwidget的size
    createMenu();
    autoHideCursorTimer=new QTimer(this);
    autoHideCursorTimer->setInterval(3000);
    autoHideCursorTimer->start();
    connect(autoHideCursorTimer,SIGNAL(timeout()),this,SLOT(hideCursor()));
}
myVideoWidget::~myVideoWidget()
{

}
void myVideoWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    e->accept();
    if(okToFullScreen&&(e->button()==Qt::LeftButton))
        setFullScreen(!isFullScreen());
}

void myVideoWidget::setEnableFullScreen(bool yes)
{
    okToFullScreen=yes;
}

void myVideoWidget::mouseReleaseEvent(QMouseEvent *e)
{
    switch(e->button())
    {
        case Qt::RightButton:
            e->accept();
            popMenu();
            break;
        case Qt::LeftButton:
            e->accept();
            player->pause();
            break;
        default :
            e->ignore();
            break;
    }
}

void myVideoWidget::mouseMoveEvent(QMouseEvent *e)
{
    #define MARGIN 50
    if(!isFullScreen())
    {
        e->ignore();
        return;
    }
    e->accept();
    /*检测鼠标位置*/
    if (e->y()>=height() - MARGIN) {
            if (!near_bottom) {
                    emit cursorNearBottom();
                    near_bottom = true;
            }
    } else {
            if (near_bottom) {
                    emit cursorFarEdges();
                    near_bottom = false;
            }
    }

    if (e->y() < MARGIN) {
            if (!near_top) {
                    emit cursorNearTop();
                    near_top = true;
            }
    } else {
            if (near_top) {
                    emit cursorFarEdges();
                    near_top = false;
            }
    }

    if (cursor().shape() != Qt::ArrowCursor)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void myVideoWidget::setPlayer(myplayer *mainPlayer)
{
    player=mainPlayer;
}

void myVideoWidget::aspectChanged(QAction *act)
{
    if (act->text() == tr("16/9"))
        setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    else if (act->text() == tr("Scale"))
        setAspectRatio(Phonon::VideoWidget::AspectRatioWidget);
    else if (act->text() == tr("4/3"))
        setAspectRatio(Phonon::VideoWidget::AspectRatio4_3);
    else
        setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
}
void myVideoWidget::scaleChanged(QAction *act)
{
    if (act->text() == tr("Scale and crop"))
        setScaleMode(Phonon::VideoWidget::ScaleAndCrop);
    else
        setScaleMode(Phonon::VideoWidget::FitInView);
}
void myVideoWidget::openFile()
{
    player->getFile();
}
void myVideoWidget::openUrl()
{
    QString sourceURL;
    bool ok = false;
    sourceURL = QInputDialog::getText(this, tr("打开URL"), tr("请输入一个正确的URL地址:"), QLineEdit::Normal, sourceURL, &ok);
    if (ok && !sourceURL.isEmpty())
    {
        player->setFilePath(sourceURL);
        player->play();
    }
}

void myVideoWidget::createMenu()
{
    fileMenu = new QMenu(this);
    QAction *openFileAction = fileMenu->addAction(tr("打开文件"));
    connect(openFileAction,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    QAction *openUrlAction =fileMenu->addAction(tr("打开地址"));
    connect(openUrlAction,SIGNAL(triggered()),this,SLOT(openUrl()));

    fileMenu->addSeparator();

    QMenu *aspectMenu = fileMenu->addMenu(tr("高度比"));
    QActionGroup *aspectGroup = new QActionGroup(aspectMenu);
    connect(aspectGroup, SIGNAL(triggered(QAction*)), this, SLOT(aspectChanged(QAction*)));
    aspectGroup->setExclusive(true);
    QAction *aspectActionAuto = aspectMenu->addAction(tr("自动"));
    aspectActionAuto->setCheckable(true);
    aspectActionAuto->setChecked(true);
    aspectGroup->addAction(aspectActionAuto);
    QAction *aspectActionScale = aspectMenu->addAction(tr("伸展"));
    aspectActionScale->setCheckable(true);
    aspectGroup->addAction(aspectActionScale);
    QAction *aspectAction16_9 = aspectMenu->addAction(tr("16/9"));
    aspectAction16_9->setCheckable(true);
    aspectGroup->addAction(aspectAction16_9);
    QAction *aspectAction4_3 = aspectMenu->addAction(tr("4/3"));
    aspectAction4_3->setCheckable(true);
    aspectGroup->addAction(aspectAction4_3);

    QMenu *scaleMenu = fileMenu->addMenu(tr("缩放模式"));
    QActionGroup *scaleGroup = new QActionGroup(scaleMenu);
    connect(scaleGroup, SIGNAL(triggered(QAction*)), this, SLOT(scaleChanged(QAction*)));
    scaleGroup->setExclusive(true);
    QAction *scaleActionFit = scaleMenu->addAction(tr("适应视图"));
    scaleActionFit->setCheckable(true);
    scaleActionFit->setChecked(true);
    scaleGroup->addAction(scaleActionFit);
    QAction *scaleActionCrop = scaleMenu->addAction(tr("拉伸视图"));
    scaleActionCrop->setCheckable(true);
    scaleGroup->addAction(scaleActionCrop);


    fileMenu->addSeparator();
    QAction *snapACtion=fileMenu->addAction(tr("截图"));
    connect(snapACtion,SIGNAL(triggered()),this,SLOT(snap()));

    fileMenu->addSeparator();
    QAction *settingsAction = fileMenu->addAction(tr("设置"));
    setting=new settingDialog;
    setting->setVideoWidget(this);
    setting->setMainWindow(window());// top widget
    connect(settingsAction,SIGNAL(triggered()),setting,SLOT(show()));
}
void myVideoWidget::snap()
{
    QString filedir=player->getFilePath().left(player->getFilePath().lastIndexOf('/'));
    QDateTime datetime=QDateTime::currentDateTime();
    QString picName;
    picName.sprintf("%d-%d-%d#%dh%dm%ds", datetime.date().year(),datetime.date().month(),datetime.date().day(),datetime.time().hour(),datetime.time().minute(),datetime.time().second());

    QString picPath=QString(filedir+"/"+picName+".jpg");

    QPixmap pic=QPixmap::grabWidget(this);//videowidget->snapshot() is a bug in qt-phonon
    if(player->IsPlaying())
        if(pic.save(picPath))
            QMessageBox::information(this,"截图","截图保存为"+picPath);
}
void myVideoWidget::popMenu()
{
    setting->updateSetting();
    fileMenu->exec(QCursor::pos());
}
void myVideoWidget::hideCursor()
{
    if(isFullScreen())
        setCursor(QCursor(Qt::BlankCursor));
}
