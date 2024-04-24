#pragma once

#include "Engine.hpp"
#include "LiquefiedInterface.hpp"

namespace Liq
{
    class LiquefiedApp final : public Engine::App
    {
        private:
            double                                _timeElapsed   = 0.0;
            Engine::PixelRenderer*                _pixelRenderer = nullptr;
            Engine::Scope<LiquefiedInterface>     _interface;
            Engine::Scope<Engine::FluidSimulator> _fluidSimulator;

            void           LoadResources() override;
            Engine::uint32 InitModules()   override;
            void           VisualizeSmoke() const;

        public:
            LiquefiedApp();
            ~LiquefiedApp() override;
            void Update()   override;
    };
}