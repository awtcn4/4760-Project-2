#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

extern int sharedmem_id;
extern struct LICENSE *license;

struct LICENSE{
    int nLicenses;
    int processesRunning;
    int childrenProcesses;
    int choosing[50];
    int turnNum[50];
    int allUsed;
};

void sigHandler();
void docommand();

extern int getlicense ();
extern int returnlicense();
extern int initlicense();
extern void addtolicenses(int n);
extern void removelicenses(int n);
extern void logmsg(char* pid, int *iterationNum, int *totalIteration);

#endif

