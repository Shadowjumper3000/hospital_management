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
        printf("5. Search by name and gender\n");
        printf("6. Search by ID\n");
        printf("Enter your choice: ");

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
            printf("Enter patient name: ");
            if (fgets(name, sizeof(name), stdin) != NULL) {
                name[strcspn(name, "\n")] = 0;
                char gender_input;
                printf("Enter gender (M/F/O): ");
                scanf(" %c", &gender_input);
                getchar(); // Consume newline
                
                Gender gender;
                switch(toupper(gender_input)) {
                    case 'F': gender = FEMALE; break;
                    case 'O': gender = OTHER; break;
                    default: gender = MALE; break;
                }
                
                struct Patient *found = searchPatient(patients, name, gender);
                if (found) {
                    printf("\nFound patient:\n");
                    printf("ID: %u\n", found->patient_id);
                    printf("Name: %s\n", found->name);
                    printf("Age: %d\n", found->age);
                    printf("Gender: %c\n", gender_input);
                    printf("Diagnosis: %s\n", found->diagnosis);
                    printf("Treatment: %s\n", found->treatment);
                    printf("------------------------\n");
                } else {
                    printf("Patient not found.\n");
                }
            }
            break;
            
        case 6:
            printf("Enter patient ID: ");
            unsigned int search_id;
            if (scanf("%u", &search_id) == 1) {
                getchar(); // Consume newline
                struct Patient *found = searchPatientByID(patients, search_id);
                if (found) {
                    printf("\nFound patient:\n");
                    printf("ID: %u\n", found->patient_id);
                    printf("Name: %s\n", found->name);
                    printf("Age: %d\n", found->age);
                    char gender_display;
                    switch(found->gender) {
                        case FEMALE: gender_display = 'F'; break;
                        case OTHER: gender_display = 'O'; break;
                        default: gender_display = 'M'; break;
                    }
                    printf("Gender: %c\n", gender_display);
                    printf("Diagnosis: %s\n", found->diagnosis);
                    printf("Treatment: %s\n", found->treatment);
                    printf("------------------------\n");
                } else {
                    printf("Patient not found.\n");
                }
            }
            break;
        default:
            printf("Invalid option. Please choose 1-6.\n"); // More descriptive error message
        }
    } while (1);

    return 0;
}