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
auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
};

struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};


static ScrollingBuffer sdata1;
static RollingBuffer   rdata1;
static float history = 10.0f;
long int t_start = 0;
                        

bool show_data = false;

LabJackState lj_state;
LabJackStreamData lj_data;
LabJackStreamData lj_local;

// Main code
int main(int argc, const char** argv)
{
    printf("started cbot\n");
    printf(CURRENT_TIME_STR);
    printf("\n");
    async_logger->info("starting cbot");

   

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

            ImGui::Begin("setup");

            if(ImGui::Button(lj_state.is_connected? "disconnect T7": "connect to T7"))
            {

                // int er= LJM_Open(LJM_dtT7, LJM_ctETHERNET, "ANY", &handle);
                // if(er!=LJME_NOERROR){
                //     printf("Failed to connect to LabJack\n");
                // }
                // else    {
                //     printf("Connected to LabJack\n");
                // }

                if (lj_state.is_connected)  {
                    async_logger->info("disconnected from labjack");
                    close_labjack(&lj_state);
                }
                else                {
                    printf("connecting to labjack\n");
                    async_logger->info("connected to labjack");
                    open_labjack(&lj_state);
                }

                
            }

            if(ImGui::Button(lj_state.stream_on? "stop stream": "start stream"))
            {
                if (lj_state.stream_on) {
                    stop_streaming(&lj_state,&lj_data);
                    lj_state.stream_on = false;
                    lj_state.lj_thread->join();
                    lj_state.log_thread->join();
                    async_logger->info("stopped streaming");

                }
                else {
                    lj_state.stream_on = true;
                    start_streaming(&lj_state,&lj_data);
                    lj_state.lj_thread = new std::thread(lj_stream_thread, &lj_state, &lj_data);
                    lj_state.log_thread = new std::thread(gui_log_thread, &lj_state, &lj_data, async_logger);

                }
            }
            
            if(!lj_state.stream_in_queue.empty())
                LabJackStreamData in_data = lj_state.stream_in_queue.pop();

            if(ImGui::Button( show_data?"stop polling":"poll data"))
            {
                if (show_data)  {
                    show_data = false;
                }
                else            {
                    sdata1.Erase();
                    rdata1.Data.clear();
             
                    show_data = true;
                    clock_gettime(CLOCK_REALTIME, &spec);
                    t_start = spec.tv_sec;
                    
                }
            }

  
            

            ImGui::End();
            

        }
        
        {
            ImGui::Begin("plot");

            
                if(show_data)
                {
                        
                   
                        // Read the digital state of FIO0
                        LJM_eReadName(handle, "FIO0", &state);

                        // Get the current time with millisecond precision
                        clock_gettime(CLOCK_REALTIME, &spec);
                        us = round(spec.tv_nsec / 1.0e3); // Convert nanoseconds to microseconds

                        // Print the state with timestamp
                        // printf("Time: %f, FIO0 State: %f\n", spec.tv_sec*1e-6+us, state);
                        // printf("Time: %ld.%06ld, FIO0 State: %f\n", (spec.tv_sec - t_start), us, state);

                        // Sleep for a bit before the next poll
                        // LJM_Sleep(1000); // Sleep for 1000 milliseconds (1 second)


                        sdata1.AddPoint((spec.tv_sec-t_start)+us*1e-6, state);
                        rdata1.AddPoint((spec.tv_sec-t_start)+us*1e-6, state);
                        
                        ImGui::SliderFloat("History",&history,1,30,"%.1f s");
                        rdata1.Span = history;
                        
                        static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

                        if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150))) {
                            ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
                            ImPlot::SetupAxisLimits(ImAxis_X1,(spec.tv_sec-t_start)+us*1e-6 - history, (spec.tv_sec-t_start)+us*1e-6, ImGuiCond_Always);
                            ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
                            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
                            ImPlot::PlotShaded("Pulse", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, 0, sdata1.Offset, 2 * sizeof(float));
                            ImPlot::EndPlot();
                        }

                        if (ImPlot::BeginPlot("##Rolling", ImVec2(-1,150))) {
                            ImPlot::SetupAxes(NULL, NULL, flags, flags);
                            ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
                            ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
                            ImPlot::PlotLine("Pulse", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
                            ImPlot::EndPlot();
                        }


                    


                }
           


            
            
            ImGui::End();

        }


        render_a_frame(window);
        async_logger->flush();
    }

    
    gx_cleanup(window);
    return 0;
}

