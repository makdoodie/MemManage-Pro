#include "policies.h"
#include "dataStructures.h"
#include "fifo.cpp"
#include "lru.cpp"

using namespace std;

void executeFifo(char* tracefile, int num_frames, char* execution) {
    fifo(tracefile, num_frames, execution);
}

void executeLru(char* tracefile, int num_frames, char* execution) {
    lru(tracefile, num_frames, execution);
}

int validateExecutionMode(const char* execution) {
    if (strcmp("quiet", execution) == 0)
        return 1;
    else if (strcmp("debug", execution) == 0)
        return 2;
    else
        return -1;
}

int main(int argc, char* argv[]) {
    if (argc == 5) {
        int num_frames = atoi(argv[2]); // Convert the number of frames inserted by the user into an integer
        int execution_mode = validateExecutionMode(argv[4]);

        if (strcmp("fifo", argv[3]) == 0) {
            switch (execution_mode) {
                case 1:
                    executeFifo(argv[1], num_frames, "quiet");
                    break;
                case 2:
                    executeFifo(argv[1], num_frames, "debug");
                    break;
                default:
                    cout << "Invalid execution mode." << endl;
                    break;
            }
        }
        else if (strcmp("lru", argv[3]) == 0) {
            switch (execution_mode) {
                case 1:
                    executeLru(argv[1], num_frames, "quiet");
                    break;
                case 2:
                    executeLru(argv[1], num_frames, "debug");
                    break;
                default:
                    cout << "Invalid execution mode." << endl;
                    break;
            }
        }
        else {
            cout << "Invalid page replacement algorithm." << endl;
        }
    }
    else {
        cout << "Invalid arguments were provided." << endl;
    }

    return 0;
}
