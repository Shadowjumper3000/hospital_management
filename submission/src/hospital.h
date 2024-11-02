#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 1

#define NAME_SIZE 50
#define DIAGNOSIS_SIZE 100
#define TREATMENT_SIZE 100
#define PATIENT_DIR "../assets/patient-data/"
#define PATIENT_LIST "../assets/patient-list.txt"

/// @brief Patient struct
struct Patient {
    char name[NAME_SIZE];
    int age;
    char gender;
    char diagnosis[DIAGNOSIS_SIZE];
    char treatment[TREATMENT_SIZE];
    struct Patient *next;
};

int loadPatients(struct Patient **head);
int savePatient(struct Patient *patient);
int addPatient(struct Patient **head);
int deletePatient(struct Patient **head, const char *name);
int updatePatient(struct Patient *head, const char *name);
int displayPatients(struct Patient *head);
int searchPatient(struct Patient *head, const char *name);

#endif