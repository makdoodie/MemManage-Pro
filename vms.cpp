#include "policies.h"
#include "dataStructures.h"

using namespace std;

void vms(char* tracefile, int num_frames, char* execution) {
    vector <memorysystem> vms_pages; // Vector to store pages in segmented FIFO order
    memorysystem single_read; // Struct to store information of a single read from the tracefile
    int read_count = 0; // Counter for the number of reads (page faults)
    int write_count = 0; // Counter for the number of writes (ejections/replacements of page addresses)
    int event_count = 0; // Counter for the total number of traces read from the tracefile

    FILE* read_file = fopen(tracefile, "r"); // Open the tracefile in "r" mode

    while (fscanf(read_file, "%x %c", &single_read.address, &single_read.function) != EOF) {
        single_read.address /= 4096; // Divide the address obtained by 4096 due to the difference in page size
        event_count++; // Increment the event count for each new trace read

        bool trace_exists = false; // Boolean variable to determine if the trace is already present in the vector
        int i;

        if (strcmp("debug", execution) == 0)
            cout << single_read.address << single_read.function << endl; // Print the address and function of each trace when in debug mode

        for (i = 0; i < vms_pages.size(); i++) {
            if (vms_pages[i].address != single_read.address)
                continue;
            else {
                switch (vms_pages[i].function) {
                    case 'R':
                        switch (single_read.function) {
                            case 'W':
                                vms_pages[i].function = 'W'; // If the functions differ, update the function of the existing trace to 'W'
                                break;
                        }
                        break;
                }
                trace_exists = true; // Set trace_exists to true since the trace is already present in the vector
                break;
            }
        }

        if (!trace_exists) {
            if (vms_pages.size() >= num_frames) {
                switch (vms_pages.front().function) {
                    case 'W':
                        write_count++; // If the ejected/replaced page had function 'W', increment the write count
                        break;
                }
                vms_pages.erase(vms_pages.begin()); // Erase the first element of the vector
                vms_pages.push_back(single_read); // Push the new element at the end of the vector
                read_count++; // Increment the read count
            }
            else {
                vms_pages.push_back(single_read); // Push the new element at the end of the vector
                read_count++; // Increment the read count
            }
        }
    }

    cout << "Memory Frames: " << num_frames << endl;
    cout << "Trace Count: " << event_count << endl;
    cout << "Read Count: " << read_count << endl;
    cout << "Write Count: " << write_count << endl;

    fclose(read_file); // Close the file
}
