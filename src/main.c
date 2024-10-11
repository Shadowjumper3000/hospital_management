#include "funcs.h"

struct Patient {
    char name[50];
    int age;
    char gender;
    char diagnosis[100];
    char treatment[100];
    //struct Patient *next;
};


int main() {
    int capacity = 5;
    int count = 0;

    
    int choice;
    char name[50];

    do {
        printf("\nHospital Management System Menu:\n");
        printf("1. Add a new patient\n");
        printf("2. Delete a patient\n");
        printf("3. Update patient information\n");
        printf("4. Display all patients\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addPatient(&patients, &count);
                break;
            case 2:
                printf("Enter patient name to delete: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                deletePatient(&patients, &count, name);
                break;
            case 3:
                printf("Enter patient name to update: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                updatePatient(patients, count, name);
                break;
            case 4:
                displayPatients(patients, count);
                break;
            case 5:
                freePatients(patients, count);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}


int addPatient(struct Patient **patients, int *count) {
    if (*count % 5 == 0) {
        *patients = realloc(*patients, (*count + 5) * sizeof(struct Patient));
        if (*patients == NULL) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }

    struct Patient *newPatient = &(*patients)[*count];
    printf("Enter patient name: ");
    fgets(newPatient->name, sizeof(newPatient->name), stdin);
    newPatient->name[strcspn(newPatient->name, "\n")] = 0;

    printf("Enter age: ");
    scanf("%d", &newPatient->age);
    getchar();

    printf("Enter gender (M/F): ");
    scanf("%c", &newPatient->gender);
    getchar();

    printf("Enter diagnosis: ");
    fgets(newPatient->diagnosis, sizeof(newPatient->diagnosis), stdin);
    newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = 0;

    printf("Enter treatment: ");
    fgets(newPatient->treatment, sizeof(newPatient->treatment), stdin);
    newPatient->treatment[strcspn(newPatient->treatment, "\n")] = 0;

    (*count)++;
}


int deletePatient(struct Patient **patients, int *count, char *name) {
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
        printf("Patient deleted successfully.\n");
    } else {
        printf("Patient not found.\n");
    }
}


int updatePatient(struct Patient *patients, int count, char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(patients[i].name, name) == 0) {
            printf("Updating patient %s:\n", patients[i].name);
            printf("Enter new diagnosis: ");
            fgets(patients[i].diagnosis, sizeof(patients[i].diagnosis), stdin);
            patients[i].diagnosis[strcspn(patients[i].diagnosis, "\n")] = 0;

            printf("Enter new treatment: ");
            fgets(patients[i].treatment, sizeof(patients[i].treatment), stdin);
            patients[i].treatment[strcspn(patients[i].treatment, "\n")] = 0;

            printf("Patient information updated successfully.\n");
            return;
        }
    }
    printf("Patient not found.\n");
}


int displayPatients(struct Patient *patients, int count) {
    if (count == 0) {
        printf("No patients found.\n");
        return;
    }

    printf("\nPatient Records:\n");
    for (int i = 0; i < count; i++) {
        printf("Name: %s, Age: %d, Gender: %c, Diagnosis: %s, Treatment: %s\n",
               patients[i].name, patients[i].age, patients[i].gender, patients[i].diagnosis, patients[i].treatment);
    }
}


int displayPatient() {

}
