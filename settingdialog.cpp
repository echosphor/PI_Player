#include "settingdialog.h"
#include "ui_settingdialog.h"

#define SLIDER_RANGE 8

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
    ui->brightnessslider->setRange(-SLIDER_RANGE,SLIDER_RANGE);
    ui->contrastslider->setRange(-SLIDER_RANGE,SLIDER_RANGE);
    ui->hueslider->setRange(-SLIDER_RANGE,SLIDER_RANGE);
    ui->saturationslider->setRange(-SLIDER_RANGE,SLIDER_RANGE);
    ui->opacityslider->setRange(SLIDER_RANGE,SLIDER_RANGE*10);
    connect(ui->brightnessslider,SIGNAL(valueChanged(int)),this,SLOT(setBrightness(int)));
    connect(ui->contrastslider,SIGNAL(valueChanged(int)),this,SLOT(setContrast(int)));
    connect(ui->hueslider,SIGNAL(valueChanged(int)),this,SLOT(setHue(int)));
    connect(ui->saturationslider,SIGNAL(valueChanged(int)),this,SLOT(setSaturation(int)));
    connect(ui->aspectCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setAspect(int)));
    connect(ui->scalemodeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setScale(int)));
    connect(ui->opacityslider,SIGNAL(valueChanged(int)),this,SLOT(setOpacity(int)));
}

settingDialog::~settingDialog()
{
    delete ui;
}
void settingDialog::setVideoWidget(Phonon::VideoWidget *vwidget)
{
    videoWidget=vwidget;
}
void settingDialog::setAspect(int val)
{
    videoWidget->setScaleMode(Phonon::VideoWidget::ScaleMode(val));
}
void settingDialog::setBrightness(int val)
{
    videoWidget->setBrightness(val/qreal(SLIDER_RANGE));
}
void settingDialog::setContrast(int val)
{
    videoWidget->setContrast(val/qreal(SLIDER_RANGE));
}
void settingDialog::setHue(int val)
{
    videoWidget->setHue(val/qreal(SLIDER_RANGE));
}
void settingDialog::setSaturation(int val)
{
    videoWidget->setSaturation(val/qreal(SLIDER_RANGE));
}
void settingDialog::setScale(int val)
{
    videoWidget->setScaleMode(Phonon::VideoWidget::ScaleMode(val));
}
void settingDialog::updateSetting()
{
    Phonon::VideoWidget::AspectRatio oldAspect = videoWidget->aspectRatio();
    Phonon::VideoWidget::ScaleMode oldScale = videoWidget->scaleMode();
    ui->aspectCombo->setCurrentIndex(oldAspect);
    ui->scalemodeCombo->setCurrentIndex(oldScale);

    ui->brightnessslider->setValue(int(videoWidget->brightness()*SLIDER_RANGE));
    ui->contrastslider->setValue(int(videoWidget->contrast()*SLIDER_RANGE));
    ui->hueslider->setValue(int(videoWidget->hue()*SLIDER_RANGE));
    ui->saturationslider->setValue(int(videoWidget->saturation()*SLIDER_RANGE));
    ui->opacityslider->setValue(int(mainWindow->windowOpacity()*SLIDER_RANGE*10));
    //QMessageBox::warning(this,"- -",QString::number(mainWindow->windowOpacity()));
}
void settingDialog::setMainWindow(QWidget *window)
{
    mainWindow=window;
}
void settingDialog::setOpacity(int val)
{
    mainWindow->setWindowOpacity(val/qreal(SLIDER_RANGE*10));
}
