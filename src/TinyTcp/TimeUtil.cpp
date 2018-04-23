#include "TimeUtil.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#define MILLISECONDS_TO_NANOSECONDS 1000000
#endif

/*
nanosecond   (ns) = 0.000000001 sec = 1e-9 sec
microseconds (μs) = 0.000001 sec    = 1e-6 sec
milliseconds (ms) = 0.001 sec       = 1e-3 sec
*/
void sleep_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    struct timespec req = { 0 };
    req.tv_sec = 0;
    req.tv_nsec = milliseconds * MILLISECONDS_TO_NANOSECONDS;
    nanosleep(&req, (struct timespec *)NULL);
#endif
}
