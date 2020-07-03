
typedef struct W32_Timer
{
    LARGE_INTEGER counts_per_second;
    LARGE_INTEGER begin_frame;
    b32 sleep_is_granular;
}
W32_Timer;

internal b32 W32_TimerInit(W32_Timer *timer);
internal void W32_TimerBeginFrame(W32_Timer *timer);
internal void W32_TimerEndFrame(W32_Timer *timer, f64 milliseconds_per_frame);