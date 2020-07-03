
#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dw_user_index, XINPUT_STATE *p_state)
typedef XINPUT_GET_STATE(XInputGetStateProc);
XINPUT_GET_STATE(XInputGetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
global XInputGetStateProc *XInputGetStateProcPtr = XInputGetStateStub;

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dw_user_index, XINPUT_VIBRATION *p_vibration)
typedef XINPUT_SET_STATE(XInputSetStateProc);
XINPUT_SET_STATE(XInputSetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
global XInputSetStateProc *XInputSetStateProcPtr = XInputSetStateStub;

internal void
W32_LoadXInput(void)
{
    XInputSetStateProcPtr = XInputSetStateStub;
    XInputGetStateProcPtr = XInputGetStateStub;
    
    HMODULE x_input_lib = LoadLibraryA("xinput1_4.dll");
    if(!x_input_lib)
    {
        x_input_lib = LoadLibraryA("xinput9_1_0.dll");
    }
    if(!x_input_lib)
    {
        x_input_lib = LoadLibraryA("xinput1_3.dll");
    }
    if(x_input_lib)
    {
        XInputGetStateProcPtr = (XInputGetStateProc *)GetProcAddress(x_input_lib, "XInputGetState");
        XInputSetStateProcPtr = (XInputSetStateProc *)GetProcAddress(x_input_lib, "XInputSetState");
    }
}

internal void
W32_UpdateXInput(void)
{
    for(u32 i = 0; i < W32_MAX_GAMEPADS; ++i)
    {
        if(i < XUSER_MAX_COUNT)
        {
            XINPUT_STATE controller_state = {0};
            if(XInputGetStateProcPtr(0, &controller_state) == ERROR_SUCCESS)
            {
                // NOTE(rjf): Controller connected.
                global_gamepads[i].connected = 1;
                XINPUT_GAMEPAD *pad = &controller_state.Gamepad;
                
                for(u32 j = 0; j < W32_MAX_GAMEPADS; ++j)
                {
                    global_gamepads[i].button_states[j] = 0;
                }
                
                global_gamepads[i].button_states[GamepadButton_DPadUp] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                global_gamepads[i].button_states[GamepadButton_DPadDown] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                global_gamepads[i].button_states[GamepadButton_DPadLeft] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                global_gamepads[i].button_states[GamepadButton_DPadRight] |= !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                global_gamepads[i].button_states[GamepadButton_Start] |= !!(pad->wButtons & XINPUT_GAMEPAD_START);
                global_gamepads[i].button_states[GamepadButton_Back] |= !!(pad->wButtons & XINPUT_GAMEPAD_BACK);
                global_gamepads[i].button_states[GamepadButton_LeftThumb] |= !!(pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
                global_gamepads[i].button_states[GamepadButton_RightThumb] |= !!(pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
                global_gamepads[i].button_states[GamepadButton_LeftShoulder] |= !!(pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                global_gamepads[i].button_states[GamepadButton_RightShoulder] |= !!(pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                global_gamepads[i].button_states[GamepadButton_A] |= !!(pad->wButtons & XINPUT_GAMEPAD_A);
                global_gamepads[i].button_states[GamepadButton_B] |= !!(pad->wButtons & XINPUT_GAMEPAD_B);
                global_gamepads[i].button_states[GamepadButton_X] |= !!(pad->wButtons & XINPUT_GAMEPAD_X);
                global_gamepads[i].button_states[GamepadButton_Y] |= !!(pad->wButtons & XINPUT_GAMEPAD_Y);
                
                global_gamepads[i].joystick_1.x = pad->sThumbLX / 32768.f;
                global_gamepads[i].joystick_1.y = pad->sThumbLY / 32768.f;
                global_gamepads[i].joystick_2.x = pad->sThumbRX / 32768.f;
                global_gamepads[i].joystick_2.y = pad->sThumbRY / 32768.f;
            }
            else
            {
                // NOTE(rjf): Controller is not connected
                global_gamepads[i].connected = 0;
            }
        }
    }
}
