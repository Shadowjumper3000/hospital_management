#include "funcs.h"

struct Patient {
    int id;
    char name[50];
    int age;
    char gender;
    char diagnosis[100];
    char treatment[100];
    //struct Patient *next;
};


int main() {
    int capacity = 0;
    int count = 0;
    struct Patient *patients = malloc((size_t)capacity * sizeof(struct Patient));
    if (patients == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    
    int choice;
    int id;

    if (DEBUG) {printf("[DEBUG] [main] initializing menu\n");};

    do {
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
                printf("Enter patient ID to delete\n->");
                scanf("%d", &id);
                getchar();
                if (DEBUG) {printf("[DEBUG] [main] calling deletePatient()\n");};
                deletePatient(&patients, &count, id);
                break;
            case 3:
                printf("Enter patient ID to update\n->");
                scanf("%d", &id);
                getchar(); // Consume the newline character left by scanf
                if (DEBUG) {printf("[DEBUG] [main] calling updatePatient()\n");};
                updatePatient(patients, count, id);
                break;
            case 4:
                if (DEBUG) {printf("[DEBUG] [main] calling displayPatients()\n");};
                displayPatients(patients, count);
                break;
            case 5:
                free(patients);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
    if (DEBUG) {printf("[DEBUG] [main] exiting\n");};
    return 0;
}


int addPatient(struct Patient **patients, int *count) {
    if (DEBUG) {printf("[DEBUG] [addPatient] initializing\n");};

    static int last_id = 0;

    if (*count % 5 == 0) {
        *patients = (struct Patient *) realloc(*patients, (size_t)(*count + 5) * sizeof(struct Patient));
        if (DEBUG) {printf("[DEBUG] [addPatient] expanding PatientList\n");};
        if (*patients == NULL) {
            printf("Memory reallocation failed!\n");
            return 0;
        }
    }

    struct Patient *newPatient = &(*patients)[*count];

    if (*count == 0) {
        newPatient->id = 1;
    } else {
        newPatient->id = last_id + 1;
    }
    last_id = newPatient->id;
    
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


int deletePatient(struct Patient **patients, int *count, int id) {
    if (DEBUG) {printf("[DEBUG] [deletePatient] initializing\n");};

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if ((*patients)[i].id == id) {
            found = 1;
            for (int j = i; j < *count - 1; j++) {
                (*patients)[j] = (*patients)[j + 1];
            }
            (*count)--;
            break;
        }
    }

    if (found) {
        printf("Patient deleted successfully.\n");
        *patients = realloc(*patients, (size_t)(*count) * sizeof(struct Patient));
        if (*patients == NULL && *count > 0) {
            printf("Memory reallocation failed!\n");
            return 0;
        }
    } else {
        printf("Patient not found.\n");
    }

    return 0;
}


int updatePatient(struct Patient *patients, int count, int id) {
    if (DEBUG) {printf("[DEBUG] [updatePatient] initializing\n");};

    for (int i = 0; i < count; i++) {
        if (patients[i].id == id) {
            printf("Updating patient %d:\n", patients[i].id);
            printf("Enter new diagnosis\n->");
            fgets(patients[i].diagnosis, sizeof(patients[i].diagnosis), stdin);
            patients[i].diagnosis[strcspn(patients[i].diagnosis, "\n")] = 0;

            printf("Enter new treatment\n->");
            fgets(patients[i].treatment, sizeof(patients[i].treatment), stdin);
            patients[i].treatment[strcspn(patients[i].treatment, "\n")] = 0;

            printf("Patient information updated successfully.\n");
            return 0;
        }
    }
    printf("Patient not found.\n");

    return 1;
}


int displayPatients(struct Patient *patients, int count) {
    if (DEBUG) {printf("[DEBUG] [displayPatients] initializing\n");};

    if (count == 0) {
        printf("No patients found.\n");
        return 0;
    }

    printf("\nPatient Records:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Age: %d, Gender: %c, Diagnosis: %s, Treatment: %s\n",
               patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].diagnosis, patients[i].treatment);
    }

    return 0;
}
