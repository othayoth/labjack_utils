#ifndef LOGHELPER
#define LOGHELPER


#include "labjack/LJM_Utilities.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

const char * identifier = "ANY";

struct LabJackState{
    bool is_connected=false;
    int handle;
    int err;
    double scan_rate = 1000;
    int scans_per_read = (int)(scan_rate/2);

    double state;

    enum{num_in_channels = 2};
    const char* in_channels[num_in_channels] = {"FIO0", "FIO1"};

    
    

};

struct LogState{
    std::shared_ptr<spdlog::logger> logger;
    std::string log_file;
    std::string log_name;
    bool is_logging=false;
};


void open_labjack(LabJackState* lj_state)       {
    lj_state->err = LJM_Open(LJM_dtT7, LJM_ctETHERNET, identifier, &lj_state->handle);
    if(lj_state->err!=LJME_NOERROR)   {
        lj_state->is_connected=false;
        printf("Failed to connect to LabJack\n");
        return;
    }
    else        {
        lj_state->is_connected=true;
        printf("Connected to LabJack\n");
    }
    
}

void close_labjack(LabJackState* lj_state)      {
    lj_state->err = LJM_Close(lj_state->handle);
    if(lj_state->err!=LJME_NOERROR)   {
        printf("Failed to close LabJack\n");
        return;
    }
    else        {
        printf("Closed LabJack\n");
        GetCurrentTimeMS();
    }
    
}

void receive_labjack_stream(LabJackState* lj_state)     {
    
    LJM_eReadName(lj_state->handle, "FIO0", &lj_state->state);
    
}

void lj_receiver()
{

}

void log_publisher()
{

}


#endif