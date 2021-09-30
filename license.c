#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "config.h"

int initlicense(){
    license->allUsed = 0;
    return license->childrenProcesses;
}

int returnlicense(){
    license->nLicenses++;
    return license->nLicenses;
}

int getlicense(){
    if(license->nLicenses == 0){
        wait(&license->nLicenses);
    }else{
        license->nLicenses--;
    }
    return license->allUsed;
}

void addtolicenses(int n){
    license->nLicenses += n;
    return;
}

void removelicenses(int n){
    license->nLicenses -= n;
    return;
}

void logmsg(char* pid, int *iterationNum, int *totalIteration){
    FILE *fp;
    fp = fopen("logfile.data", "a");
        if(fp == NULL){
            perror("Failed to open logfile");
            exit(1);
        }
        time_t tm;
        time(&tm);
        struct tm *timeptr = localtime(&tm);
        fprintf(fp, "%.2d:%.2d:%.2d", timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec);
        fprintf(fp, "%s Iteration %d of %d\n", pid, iterationNum, totalIteration);
        fclose(fp);
}

