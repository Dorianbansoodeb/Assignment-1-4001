/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int now_ms = 0;               // current time in ms

    int KERNEL_MODE = 1; //time to switch to kernel mode
    int CTX_RSTR = 10; //time to save context/restore
    int ISR_COST = 40; //time to execute ISR

    int DEVICE_COUNT = 32; //number of devices
    int device_due_at[DEVICE_COUNT]; //array to track when each device will complete its I/O
    for (int i = 0; i < DEVICE_COUNT; i++) {
        device_due_at[i] = -1; //initialize all devices to not busy
    }
    



    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/



        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
