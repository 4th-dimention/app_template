
function B32
W32_TimerInit(W32_Timer *timer)
{
    B32 result = 0;
    
    if(QueryPerformanceFrequency(&timer->counts_per_second))
    {
        result = 1;
    }
    
    timer->sleep_is_granular = (timeBeginPeriod(1) == TIMERR_NOERROR);
    
    return result;
}

function void
W32_TimerBeginFrame(W32_Timer *timer)
{
    QueryPerformanceCounter(&timer->begin_frame);
}

function void
W32_TimerEndFrame(W32_Timer *timer, F64 milliseconds_per_frame)
{
    LARGE_INTEGER end_frame;
    QueryPerformanceCounter(&end_frame);
    
    F64 desired_seconds_per_frame = (milliseconds_per_frame / 1000.0);
    S64 elapsed_counts = end_frame.QuadPart - timer->begin_frame.QuadPart;
    S64 desired_counts = (S64)(desired_seconds_per_frame * timer->counts_per_second.QuadPart);
    S64 counts_to_wait = desired_counts - elapsed_counts;
    
    LARGE_INTEGER start_wait;
    LARGE_INTEGER end_wait;
    
    QueryPerformanceCounter(&start_wait);
    
    while(counts_to_wait > 0)
    {
        if(timer->sleep_is_granular)
        {
            DWORD milliseconds_to_sleep = (DWORD)(1000.0 * ((F64)(counts_to_wait) / timer->counts_per_second.QuadPart));
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
