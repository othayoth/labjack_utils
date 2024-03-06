#ifndef LOGHELPER
#define LOGHELPER


#include "labjack/LJM_Utilities.h"
#include "labjack/LJM_StreamUtilities.h"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"


const char * identifier = "ANY";
bool clean_exit = false;

// using namespace spdlog;

std::thread* lj_thread;
std::thread* log_thread;

// data structure to stream data
struct LabJackStreamData{
    int num_skipped_scans=0;
    int total_skipped_scans=0;
    int device_scan_backlog=0;
    int lj_scan_backlog=0;
    uint64_t us_tick=0;
    uint64_t us_scan_start=0;
    uint64_t counter=0;
    double* data;    
};

struct LabJackState{
    // states
    bool is_connected=false;
    bool stream_on=false;
    bool log_on = true;
    int handle;
    int err;

    std::thread* lj_thread;
    std::thread* log_thread;
    
    // scan pararmeters
    double scan_rate = 10000;
    int scans_per_read = (int)(scan_rate/2);
    enum {num_channels = 1};
    const char * channels[num_channels] = {"FIO0"};    
    int* scan_list_addresses;
    unsigned int scan_data_size = num_channels*scans_per_read;       
    TSQueue<LabJackStreamData> stream_in_queue;
    
};



void lj_stream_thread(LabJackState* lj_state, LabJackStreamData* lj_stream);
void gui_log_thread(LabJackState* lj_state, LabJackStreamData *lj_stream, std::shared_ptr<spdlog::logger> logger);



void open_labjack(LabJackState* lj_state)       {
    lj_state->err = LJM_Open(LJM_dtT7, LJM_ctETHERNET, identifier, &lj_state->handle);
    if(lj_state->err!=LJME_NOERROR)   {
        lj_state->is_connected=false;
        printf("Failed to connect to LabJack\n");
        return;
    }
    else        {
        lj_state->is_connected=true;
        printf("Opened connection to LabJack\n");        
    }
    
}

void close_labjack(LabJackState* lj_state)      {
    lj_state->err = LJM_Close(lj_state->handle);
    if(lj_state->err!=LJME_NOERROR)   {
        printf("Failed to close LabJack\n");
        return;
    }
    else        {
        lj_state->is_connected=false;
        printf("Closed connecttion to LabJack\n");
        GetCurrentTimeMS();
        LJM_GetHostTick();
    }
    
}

void start_streaming(LabJackState* lj_state, LabJackStreamData* lj_stream, std::shared_ptr<spdlog::logger> camera_logger)    {

    // get channel list and convert them to addresses
    lj_state->scan_list_addresses = (int*)malloc(lj_state->num_channels*sizeof(int));
    int err = LJM_NamesToAddresses(lj_state->num_channels, lj_state->channels, lj_state->scan_list_addresses, NULL);

    // allocate memory for scan data
    lj_stream->data = (double*)malloc(lj_state->scan_data_size*sizeof(double));
    lj_state->err = LJM_eStreamStart(lj_state->handle, lj_state->scans_per_read, lj_state->num_channels, lj_state->scan_list_addresses, &lj_state->scan_rate);
    if(lj_state->err!=LJME_NOERROR)   {
        printf("Failed to start streaming\n");
        lj_state->stream_on = false;
        return;
    }
    else        {
        lj_state->stream_on=true;
        lj_thread = new std::thread(lj_stream_thread, lj_state, lj_stream);
        log_thread = new std::thread(gui_log_thread, lj_state, lj_stream, camera_logger);
        printf("Started streaming\n");
        clean_exit = false;
    }
}

void stop_streaming(LabJackState* lj_state, LabJackStreamData* lj_stream)     {
    
    lj_state->stream_on = false;

    while(clean_exit==false){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    clean_exit = false;
    lj_thread->join();
    log_thread->join();

    lj_state->err = LJM_eStreamStop(lj_state->handle);
    if(lj_state->err!=LJME_NOERROR)   {
        printf("Failed to stop streaming\n");
        return;
    }
    else        {       
        lj_state->stream_on=false;
        printf("Stopped streaming\n");
    }

    
}

void lj_stream_thread(LabJackState* lj_state, LabJackStreamData* lj_stream)
{
    printf("starting lj_stream_thread\n");
    lj_stream->us_scan_start = LJM_GetHostTick();
    while(lj_state->stream_on)
    {
        lj_stream->us_tick=LJM_GetHostTick() - lj_stream->us_scan_start;
        lj_state->err = LJM_eStreamRead(lj_state->handle, lj_stream->data, &lj_stream->device_scan_backlog, &lj_stream->lj_scan_backlog);
        lj_state->stream_in_queue.push(*lj_stream);
    }


    free(lj_state->scan_list_addresses);
    free(lj_stream->data);

    clean_exit = true;

    
}

void gui_log_thread(LabJackState* lj_state, LabJackStreamData *lj_stream, std::shared_ptr<spdlog::logger> logger)
{

    printf("starting gui_log_thread\n");
    double last_data = 0;
    while (lj_state->log_on) {
        if (!lj_state->stream_in_queue.empty()) {
        
            LabJackStreamData in_data = lj_state->stream_in_queue.pop();
        
            for (int i = 0; i < lj_state->scan_data_size; i++) {
                if (in_data.data[i] != last_data && in_data.data[i] != 0) {
                    lj_stream->counter++;                
                    logger->info("tick: {0}, data: {1}, pulse_count: {2}", in_data.us_tick, in_data.data[i],lj_stream->counter);
                    
                }
            last_data = in_data.data[i];
            }
        }
        logger->flush();
    }
}

#endif