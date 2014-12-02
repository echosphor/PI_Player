#include "qseturlevent.h"

QSetUrlEvent::QSetUrlEvent() : QEvent( Type(SET_URL_EVENT) )
{
    url="";
}
