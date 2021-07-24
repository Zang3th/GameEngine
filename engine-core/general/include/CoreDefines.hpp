#pragma once

#include <memory>

namespace Core
{
    //Window-Settings
    #define WINDOW_WIDTH 1920
    #define WINDOW_HEIGHT 1080

    //Smart-Pointer
    template<typename T>
        using Scope = std::unique_ptr<T>;

    template<typename T>
        using Ref = std::shared_ptr<T>;  
}