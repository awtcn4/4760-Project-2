#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "config.h"

int sharedmem_id;
struct LICENSE *license;

int main(int argc, char* argv[]){
    //create all integers
    int sleepNum = atoi(argv[1]);
    int repeatNum = atoi(argv[2]);
    int currentIteration;

    //create array
    char pid[10];

    //create pid_t variable
    pid_t id;
    id = getpid();

    //Check if there are correct parameters
    //if(argc != 2){
    //    perror("Not a valid number of parameters");
    //    exit(1);
    //}

    //Shared memory ID and shared memory
    sharedmem_id = shmget(IPC_PRIVATE, sizeof(struct LICENSE) * 2, 0666);
    license = (struct LICENSE *)shmat(sharedmem_id, NULL, 0);

    //Setup and Execution of Bakery Algorithm
    int i, j , k, maximum;
    int numChildProcesses = license->childrenProcesses;
    license->choosing[numChildProcesses - 1] = 1;
    for(i = 0; i < repeatNum; i++){
        if((license->turnNum[i]) > maximum){
            maximum = (license->turnNum[i]);
        }
        license->turnNum[(numChildProcesses - 1)] = 1 + maximum;
        license->choosing[(numChildProcesses - 1)] = 0;
        /*for(j = 0; j < numChildProcesses; j++){
            while(license->choosing[j] == 1){}
            while((license->turnNum[j] != 0) && (license->turnNum[j] < license->turnNum[(numChildProcesses -1)])){}
        }*/

        sprintf(pid, "%d", id);
        currentIteration = i++;

        logmsg(pid, currentIteration, repeatNum);
        sleep(sleepNum);
        license->turnNum[(numChildProcesses - 1)] = 0;
    }
    shmdt(license);
    return 0;
}


