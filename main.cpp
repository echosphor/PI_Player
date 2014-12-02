#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB18030"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    a.setQuitOnLastWindowClosed(true);
    MainWindow w;
    w.show();
    if(argc==2)
    {
        w.getPlayer()->setFilePath(QDir::fromNativeSeparators(argv[1]));//路径转换"\\"->"/"
        w.getPlayer()->play();
    }

    return a.exec();
}
