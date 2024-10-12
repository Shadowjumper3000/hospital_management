#ifndef FUNCS_H
#define FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 1

struct Patient;

int addPatient();
int deletePatient();
int updatePatient();
// int displayPatient();
int displayPatients();

int clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard characters until newline or EOF
    }
    return 0;
}

#endif