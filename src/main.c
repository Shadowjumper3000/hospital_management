#include "funcs.h"

int main() {
    int count = 0;
    if (DEBUG) {printf("[DEBUG] [main] initializing PatientList\n");};
    struct Patient *patients = malloc((size_t)count * sizeof(struct Patient));
    if (patients == NULL) {
        if (DEBUG) {printf("[DEBUG] [main] Memory Error initializing PatientList\n");};
        return 1;
    }

    
    int choice;
    char name[50];

    do {
        size_t memoryUsage = (size_t)count * sizeof(struct Patient);
        if (DEBUG) {printf("[DEBUG] [main] memoryUsage -> %lu bytes\n", (unsigned long)memoryUsage);};

        printf("\nHospital Management System Menu:\n");
        printf("1. Add a new patient\n");
        printf("2. Delete a patient\n");
        printf("3. Update patient information\n");
        printf("4. Display all patients\n");
        printf("5. Exit\n");
        printf("Enter your choice\n->");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                if (DEBUG) {printf("[DEBUG] [main] calling addPatient()\n");};
                addPatient(&patients, &count);
                break;
            case 2:
                printf("Enter patient Name to delete\n->");
                scanf("%s", name);
                getchar();
                if (DEBUG) {printf("[DEBUG] [main] calling deletePatient()\n");};
                deletePatient(&patients, &count, name);
                break;
            case 3:
                printf("Enter patient Name to update\n->");
                scanf("%s", name);
                getchar();
                if (DEBUG) {printf("[DEBUG] [main] calling updatePatient()\n");};
                updatePatient(patients, count, name);
                break;
            case 4:
                if (DEBUG) {printf("[DEBUG] [main] calling displayPatients()\n");};
                displayPatients(patients, count);
                break;
            case 5:
                free(patients);
                return 0;
            default:
                printf("Not an option\n");
        }
    } while (choice != 5);

    return 0;
}


int addPatient(struct Patient **patients, int *count) {

    *patients = (struct Patient *) realloc(*patients, (size_t)(*count + 1) * sizeof(struct Patient));
    if (DEBUG) {printf("[DEBUG] [addPatient] expanding PatientList\n");};
    if (*patients == NULL) {
        if (DEBUG) {printf("[DEBUG] [addPatient] Memory Error expanding PatientList\n");};
        return 0;
    }

    struct Patient *newPatient = &(*patients)[*count];

    printf("Enter patient name\n->");
    fgets(newPatient->name, sizeof(newPatient->name), stdin);
    newPatient->name[strcspn(newPatient->name, "\n")] = 0;

    printf("Enter age\n->");
    scanf("%d", &newPatient->age);
    getchar();

    printf("Enter gender (M/F)\n->");
    scanf("%c", &newPatient->gender);
    getchar();

    printf("Enter diagnosis\n->");
    fgets(newPatient->diagnosis, sizeof(newPatient->diagnosis), stdin);
    newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = 0;

    printf("Enter treatment\n->");
    fgets(newPatient->treatment, sizeof(newPatient->treatment), stdin);
    newPatient->treatment[strcspn(newPatient->treatment, "\n")] = 0;

    (*count)++;
    return 0;
}


int deletePatient(struct Patient **patients, int *count, char* name) {
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp((*patients)[i].name, name) == 0) {
            found = 1;
            for (int j = i; j < *count - 1; j++) {
                (*patients)[j] = (*patients)[j + 1];
            }
            (*count)--;
            break;
        }
    }

    if (found) {
        printf("Patient deleted\n");
        *patients = realloc(*patients, (size_t)(*count) * sizeof(struct Patient));
        if (*patients == NULL && *count > 0) {
            if (DEBUG) {printf("[DEBUG] [deletePatient] Memory Error deleting Patient\n");};
            return 0;
        }
        if (count == 0) {
            printf("No more Patients in System\n");
        }
    } else {
        if (DEBUG) {printf("[DEBUG] [deletePatient] Patient not found\n");};
    }

    return 0;
}


int updatePatient(struct Patient *patients, int count, char* name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(patients[i].name, name) == 0) {
            printf("Updating patient %s\n", patients[i].name);
            printf("Enter new diagnosis\n->");
            fgets(patients[i].diagnosis, sizeof(patients[i].diagnosis), stdin);
            patients[i].diagnosis[strcspn(patients[i].diagnosis, "\n")] = 0;

            printf("Enter new treatment\n->");
            fgets(patients[i].treatment, sizeof(patients[i].treatment), stdin);
            patients[i].treatment[strcspn(patients[i].treatment, "\n")] = 0;

            printf("Patient information updated successfully\n");

            if (DEBUG) {printf("[DEBUG] [updatePatient] exiting\n");};
            
            return 0;
        }
    }

    if (DEBUG) {printf("[DEBUG] [updatePatient] Patient not found\n");};
    return 1;
}


int displayPatients(struct Patient *patients, int count) {
    if (count == 0) {
        if (DEBUG) {printf("[DEBUG] [displayPatients] no Patients found\n");};
        return 0;
    }

    printf("\nPatient Records:\n");
    for (int i = 0; i < count; i++) {
        printf("Name: %s, Age: %d, Gender: %c, Diagnosis: %s, Treatment: %s\n",
               patients[i].name, patients[i].age, patients[i].gender, patients[i].diagnosis, patients[i].treatment);
    }

    return 0;
}
