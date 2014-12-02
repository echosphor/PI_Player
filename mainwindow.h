#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMenuBar>
#include <QLibrary>
#include "myvideowidget.h"
#include "myplayer.h"
#include "pluginwidget.h"
#include "qseturlevent.h"
#include "qctrlplayerevent.h"
#include <QImage>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    int getPlugin(QAction *actionArray[],pluginWidget *widgetArray[]);
    ~MainWindow();
    myplayer* getPlayer();
    void createTrayIcon();
    void createTrayMenu();
public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void quit();
protected:
     void customEvent( QEvent *event );
     void dragEnterEvent(QDragEnterEvent *event);
     void dropEvent(QDropEvent *event);
     void closeEvent(QCloseEvent *e);
private:
    QSystemTrayIcon *trayIcon;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *sysTrayMenu;
    myplayer *player;
    QWidget *videoWindow;
    QMenu *plugMenu;
    QMenu *optionMenu;
    QAction *actionArray[10],*plugMangerAction;
    pluginWidget *widgetArray[10];
};

#endif // MAINWINDOW_H
