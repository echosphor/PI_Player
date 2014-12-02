#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H


#include <QtGui>
#include <QAction>
#include <QDialog>
#include <phonon/VideoWidget>
#include <QMainWindow>

namespace Ui {
    class settingDialog;
}

class settingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingDialog(QWidget *parent = 0);
    ~settingDialog();
    void setVideoWidget(Phonon::VideoWidget *vwidget);
    void setMainWindow(QWidget *window);
public slots:
    void setAspect(int val);
    void setScale(int val);
    void setSaturation(int val);
    void setContrast(int val);
    void setHue(int val);
    void setBrightness(int val);
    void updateSetting();
    void setOpacity(int val);
private:
    Phonon::VideoWidget *videoWidget;
    QWidget *mainWindow;
    Ui::settingDialog *ui;
};

#endif // SETTINGDIALOG_H
