#include "hospital.h"
#include <errno.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Helper function for recursive search
struct Patient *searchPatientRecursive(struct Patient *current, const char *name)
{
    // Base case: reached end of list
    if (current == NULL)
    {
        return NULL;
    }

    // Found the patient
    if (strcmp(current->name, name) == 0)
    {
        return current;
    }

    // Recursively search rest of list
    return searchPatientRecursive(current->next, name);
}

// Encode patient ID using bit manipulation
// Uses first 7 bits for age (0-127) and last bit for gender (0=M, 1=F)
unsigned int encodePatientID(int age, char gender) {
    unsigned int id = 0;
    // Set age in first 7 bits
    id = (age & 0x7F) << 1;
    // Set gender in last bit
    if (gender == 'F' || gender == 'f') {
        id |= 1;
    }
    return id;
}

// Decode patient ID back into age and gender
void decodePatientID(unsigned int id, int *age, char *gender) {
    // Extract age from first 7 bits
    *age = (id >> 1) & 0x7F;
    // Extract gender from last bit
    *gender = (id & 1) ? 'F' : 'M';
}

void *checkInPatient(void *arg) {
    printf("[DEBUG] Thread started\n");
    struct CheckInData *data = (struct CheckInData *)arg;
    
    // Create new patient
    struct Patient *newPatient = malloc(sizeof(struct Patient));
    if (newPatient == NULL) {
        printf("[DEBUG] Memory allocation failed\n");
        return NULL;
    }
    printf("[DEBUG] Patient memory allocated\n");

    // Copy data
    strncpy(newPatient->name, data->name, NAME_SIZE - 1);
    newPatient->name[NAME_SIZE - 1] = '\0';
    newPatient->age = data->age;
    newPatient->gender = data->gender;
    strncpy(newPatient->diagnosis, data->diagnosis, DIAGNOSIS_SIZE - 1);
    newPatient->diagnosis[DIAGNOSIS_SIZE - 1] = '\0';
    strncpy(newPatient->treatment, data->treatment, TREATMENT_SIZE - 1);
    newPatient->treatment[TREATMENT_SIZE - 1] = '\0';
    printf("[DEBUG] Patient data copied\n");

    // Lock for file operations
    pthread_mutex_lock(&lock);
    printf("[DEBUG] Mutex locked\n");

    // Add to list
    newPatient->next = *(data->head);
    *(data->head) = newPatient;
    printf("[DEBUG] Added to list\n");

    // Save to files
    FILE *list_fp = fopen(PATIENT_LIST, "a");
    if (list_fp == NULL) {
        printf("[DEBUG] Failed to open patient list\n");
        pthread_mutex_unlock(&lock);
        free(newPatient);
        return NULL;
    }
    fprintf(list_fp, "%s\n", newPatient->name);
    fclose(list_fp);
    printf("[DEBUG] Patient list updated\n");

    if (savePatient(newPatient) != 0) {
        printf("[DEBUG] Failed to save patient data\n");
    }
    printf("[DEBUG] Patient data saved\n");

    pthread_mutex_unlock(&lock);
    printf("[DEBUG] Mutex unlocked\n");
    printf("[DEBUG] Thread completed\n");
    
    return NULL;
}

int savePatient(struct Patient *patient)
{
    if (patient == NULL) {
        return 1;
    }

    pthread_mutex_lock(&lock);
    char filePath[100];
    snprintf(filePath, sizeof(filePath), "%s%s.txt", PATIENT_DIR, patient->name);
    FILE *fp = fopen(filePath, "w");
    if (fp == NULL)
    {
        if (DEBUG)
        {
            printf("[DEBUG] [savePatient] Error opening file\n");
        };
        pthread_mutex_unlock(&lock);
        return 1;
    }
    fprintf(fp, "name: %s\n", patient->name);
    fprintf(fp, "age: %d\n", patient->age);
    fprintf(fp, "gender: %c\n", patient->gender); // Changed to %c for character
    fprintf(fp, "diagnosis: %s\n", patient->diagnosis);
    fprintf(fp, "treatment: %s\n", patient->treatment);
    fclose(fp);
    pthread_mutex_unlock(&lock);
    return 0;
}

int loadPatient(const char *name, struct Patient *patient)
{
    if (name == NULL || patient == NULL) {
        return 1;
    }

    pthread_mutex_lock(&lock);
    char filePath[100];
    snprintf(filePath, sizeof(filePath), "%s%s.txt", PATIENT_DIR, name);
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL)
    {
        if (DEBUG)
        {
            printf("[DEBUG] [loadPatient] Error opening file\n");
        }
        pthread_mutex_unlock(&lock);
        return 1;
    }
    
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), fp) == NULL || sscanf(buffer, "name: %[^\n]", patient->name) != 1) goto error;
    if (fgets(buffer, sizeof(buffer), fp) == NULL || sscanf(buffer, "age: %d", &patient->age) != 1) goto error;
    char gender;
    if (fgets(buffer, sizeof(buffer), fp) == NULL || sscanf(buffer, "gender: %c", &gender) != 1) goto error;
    patient->gender = gender;
    if (fgets(buffer, sizeof(buffer), fp) == NULL || sscanf(buffer, "diagnosis: %[^\n]", patient->diagnosis) != 1) goto error;
    if (fgets(buffer, sizeof(buffer), fp) == NULL || sscanf(buffer, "treatment: %[^\n]", patient->treatment) != 1) goto error;
    
    fclose(fp);
    pthread_mutex_unlock(&lock);
    return 0;

error:
    fclose(fp);
    pthread_mutex_unlock(&lock);
    return 1;
}

int loadPatients(struct Patient **head)
{
    if (DEBUG) {
        printf("[DEBUG] [loadPatients] Loading patients\n");
    }
    if (head == NULL) {
        return 1;
    }

    pthread_mutex_lock(&lock);
    FILE *list_fp = fopen(PATIENT_LIST, "r");
    if (list_fp == NULL)
    {
        if (DEBUG)
        {
            printf("[DEBUG] [loadPatients] Error opening patient list file\n");
        };
        pthread_mutex_unlock(&lock);
        return 1;
    }

    char name[NAME_SIZE];
    *head = NULL;  // Initialize the list

    // Read each patient name and load their data
    if (DEBUG) {
        printf("[DEBUG] [loadPatients] Reading patient names\n");
    }
    while (fgets(name, sizeof(name), list_fp)) {
        name[strcspn(name, "\n")] = 0;  // Remove newline
        
        struct Patient *newPatient = malloc(sizeof(struct Patient));
        if (newPatient == NULL) {
            if (DEBUG) {
                printf("[DEBUG] [loadPatients] Failed to allocate memory for new patient\n");
            }
            fclose(list_fp);
            pthread_mutex_unlock(&lock);
            return 1;
        }

        if (loadPatient(name, newPatient) != 0) {
            if (DEBUG) {
                printf("[DEBUG] [loadPatients] Failed to load patient data for %s\n", name);
            }
            free(newPatient);
            continue;
        }

        // Add to front of list
        newPatient->next = *head;
        *head = newPatient;
        if (DEBUG) {
            printf("[DEBUG] [loadPatients] Successfully loaded patient: %s\n", name);
        }
    }

    fclose(list_fp);
    pthread_mutex_unlock(&lock);
    return 0;
}

int addPatient(struct Patient **head) {
    printf("[DEBUG] Starting addPatient function\n");
    
    struct CheckInData *data = malloc(sizeof(struct CheckInData));
    if (data == NULL) {
        printf("[DEBUG] Failed to allocate memory for CheckInData\n");
        return 1;
    }
    
    data->head = head;
    printf("[DEBUG] Memory allocated successfully\n");
    
    // Get patient info using fgets instead of scanf
    printf("Enter patient name: ");
    if (fgets(data->name, NAME_SIZE, stdin) == NULL) {
        printf("[DEBUG] Failed to read name\n");
        free(data);
        return 1;
    }
    data->name[strcspn(data->name, "\n")] = 0;
    printf("[DEBUG] Name read: %s\n", data->name);

    printf("Enter patient age: ");
    if (scanf("%d", &data->age) != 1) {
        printf("[DEBUG] Failed to read age\n");
        free(data);
        return 1;
    }
    getchar();
    printf("[DEBUG] Age read: %d\n", data->age);

    printf("Enter patient gender (M/F/O): ");
    if (scanf(" %c", &data->gender) != 1) {
        printf("[DEBUG] Failed to read gender\n");
        free(data);
        return 1;
    }
    getchar();
    printf("[DEBUG] Gender read: %c\n", data->gender);

    printf("Enter diagnosis: ");
    if (fgets(data->diagnosis, DIAGNOSIS_SIZE, stdin) == NULL) {
        printf("[DEBUG] Failed to read diagnosis\n");
        free(data);
        return 1;
    }
    data->diagnosis[strcspn(data->diagnosis, "\n")] = 0;
    printf("[DEBUG] Diagnosis read: %s\n", data->diagnosis);

    printf("Enter treatment: ");
    if (fgets(data->treatment, TREATMENT_SIZE, stdin) == NULL) {
        printf("[DEBUG] Failed to read treatment\n");
        free(data);
        return 1;
    }
    data->treatment[strcspn(data->treatment, "\n")] = 0;
    printf("[DEBUG] Treatment read: %s\n", data->treatment);
    
    printf("[DEBUG] Creating thread for patient check-in\n");
    pthread_t thread;
    if (pthread_create(&thread, NULL, checkInPatient, data) != 0) {
        printf("[DEBUG] Failed to create thread\n");
        free(data);
        return 1;
    }
    
    printf("[DEBUG] Waiting for thread to complete\n");
    pthread_join(thread, NULL);
    printf("[DEBUG] Thread completed\n");
    
    return 0;
}

int deletePatient(struct Patient **head, const char *name) {
    if (*head == NULL || name == NULL) return 1;
    
    pthread_mutex_lock(&lock);
    
    struct Patient *current = *head;
    struct Patient *prev = NULL;
    
    while (current != NULL && strcmp(current->name, name) != 0) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        pthread_mutex_unlock(&lock);
        return 1;
    }
    
    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
    pthread_mutex_unlock(&lock);
    return 0;
}

int updatePatient(struct Patient *head, const char *name) {
    struct Patient *patient = searchPatientRecursive(head, name);
    if (patient == NULL) return 1;
    
    printf("Enter new diagnosis: ");
    scanf("%s", patient->diagnosis);
    printf("Enter new treatment: ");
    scanf("%s", patient->treatment);
    
    return savePatient(patient);
}

int displayPatients(struct Patient *head) {
    pthread_mutex_lock(&lock);
    struct Patient *current = head;
    
    while (current != NULL) {
        printf("Name: %s\n", current->name);
        printf("Age: %d\n", current->age);
        printf("Gender: %c\n", current->gender);
        printf("Diagnosis: %s\n", current->diagnosis);
        printf("Treatment: %s\n", current->treatment);
        printf("------------------------\n");
        current = current->next;
    }
    
    pthread_mutex_unlock(&lock);
    return 0;
}

int searchPatient(struct Patient *head, const char *name) {
    struct Patient *patient = searchPatientRecursive(head, name);
    if (patient == NULL) {
        printf("Patient not found.\n");
        return 1;
    }
    
    printf("Name: %s\n", patient->name);
    printf("Age: %d\n", patient->age);
    printf("Gender: %c\n", patient->gender);
    printf("Diagnosis: %s\n", patient->diagnosis);
    printf("Treatment: %s\n", patient->treatment);
    return 0;
}

int ensureDirectoriesExist(void) {
    printf("\n[DEBUG] [ensureDirectoriesExist] Checking directories...\n");

    struct stat st = {0};
    if (stat("assets", &st) == -1) {
        printf("[DEBUG] Creating assets directory\n");
        if (mkdir("assets", 0700) == -1) {
            printf("[ERROR] Failed to create assets directory: %s\n", strerror(errno));
            return 1;
        }
    }

    if (stat("assets/patient-data", &st) == -1) {
        printf("[DEBUG] Creating patient-data directory\n");
        if (mkdir("assets/patient-data", 0700) == -1) {
            printf("[ERROR] Failed to create patient-data directory: %s\n", strerror(errno));
            return 1;
        }
    }

    printf("[DEBUG] Checking patient list file\n");
    FILE *fp = fopen(PATIENT_LIST, "a");
    if (fp == NULL) {
        printf("[ERROR] Failed to create/open patient list: %s\n", strerror(errno));
        return 1;
    }
    fclose(fp);

    printf("[DEBUG] Directory structure verified\n");
    return 0;
}
