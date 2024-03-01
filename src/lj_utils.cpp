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
#include "imgui_internal.h"
#include "imgui.h"
#include <stdio.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <random>
#include "math.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

const float button_width = 400.0f;


// A nice way of printing out the system time
std::string CurrentTimeStr()
{
    time_t now = time(NULL);
    return std::string(ctime(&now));
}
#define CURRENT_TIME_STR CurrentTimeStr().c_str()


auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic_log.txt");

// Main code
int main(int argc, const char** argv)
{
    printf("started cbot\n");
    printf(CURRENT_TIME_STR);
    printf("\n");
    logger->info("starting cbot");

   

    u32 win_width = 1920;
    u32 win_height = 1080;
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

            ImGui::Begin("state machine");
            
            
           
            ImGui::End();
        }

        
        render_a_frame(window);
        logger->info("rendered a frame");
        logger->flush();
    }

    
    gx_cleanup(window);
    return 0;
}

