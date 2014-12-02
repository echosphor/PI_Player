#include "qctrlplayerevent.h"

QCtrlPlayerEvent::QCtrlPlayerEvent(): QEvent( Type(CTRL_EVENT) )
{
}
void QCtrlPlayerEvent::setType(CtrlType t)
{
    type=t;
}
CtrlType QCtrlPlayerEvent::getType()
{
    return type;
}
