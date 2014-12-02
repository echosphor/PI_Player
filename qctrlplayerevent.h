#ifndef QCTRLPLAYEREVENT_H
#define QCTRLPLAYEREVENT_H

#include <QEvent>
#include <QString>

#define CTRL_EVENT QEvent::User+100
enum CtrlType {FULLSCREEN=0,PAUSE,VOLUME_UP,VOLUME_DOWN,GO_FORWARD,GO_BACKWARD};

class QCtrlPlayerEvent : public QEvent
{
public:
    QCtrlPlayerEvent();
    void setType(CtrlType t);
    CtrlType getType();
private:
    CtrlType type;
};

#endif // QCTRLPLAYEREVENT_H
