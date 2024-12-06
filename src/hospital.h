#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

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
struct Patient
{
    char name[NAME_SIZE];
    int age;
    char gender;
    char diagnosis[DIAGNOSIS_SIZE];
    char treatment[TREATMENT_SIZE];
    struct Patient *next;
};

/// @brief Data structure for patient check-in
struct CheckInData
{
    char name[NAME_SIZE];
    int age;
    Gender gender;
    char diagnosis[DIAGNOSIS_SIZE];
    char treatment[TREATMENT_SIZE];
    struct Patient **head;
};

// Function declarations
struct Patient *searchPatientRecursive(struct Patient *current, const char *name);
unsigned int encodePatientID(int age, char gender);
void decodePatientID(unsigned int id, int *age, char *gender);
void *checkInPatient(void *arg);
int loadPatient(const char *name, struct Patient *patient);
int loadPatients(struct Patient **head);
int savePatient(struct Patient *patient);
int addPatient(struct Patient **head);
int deletePatient(struct Patient **head, const char *name);
int updatePatient(struct Patient *head, const char *name);
int displayPatients(struct Patient *head);
int searchPatient(struct Patient *head, const char *name);
int ensureDirectoriesExist(void);

#endif