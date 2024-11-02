#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

int main() {
    struct Patient *patients = NULL;
    int choice;
    char name[NAME_SIZE];

    // Load all patient names into the linked list
    if (loadPatients(&patients) != 0) {
        printf("Error loading patient names.\n");
        return 1;
    }

    do {
        printf("\nHospital Management System Menu:\n");
        printf("1. Add a new patient\n");
        printf("2. Delete a patient\n");
        printf("3. Update patient information\n");
        printf("4. Display all patients\n");
        printf("5. Search for a patient\n");
        printf("6. Exit\n");
        printf("Enter your choice\n->");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addPatient(&patients);
                break;
            case 2:
                printf("Enter patient name to delete\n->");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                deletePatient(&patients, name);
                break;
            case 3:
                printf("Enter patient name to update\n->");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                updatePatient(patients, name);
                break;
            case 4:
                displayPatients(patients);
                break;
            case 5:
                printf("Enter patient name to search\n->");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                searchPatient(patients, name);
                break;
            case 6:
                return 0;
            default:
                printf("Not an option\n");
        }
    } while (choice != 6);

    return 0;
}