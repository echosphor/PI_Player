#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QMainWindow>
#include <QDockWidget>

class pluginWidget:public QDockWidget
{
public:
    pluginWidget();
    virtual void setpluginLoc()=0;
    virtual void setMainWindow(QMainWindow *mainWindow)=0;
    virtual QMainWindow *getMainWindow()=0;
};

#endif // PLUGINWIDGET_H
