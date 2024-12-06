#include "hospital.h"

int main()
{
    struct Patient *patients = NULL;
    int choice;
    char name[NAME_SIZE];

    // Add a welcome message
    printf("\nWelcome to the Hospital Management System!\n");

    // Ensure required directories exist
    if (ensureDirectoriesExist() != 0) {
        printf("Error creating required directories.\n");
        return 1;
    }

    // Load all patient names into the linked list
    if (loadPatients(&patients) != 0)
    {
        printf("Error loading patient names.\n");
        return 1;
    }
    printf("Patient data loaded successfully.\n"); // Add feedback

    do
    {
        printf("\nHospital Management System Menu:\n");
        printf("0. Exit\n");
        printf("1. Add a new patient\n");
        printf("2. Delete a patient\n");
        printf("3. Update patient information\n");
        printf("4. Display all patients\n");
        printf("5. Search for a patient\n");
        printf("Enter your choice: "); // Improved prompt formatting

        // Input validation for choice
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }
        getchar(); // Consume newline

        switch (choice)
        {
        case 0:
            // Free allocated memory before exit
            while (patients != NULL)
            {
                struct Patient *temp = patients;
                patients = patients->next;
                free(temp);
            }
            return 0;
        case 1:
            if (addPatient(&patients) != 0)
            {
                printf("Failed to add patient.\n");
            }
            break;
        case 2:
            printf("Enter patient name to delete: "); // Improved prompt formatting
            if (fgets(name, sizeof(name), stdin) != NULL)
            {
                name[strcspn(name, "\n")] = 0;
                if (deletePatient(&patients, name) != 0)
                {
                    printf("Failed to delete patient.\n");
                }
            }
            break;
        case 3:
            printf("Enter patient name to update: "); // Improved prompt formatting
            if (fgets(name, sizeof(name), stdin) != NULL)
            {
                name[strcspn(name, "\n")] = 0;
                if (updatePatient(patients, name) != 0)
                {
                    printf("Failed to update patient.\n");
                }
            }
            break;
        case 4:
            if (displayPatients(patients) != 0)
            {
                printf("Failed to display patients.\n");
            }
            break;
        case 5:
            printf("Enter patient name to search: "); // Improved prompt formatting
            if (fgets(name, sizeof(name), stdin) != NULL)
            {
                name[strcspn(name, "\n")] = 0;
                if (searchPatient(patients, name) != 0)
                {
                    printf("Patient not found.\n");
                }
            }
            break;
        default:
            printf("Invalid option. Please choose 1-6.\n"); // More descriptive error message
        }
    } while (1);

    return 0; // Unreachable but good practice
}