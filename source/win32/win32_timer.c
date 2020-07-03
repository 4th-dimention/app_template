
internal b32
W32_TimerInit(W32_Timer *timer)
{
    b32 result = 0;
    
    if(QueryPerformanceFrequency(&timer->counts_per_second))
    {
        result = 1;
    }
    
    timer->sleep_is_granular = (timeBeginPeriod(1) == TIMERR_NOERROR);
    
    return result;
}

internal void
W32_TimerBeginFrame(W32_Timer *timer)
{
    QueryPerformanceCounter(&timer->begin_frame);
}

internal void
W32_TimerEndFrame(W32_Timer *timer, f64 milliseconds_per_frame)
{
    LARGE_INTEGER end_frame;
    QueryPerformanceCounter(&end_frame);
    
    f64 desired_seconds_per_frame = (milliseconds_per_frame / 1000.0);
    i64 elapsed_counts = end_frame.QuadPart - timer->begin_frame.QuadPart;
    i64 desired_counts = (i64)(desired_seconds_per_frame * timer->counts_per_second.QuadPart);
    i64 counts_to_wait = desired_counts - elapsed_counts;
    
    LARGE_INTEGER start_wait;
    LARGE_INTEGER end_wait;
    
    QueryPerformanceCounter(&start_wait);
    
    while(counts_to_wait > 0)
    {
        if(timer->sleep_is_granular)
        {
            DWORD milliseconds_to_sleep = (DWORD)(1000.0 * ((f64)(counts_to_wait) / timer->counts_per_second.QuadPart));
            if(milliseconds_to_sleep > 0)
            {
                Sleep(milliseconds_to_sleep);
            }
        }
        
        QueryPerformanceCounter(&end_wait);
        counts_to_wait -= end_wait.QuadPart - start_wait.QuadPart;
        start_wait = end_wait;
    }
}