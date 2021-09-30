#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "config.h"

#define MAX_CANON 100

int sharedmem_id;
struct LICENSE *license;
char input[];

int main(int argc, char *argv[]){
    pid_t pid;
    signal(SIGINT, sigHandler);
    sharedmem_id = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    license = (struct License *)shmat(sharedmem_id, NULL, 0);
    int processesNum;

    //Check if argument is appropriate
    if(argc != 2){
        perror("Invalid number of arguments");
        exit(1);
    }

    processesNum = atoi(argv[1]);
    license->nLicenses = processesNum;
    license->processesRunning++;
    initlicense();
    //Make loop until EOF
    //Read a line from stdin of up to MAX_CANON characters
    while(fgets(input, MAX_CANON, stdin) != NULL){
        //Request a license
        getlicense();
            if((pid = fork()) == 0){
                if((license = (struct LICENSE *) shmat(sharedmem_id, NULL, 0)) == (struct LICENSE *) -1){
                    perror("runsim: Error: Trouble attaching shared memory");
                    shmctl(sharedmem_id, IPC_RMID, NULL);
                    shmdt(license);
                    exit(1);
                }
                char inputFormat[20];
                strcpy(inputFormat, "./");
                strcat(inputFormat, input);
                docommand(input);
            } else if(pid < 0){
                 perror("runsim: Error: Problem making process");
                 shmctl(sharedmem_id, IPC_RMID, NULL);
                 shmdt(license);
                 exit(1);
            }
            pid = waitpid(-1, NULL, WNOHANG);
            if(pid == -1){
                license->processesRunning--;
                returnlicense();
        }
}
    shmctl(sharedmem_id, IPC_RMID, NULL);
    shmdt(license);

return 0;
}

void docommand(char string[]){
    pid_t gcpid = 0;
    int gc = 0;
    getlicense();
    gcpid = fork();
    int avail;
    if((avail = getlicense()) == 1){
            wait(&license->nLicenses);
    }else {
        if(gcpid == 0){
        gc++;
        getlicense();
        char *path;
        char *argument1;
        char *argument2;
        char *argument3;
        char delimiter[] = " ";
        path = strtok(string, delimiter);
        argument1 = strtok(NULL, delimiter);
        argument2 = strtok(NULL, delimiter);
        argument3 = strtok(NULL, delimiter);
        execl(path, path, argument1, argument2, argument3, NULL);
    } else if(gcpid < 0){
        fprintf(stderr, "Couldn't create process");
        perror("runsim: Error:");
        shmctl(sharedmem_id, IPC_RMID, NULL);
        shmdt(license);
        exit(1);
    } else {
        gcpid = waitpid(-1, NULL, WNOHANG);
            if(gcpid == -1){
                gc--;
            }

        }
    }

}

void sigHandler(){
    pid_t id = getpgrp();
    shmctl(sharedmem_id, IPC_RMID, NULL);
    shmdt(license);
    killpg(id, SIGINT);
    exit(1);
}

