#ifndef KEY_ENUM_H
#define KEY_ENUM_H

namespace input
{
    enum class InputType
    {
        EVENT,
        POLLED
    };

    enum class TriggerType
    {
        SINGLE_PRESS,
        CONTINUOUS
    };

    struct KeyStruct {
        std::function<void()> keyBindFunc;
        int key;//GLFW key
        InputType inputType;//Event or polled input
        TriggerType triggerType;//Press-release, or continuously fire
        bool isSinglePressReady = true;
    };
}

#endif