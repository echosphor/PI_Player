#ifndef QSETURLEVENT_H
#define QSETURLEVENT_H

#include <QEvent>
#include <QString>
#define SET_URL_EVENT QEvent::User+1

class QSetUrlEvent : public QEvent
{
public:
    QSetUrlEvent();
public:
    QString url;
};

#endif // QSETURLEVENT_H
