#pragma once

#include "Window.hpp"
#include "GlobalParams.hpp"

namespace Engine
{
    #define GRAVITY PhysicsParams::GRAVITY
    #define ITER LiquiefiedParams::GAUSS_SEIDEL_ITERATIONS
    #define OVERRELAX LiquiefiedParams::GAUSS_SEIDEL_OVERRELAXATION

    struct StaggeredGrid
    {
        private:
            //Horizontal u-component is sampled at the centers of the vertical cell faces.
            float _u[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //Vertical is sampled at the centers of the horizontal cell faces.
            float _v[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //s-comp set to 0 for border cells.
            float _s[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

            //For visualization of the flow.
            float _smoke[LiquiefiedParams::LIQUID_NUM_CELLS] = {0.0f};

        public:
            const uint32 width = LiquiefiedParams::SIMULATION_WIDTH;
            const uint32 height = LiquiefiedParams::SIMULATION_HEIGHT;

            inline float& u_At(uint32 x, uint32 y) { return _u[x * height + y]; }
            inline float& v_At(uint32 x, uint32 y) { return _u[x * height + y]; }
            inline float& s_At(uint32 x, uint32 y) { return _u[x * height + y]; }
            inline float& smoke_At(uint32 x, uint32 y) { return _u[x * height + y]; }
    };

    class FluidSimulator
    {
        private:
            StaggeredGrid _grid;

            void AddForces(float dt);
            void Project(float dt);
            void AdvectVelocity(float dt);
            void AdvectSmoke(float dt);
            void Init();

        public:
            FluidSimulator();
            ~FluidSimulator() = default;
            void TimeStep();
            StaggeredGrid* GetGrid();
    };
}