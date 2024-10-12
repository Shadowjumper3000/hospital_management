#ifndef FUNCS_H
#define FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 1

struct Patient {
    char name[50];
    int age;
    char gender;
    char diagnosis[100];
    char treatment[100];
    //struct Patient *next;
};

int addPatient();
int deletePatient();
int updatePatient();
int displayPatients();

#endif