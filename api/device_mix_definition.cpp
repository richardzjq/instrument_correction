#include"device_mix_definition.h"

void qt_sleep(int time_msecond)
{
    QTime t;
    t.start();
    while(t.elapsed() < time_msecond)
        QCoreApplication::processEvents();
}
