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
#include <ctime>
#include <string.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"

#include <stdio.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <random>
#include "math.h"

#include <typeinfo>
#include "log_helper.h"

int handle, err;
double state;
struct timespec spec;
long int us; // Microseconds

const float button_width = 400.0f;


// A nice way of printing out the formatted system time
char* getCurrentTimeFormatted() {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    static char formatted_time[40]; // Buffer to hold the formatted time
    strftime(formatted_time, sizeof(formatted_time), "logs/cbot_log_%Y_%m_%d_%H_%M_%S.txt", tm_now);

    return formatted_time;
}


std::string log_file = std::string(getCurrentTimeFormatted());
auto camera_logger = spdlog::basic_logger_mt<spdlog::async_factory>("camera_logger", log_file);
auto cbot_logger = spdlog::basic_logger_mt<spdlog::async_factory>("cbot_gui", log_file);




static float history = 10.0f;
long int t_start = 0;
                        

bool show_data = false;

LabJackState lj_state;
LabJackStreamData lj_data;
LabJackStreamData lj_local;

// Main code
int main(int argc, const char** argv)
{
    // std::cout<<"type of camera_logger: "<<typeid(camera_logger).name()<<std::endl;
    // See this link for custom formatting: 
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    spdlog::set_pattern("%Y_%m_%d_%H_%M_%S_%e,  %n, %l, %v");

    printf("started cbot\n");
    printf(getCurrentTimeFormatted());
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

            if(ImGui::Button(lj_state.stream_on? "stop stream": "start stream"))
            {
                if (lj_state.stream_on) {
                    lj_state.stream_on = false;
                    lj_state.log_on = false;
                    stop_streaming(&lj_state,&lj_data);                    
                    

                }
                else {
                    lj_state.stream_on = true;
                    start_streaming(&lj_state,&lj_data,camera_logger);                    
                }
            }
            
            if(!lj_state.stream_in_queue.empty())
                LabJackStreamData in_data = lj_state.stream_in_queue.pop();

          

  
            

            ImGui::End();
            

        }
        
       

        render_a_frame(window);
        cbot_logger->flush();
        // camera_logger->flush();
    }

    
    gx_cleanup(window);
    return 0;
}

