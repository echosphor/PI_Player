#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H
#include <phonon/VideoWidget>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include "myplayer.h"
#include "settingdialog.h"

class myplayer;

class myVideoWidget : public Phonon::VideoWidget
{
    Q_OBJECT
public:
    myVideoWidget(QWidget *parent=0);
    ~myVideoWidget();
    void setPlayer(myplayer *mainPlayer);
public slots:
    void scaleChanged(QAction *);
    void aspectChanged(QAction *);
    void openFile();
    void openUrl();
    void snap();
    void popMenu();
    void hideCursor();
signals:
    void cursorNearBottom();
    void cursorFarEdges();
    void cursorNearTop();
protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
private:
    QTimer *autoHideCursorTimer;
    bool near_bottom;
    bool near_top;
    void createMenu();
    QMenu *fileMenu;
    settingDialog *setting;
    bool okToFullScreen;
    myplayer *player;
   // myplayer player;
private slots:
    void setEnableFullScreen(bool yes);
};

#endif // MYVIDEOWIDGET_H
