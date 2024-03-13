// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#define ENET_IMPLEMENTATION
#include "gx_helper.h"
#include "FrameBuffer.h"
#include "IconsForkAwesome.h"
#include "utility.h"
#include <chrono>
#include <string.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"

#include <stdio.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <random>
#include "math.h"

#include "log_helper.h"

int handle, err;
double state;
struct timespec spec;
long int us; // Microseconds

const float button_width = 400.0f;


// A nice way of printing out the system time
std::string CurrentTimeStr()
{
    time_t now = time(NULL);
    return std::string(ctime(&now));
}
#define CURRENT_TIME_STR CurrentTimeStr().c_str()


// auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic_log.txt");
// spdlog::set_pattern("[%H_%M_%S_%z] [%n] [%^---%L---%$] [thread %t] %v");
// spdlog::set_pattern("[%Y_%m_%d_%H_%M_%S_%f_%z] [%n] [%^---%L---%$] [thread %t] %v");
// spdlog::init_thread_pool(8192, 1);
auto camera_logger = spdlog::basic_logger_mt<spdlog::async_factory>("camera_logger", "logs/async_log.txt");
auto cbot_logger = spdlog::basic_logger_mt<spdlog::async_factory>("cbot_logger", "logs/async_log.txt");

                        

bool show_data = false;

LabJackState lj_state;
Pulse pulse;
float duty_cycle = pulse.dutyCycle;
float fps = pulse.frequency;

// Main code
int main(int argc, const char** argv)
{
    printf("started cbot\n");
    printf(CURRENT_TIME_STR);
    printf("\n");
    cbot_logger->info("starting cbot");

   

    u32 win_width = 640;
    u32 win_height = 320;
    // window 
    gx_context *window = (gx_context *)malloc(sizeof(gx_context));
    *window = (gx_context){
        .swap_interval = 1, // use vsync
        .width = win_width,
        .height = win_height,
        .render_target_title = (char *)malloc(100), // window title
        .glsl_version = (char *)malloc(100)};
    render_initialize_target(window);

    FrameBuffer sceneBuffer(win_width, win_height);
    sceneBuffer.Bind();
    
    sceneBuffer.Unbind();
    
    
    
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    f32 last_time = tick();
    f32 current_time = tick();

    while (!glfwWindowShouldClose(window->render_target))
    {

        //networking 
        current_time = tick();
        

        sceneBuffer.Bind();
        sceneBuffer.Unbind();
        create_new_frame();
        
      
        // dock:fsm
        {

            ImGui::Begin("setup");

            if(ImGui::Button(lj_state.is_connected? "disconnect T7": "connect to T7"))
            {
               
                if (lj_state.is_connected)  {
                    cbot_logger->info("disconnected from labjack");
                    close_labjack(&lj_state);
                }
                else                {
                    printf("connecting to labjack\n");
                    cbot_logger->info("connected to labjack");
                    open_labjack(&lj_state);
                }

                
            }

            if(lj_state.is_connected) {
                if(ImGui::Button(lj_state.pulse_on? "stop pulse": "start pulse"))
                {
                    if (lj_state.pulse_on) {
                        lj_state.pulse_on = false;
                        stop_pulsing(&lj_state);                    
                        

                    }
                    else {
                        lj_state.pulse_on = true;
                        start_pulsing(&lj_state, &pulse,cbot_logger);                    
                    }
                }

                ImGui::SliderFloat("duty cycle", &duty_cycle, 0.0f, 100.0f, "%.3f");
                ImGui::SliderFloat("fps", &fps, 1.0f, 100.0f, "%.3f");
                
                update_pulse(&pulse, fps, duty_cycle);
            }    
            
            ImGui::End();
            

        }
        
       

        render_a_frame(window);
        cbot_logger->flush();
        // camera_logger->flush();
    }

    
    gx_cleanup(window);
    return 0;
}

