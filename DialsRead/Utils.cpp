#include "Utils.h"

#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

void Utils::sleepms(int count)
{
#ifdef LINUX
    usleep(count * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef WINDOWS
    Sleep(count);
#endif
}