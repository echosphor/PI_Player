#include "mainwindow.h"
#include <QFileInfoListIterator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*
    QFontDatabase::addApplicationFont("font/方正少儿_gbk.ttf");//自定义字体加载
    QFontDatabase::addApplicationFont("font/方正喵呜体.ttf");
    QFontDatabase::addApplicationFont("font/华康少女文字W5(P).ttf");
    qApp->setFont(QFont("方正少儿_gbk",12));
    */
    videoWindow=new QWidget(this);
    player=new myplayer(videoWindow,this);
    plugMenu=menuBar()->addMenu(tr("插件"));
    optionMenu=menuBar()->addMenu(tr("选项"));
    setCentralWidget(videoWindow);
    if(0==getPlugin(actionArray,widgetArray))
    {
        QAction *tpAction=new QAction(tr("未发现插件"),this);
        plugMenu->addAction(tpAction);
    }
    plugMenu->addSeparator();
    plugMangerAction=new QAction(tr("插件管理"),this);
    plugMenu->addAction(plugMangerAction);
    createTrayMenu();
    createTrayIcon();
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{

}
int MainWindow::getPlugin(QAction *actionArray[],pluginWidget*widgetArray[])
{
    int pluginCount=0;
    QDir pluginDir("plugin");
    if(!pluginDir.exists())
    {
        QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\PI_Player",QSettings::NativeFormat);//读取注册表获取安装目录
        QString PlayerPath=reg.value("path").toString()+"plugin";
        pluginDir.cd(PlayerPath);
    }
    pluginDir.setFilter(QDir::Files|QDir::NoDotAndDotDot);
    QStringList fileFilters;
    fileFilters << "*.dll";
    pluginDir.setNameFilters(fileFilters);
    QFileInfoList list=pluginDir.entryInfoList();
    for(int i=0;i<list.size();i++)
    {
        QFileInfo fi=list.at(i);
        QLibrary lib("plugin/"+fi.fileName());
        lib.load();
        typedef pluginWidget* (*MyPrototype)(QWidget*);
        MyPrototype getplugin=(MyPrototype)lib.resolve("getplugin");//函数原型：pluginWidget* getplugin(QWidget *parent);
        if(getplugin!=NULL)
        {
            //actionArray[i]=new QAction(fi.fileName(),this);
            //plugMenu->addAction(actionArray[i]);
            widgetArray[i]=getplugin(NULL);//NULL，即独立的widget
            //this->layout()->addWidget(widgetArray[i]);//把插件widget放到主窗口中
            widgetArray[i]->setMainWindow(this);
            widgetArray[i]->setpluginLoc();
            widgetArray[i]->hide();//初始隐藏 @2012.09.09
            if(i!=0)
                tabifyDockWidget(widgetArray[i-1],widgetArray[i]);//插件index并不连续
            plugMenu->addAction(actionArray[i]=widgetArray[i]->toggleViewAction());
           // connect(actionArray[i],SIGNAL(triggered()),widgetArray[i],SLOT(setpluginLoc()));
            pluginCount+=1;
        }else{
            if(lib.isLoaded())
                lib.unload();
        }
    }
    return pluginCount;
}
myplayer* MainWindow::getPlayer()
{
    return player;
}
void MainWindow::customEvent(QEvent *event)
{
    if(event->type()==SET_URL_EVENT)
    {
        player->setFilePath(((QSetUrlEvent*)event)->url);
        player->play();
    }
    if(event->type()==CTRL_EVENT)
    {
        if(((QCtrlPlayerEvent*)event)->getType()==PAUSE)
            player->pause();
        if(((QCtrlPlayerEvent*)event)->getType()==FULLSCREEN)
            player->getVideoWidget()->setFullScreen(!(player->getVideoWidget()->isFullScreen()));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> fileList=event->mimeData()->urls();
    if(fileList.length()==0)
        return;
    if(fileList.length()==1)
    {
        if(player->IsPlaying())
            player->stop();
        player->setFilePath(fileList.at(0).toString());
        player->play();
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    //hide();
    trayIcon->hide();
    if(this->getPlayer()->IsPlaying())
        this->getPlayer()->stop();
}

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/images/Icon.ico"));
    trayIcon->setToolTip("PIPlayer");
    trayIcon->setContextMenu(sysTrayMenu);
    trayIcon->show();
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}
void MainWindow::createTrayMenu()
{
    sysTrayMenu=new QMenu(QApplication::desktop());
    restoreAction=new QAction(tr("还原"),this);
    connect(restoreAction,SIGNAL(triggered()),this,SLOT(showNormal()));
    quitAction=new QAction(tr("退出"),this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(quit()));
    sysTrayMenu->addAction(restoreAction);
    sysTrayMenu->addSeparator();
    sysTrayMenu->addAction(quitAction);
    sysTrayMenu->setDefaultAction(restoreAction);
}
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    default:break;
    }
}
void MainWindow::quit()
{
    trayIcon->hide();
    if(this->getPlayer()->IsPlaying())
        this->getPlayer()->stop();
    qApp->quit();
}
