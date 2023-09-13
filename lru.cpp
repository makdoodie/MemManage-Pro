#include "policies.h"
#include "dataStructures.h"

using namespace std;

void lru(char* tracefile, int num_frames, char* execution) {
    vector <memorysystem> lru_pages; // Vector to store pages in LRU order
    memorysystem single_read; // Struct to store information of a single read from the tracefile
    int read_count = 0; // Counter for the number of reads (pagefaults)
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

        for (i = 0; i < lru_pages.size(); i++) {
            if (lru_pages[i].address != single_read.address)
                continue;
            else {
                memorysystem temporary = lru_pages[i]; // Store the contents of the existing trace temporarily
                switch (lru_pages[i].function) {
                    case 'R':
                        switch (single_read.function) {
                            case 'W':
                                temporary.function = 'W'; // If the functions differ, update the function of the temporary variable to 'W'
                                break;
                        }
                        break;
                }
                lru_pages.erase(lru_pages.begin() + i); // Erase the element at index i
                lru_pages.push_back(temporary); // Push the new element at the end of the vector
                trace_exists = true; // Set trace_exists to true since the trace is already present in the vector
                break;
            }
        }

        if (!trace_exists) {
            if (lru_pages.size() >= num_frames) {
                switch (lru_pages.front().function) {
                    case 'W':
                        write_count++; // If the ejected/replaced page had function 'W', increment the write count
                        break;
                }
                lru_pages.erase(lru_pages.begin()); // Erase the element at index 0
                lru_pages.push_back(single_read); // Push the new element at the end of the vector
                read_count++; // Increment the read count
            }
            else {
                lru_pages.push_back(single_read); // Push the new element at the end of the vector
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
