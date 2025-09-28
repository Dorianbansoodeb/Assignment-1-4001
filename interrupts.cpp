/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

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

    int KERNEL_MODE = 1; //time to switch between to and from kernel mode
    int CTX_RSTR = 10; //time to save context/restore
    int ISR_COST = 40; //time to execute ISR
    int VECTOR_LOOKUP = 1; //time to lookup vector address
    int ISR_ADDR_LOOKUP = 1; //time to lookup ISR address

    const int DEVICE_COUNT = 32; //number of devices
    int device_due_at[DEVICE_COUNT]; //array to track when each device will complete its I/O
    for (int i = 0; i < DEVICE_COUNT; i++) {
        device_due_at[i] = -1; //initialize all devices to not busy
    }




    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU"){
            execution+= std::to_string(now_ms) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            now_ms += duration_intr;
        }
        else if (activity == "SYSCALL"){
            execution += std::to_string(now_ms) + ", "+std::to_string(KERNEL_MODE)+", switch to kernel mode\n";
            now_ms += KERNEL_MODE;

            execution += std::to_string(now_ms) + ", " + std::to_string(CTX_RSTR) + ", context saved\n";
            now_ms += CTX_RSTR;

            int mem_pos = ADDR_BASE + duration_intr * VECTOR_SIZE;
            execution += std::to_string(now_ms) + ", " + std::to_string(VECTOR_LOOKUP) + ", find vector " + std::to_string(duration_intr) + " in memory position " + std::to_string(mem_pos) + "\n";
            now_ms += VECTOR_LOOKUP;

            execution += std::to_string(now_ms) + ", " + std::to_string(ISR_ADDR_LOOKUP) + ", obtain ISR address (" + vectors.at(duration_intr) + ")\n";
            now_ms += ISR_ADDR_LOOKUP;

            execution += std::to_string(now_ms) + ", " + std::to_string(ISR_COST) + ", call device driver\n";
            now_ms += ISR_COST;

            device_due_at[duration_intr] = now_ms + delays.at(duration_intr);

            execution += std::to_string(now_ms) + ", " + std::to_string(KERNEL_MODE) + ", IRET\n";
            now_ms += KERNEL_MODE;
        }
        else if (activity == "END_IO"){
            if(duration_intr >=0 && duration_intr < DEVICE_COUNT && device_due_at[duration_intr]>=0 && now_ms<device_due_at[duration_intr]){
                now_ms = device_due_at[duration_intr];
            }

            if (duration_intr>=0 && duration_intr < DEVICE_COUNT){
                device_due_at[duration_intr] = -1; //reset device to not busy
            }
            execution += std::to_string(now_ms) + ", " + std::to_string(KERNEL_MODE) + ", switch to kernel mode\n";
            now_ms += KERNEL_MODE;

            execution += std::to_string(now_ms) +", " + std::to_string(CTX_RSTR) + ", context saved\n";
            now_ms += CTX_RSTR;

            int mem_pos = ADDR_BASE + duration_intr * VECTOR_SIZE;
            execution += std::to_string(now_ms) + ", " + std::to_string(VECTOR_LOOKUP) + ", find vector " + std::to_string(duration_intr) + " in memory position " + std::to_string(mem_pos) + "\n";
            now_ms += VECTOR_LOOKUP;

            execution += std::to_string(now_ms) + ", " + std::to_string(ISR_ADDR_LOOKUP) + ", obtain ISR address (" + vectors.at(duration_intr) + ")\n";
            now_ms += ISR_ADDR_LOOKUP;

            execution += std::to_string(now_ms) + ", " + std::to_string(ISR_COST) + ", end of I/O " + std::to_string(duration_intr) + ": interrupt service\n";
            now_ms += ISR_COST;

            execution += std::to_string(now_ms) + ", " + std::to_string(KERNEL_MODE) + ", IRET\n";
            now_ms += KERNEL_MODE;

        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
