
static const GUID IID_IAudioClient = {0x1CB9AD4C, 0xDBFA, 0x4c32, 0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2};
static const GUID IID_IAudioRenderClient = {0xF294ACFC, 0x3146, 0x4483, 0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2};
static const GUID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E};
static const GUID IID_IMMDeviceEnumerator = {0xA95664D2, 0x9614, 0x4F35, 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6};
static const GUID PcmSubformatGuid = {STATIC_KSDATAFORMAT_SUBTYPE_PCM};

#ifndef AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM
#define AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM 0x80000000
#endif

#ifndef AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY
#define AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY 0x08000000
#endif

#define SOUND_LATENCY_FPS 10
#define REFTIMES_PER_SEC 10000000

#define CO_CREATE_INSTANCE(name) HRESULT name(REFCLSID rclsid, LPUNKNOWN *pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv)
typedef CO_CREATE_INSTANCE(CoCreateInstance_);
CO_CREATE_INSTANCE(CoCreateInstanceStub)
{
    return 1;
}
global CoCreateInstance_ *CoCreateInstanceProc = CoCreateInstanceStub;

#define CO_INITIALIZE_EX(name) HRESULT name(LPVOID pvReserved, DWORD dwCoInit)
typedef CO_INITIALIZE_EX(CoInitializeEx_);
CO_INITIALIZE_EX(CoInitializeExStub)
{
    return 1;
}
global CoInitializeEx_ *CoInitializeExProc = CoInitializeExStub;

typedef struct W32_SoundOutput W32_SoundOutput;
struct W32_SoundOutput
{
    B32 initialized;
    
    IMMDeviceEnumerator *device_enum;
    IMMDevice *device;
    IAudioClient *audio_client;
    IAudioRenderClient *audio_render_client;
    REFERENCE_TIME sound_buffer_duration;
    U32 buffer_frame_count;
    U32 channels;
    U32 samples_per_second;
    U32 latency_frame_count;
};

function void
W32_LoadWASAPI(void)
{
    HMODULE wasapi_lib = LoadLibraryA("ole32.dll");
    if(wasapi_lib)
    {
        CoCreateInstanceProc = (CoCreateInstance_ *)GetProcAddress(wasapi_lib, "CoCreateInstance");
        CoInitializeExProc = (CoInitializeEx_ *)GetProcAddress(wasapi_lib, "CoInitializeEx");
    }
    else
    {
        CoCreateInstanceProc = CoCreateInstanceStub;
        CoInitializeExProc = CoInitializeExStub;
    }
}

function void
W32_InitWASAPI(W32_SoundOutput *output)
{
    CoInitializeExProc(0, COINIT_SPEED_OVER_MEMORY);
    
    REFERENCE_TIME requested_sound_duration = REFTIMES_PER_SEC * 2;
    
    HRESULT result;
    
    result = CoCreateInstanceProc(&CLSID_MMDeviceEnumerator,
                                  0,
                                  CLSCTX_ALL,
                                  &IID_IMMDeviceEnumerator,
                                  (LPVOID *)(&output->device_enum));
    if(result == S_OK)
    {
        
        output->device_enum->lpVtbl->GetDefaultAudioEndpoint(output->device_enum,
                                                             eRender,
                                                             eConsole,
                                                             &output->device);
        if(result == S_OK)
        {
            
            result = output->device->lpVtbl->Activate(output->device,
                                                      &IID_IAudioClient,
                                                      CLSCTX_ALL,
                                                      0,
                                                      (void **)&output->audio_client);
            if(result == S_OK)
            {
                WAVEFORMATEX *wave_format = 0;
                
                output->audio_client->lpVtbl->GetMixFormat(output->audio_client, &wave_format);
                
                output->samples_per_second = 44100;//wave_format->nSamplesPerSec;
                WORD bits_per_sample = sizeof(S16)*8;
                WORD block_align = (output->channels * bits_per_sample) / 8;
                DWORD average_bytes_per_second = block_align * output->samples_per_second;
                WORD cb_size = 0;
                
                WAVEFORMATEX new_wave_format = {
                    WAVE_FORMAT_PCM,
                    (WORD)output->channels,
                    output->samples_per_second,
                    average_bytes_per_second,
                    block_align,
                    bits_per_sample,
                    cb_size,
                };
                
                result = output->audio_client->lpVtbl->Initialize(output->audio_client,
                                                                  AUDCLNT_SHAREMODE_SHARED,
                                                                  AUDCLNT_STREAMFLAGS_RATEADJUST | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
                                                                  requested_sound_duration,
                                                                  0,
                                                                  &new_wave_format,
                                                                  0);
                
                output->latency_frame_count = output->samples_per_second / SOUND_LATENCY_FPS;
                
                if(result == S_OK)
                {
                    
                    result = output->audio_client->lpVtbl->GetService(output->audio_client,
                                                                      &IID_IAudioRenderClient,
                                                                      (void **)&output->audio_render_client);
                    
                    if(result == S_OK)
                    {
                        // NOTE(Ryan): Audio initialization was successful
                        
                        output->audio_client->lpVtbl->GetBufferSize(output->audio_client, &output->buffer_frame_count);
                        
                        output->sound_buffer_duration = (REFERENCE_TIME)((F64)REFTIMES_PER_SEC *
                                                                         output->buffer_frame_count / output->samples_per_second);
                        
                        output->audio_client->lpVtbl->Start(output->audio_client);
                        
                        output->initialized = 1;
                    }
                    else
                    {
                        OS_OutputError("WASAPI Error", "Request for audio render service failed.");
                    }
                }
                else
                {
                    OS_OutputError("WASAPI Error",
                                   "Audio client initialization failed.");
                }
            }
            else
            {
                OS_OutputError("WASAPI Error", "Could not activate audio device.");
            }
        }
        else
        {
            OS_OutputError("WASAPI Error", "Default audio endpoint was not found.");
        }
    }
    else
    {
        OS_OutputError("WASAPI Error", "Device enumerator retrieval failed.");
    }
}

function void
W32_CleanUpWASAPI(W32_SoundOutput *output)
{
    if(output->initialized)
    {
        output->audio_client->lpVtbl->Stop(output->audio_client);
        
        output->device_enum->lpVtbl->Release(output->device_enum);
        output->device->lpVtbl->Release(output->device);
        output->audio_client->lpVtbl->Release(output->audio_client);
        output->audio_render_client->lpVtbl->Release(output->audio_render_client);
    }
}

function void
W32_FillSoundBuffer(U32 samples_to_write, F32 *samples, W32_SoundOutput *output)
{
    if(samples_to_write)
    {
        BYTE *data = 0;
        DWORD flags = 0;
        
        output->audio_render_client->lpVtbl->GetBuffer(output->audio_render_client, samples_to_write, &data);
        if(data)
        {
            S16 *destination = (S16 *)data;
            F32 *source = samples;
            for(UINT32 i = 0; i < samples_to_write; ++i)
            {
                S16 left = (S16)(*source++ * 3000);
                S16 right = (S16)(*source++ * 3000);
                *destination++ = left;
                *destination++ = right;
            }
        }
        output->audio_render_client->lpVtbl->ReleaseBuffer(output->audio_render_client, samples_to_write, flags);
    }
}
