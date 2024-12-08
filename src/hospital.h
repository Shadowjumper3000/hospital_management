#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

#define DEBUG 1

#define NAME_SIZE 50
#define DIAGNOSIS_SIZE 100
#define TREATMENT_SIZE 100
#define PATIENT_DIR "./assets/patient-data/"
#define PATIENT_LIST "./assets/patient-list.txt"

// Add near the top, before struct definitions
typedef enum {
    MALE,
    FEMALE,
    OTHER
} Gender;

/// @brief Patient struct
typedef struct Patient
{
    char name[NAME_SIZE];
    int age;
    Gender gender;
    unsigned int patient_id;
    char diagnosis[DIAGNOSIS_SIZE];
    char treatment[TREATMENT_SIZE];
    struct Patient *next;
} Patient;

// Function declarations
unsigned int encodePatientID(const char* name, Gender gender);
int loadPatient(const char *name, struct Patient *patient);
int loadPatients(struct Patient **head);
int savePatient(struct Patient *patient);
int addPatient(struct Patient **head);
int deletePatient(struct Patient **head, const char *name);
int updatePatient(struct Patient *head, const char *name);
int displayPatients(struct Patient *head);
Patient *searchPatient(struct Patient *head, const char *name, Gender gender);
int ensureDirectoriesExist(void);
Patient *searchPatientByID(struct Patient *head, unsigned int id);

#endif