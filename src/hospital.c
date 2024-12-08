#include "hospital.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Encode patient ID using bit manipulation
// Uses first 6 bits for age (0-63) and last 2 bits for gender (00=MALE, 01=FEMALE, 10=OTHER)
unsigned int encodePatientID(const char* name, Gender gender)
{
    unsigned int id = 0;
    // Hash the name
    for (const char *c = name; *c; c++) {
        id = (id * 31 + *c);
    }
    // Reserve last 2 bits for gender
    id = (id << 2) | (gender & 0x3);
    return id;
}

int savePatient(struct Patient *patient)
{
    if (patient == NULL)
    {
        printf("[DEBUG] [savePatient] Null patient pointer\n");
        return 1;
    }

    printf("[DEBUG] [savePatient] Starting save\n");
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s%s.txt", PATIENT_DIR, patient->name);
    printf("[DEBUG] [savePatient] Saving to: %s\n", filePath);

    FILE *fp = fopen(filePath, "w");
    if (fp == NULL)
    {
        printf("[DEBUG] [savePatient] Failed to open file: %s\n", strerror(errno));
        return 1;
    }

    fprintf(fp, "name: %s\n", patient->name);
    fprintf(fp, "id: %u\n", patient->patient_id);
    fprintf(fp, "age: %d\n", patient->age);
    fprintf(fp, "gender: %c\n", patient->gender);
    fprintf(fp, "diagnosis: %s\n", patient->diagnosis);
    fprintf(fp, "treatment: %s\n", patient->treatment);

    fclose(fp);
    printf("[DEBUG] [savePatient] Save completed\n");
    return 0;
}

int loadPatient(const char *name, struct Patient *patient)
{
    if (name == NULL || patient == NULL)
    {
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

    // Read patient's basic information
    if (!fgets(buffer, sizeof(buffer), fp))
    {
        printf("[DEBUG] Failed to read patient name\n");
        goto error;
    }
    if (sscanf(buffer, "name: %[^\n]", patient->name) != 1)
    {
        printf("[DEBUG] Invalid name format\n");
        goto error;
    }

    // Read patient ID
    if (!fgets(buffer, sizeof(buffer), fp))
    {
        printf("[DEBUG] Failed to read patient ID\n");
        goto error;
    }
    if (sscanf(buffer, "id: %u", &patient->patient_id) != 1)
    {
        printf("[DEBUG] Invalid ID format\n");
        goto error;
    }

    // Read age
    if (!fgets(buffer, sizeof(buffer), fp))
    {
        printf("[DEBUG] Failed to read patient age\n");
        goto error;
    }
    if (sscanf(buffer, "age: %d", &patient->age) != 1)
    {
        printf("[DEBUG] Invalid age format\n");
        goto error;
    }

    // Read gender
    char gender;
    if (!fgets(buffer, sizeof(buffer), fp))
    {
        printf("[DEBUG] Failed to read patient gender\n");
        goto error;
    }
    if (sscanf(buffer, "gender: %c", &gender) != 1)
    {
        printf("[DEBUG] Invalid gender format\n");
        goto error;
    }
    patient->gender = gender;

    // Read medical information
    if (!fgets(buffer, sizeof(buffer), fp))
    {
        printf("[DEBUG] Failed to read diagnosis\n");
        goto error;
    }
    if (sscanf(buffer, "diagnosis: %[^\n]", patient->diagnosis) != 1)
    {
        printf("[DEBUG] Invalid diagnosis format\n");
        goto error;
    }

    if (!fgets(buffer, sizeof(buffer), fp))
    {
        printf("[DEBUG] Failed to read treatment\n");
        goto error;
    }
    if (sscanf(buffer, "treatment: %[^\n]", patient->treatment) != 1)
    {
        printf("[DEBUG] Invalid treatment format\n");
        goto error;
    }

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
    if (DEBUG)
    {
        printf("[DEBUG] [loadPatients] Loading patients\n");
    }
    if (head == NULL)
    {
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
    *head = NULL; // Initialize the list

    // Read each patient name and load their data
    if (DEBUG)
    {
        printf("[DEBUG] [loadPatients] Reading patient names\n");
    }
    while (fgets(name, sizeof(name), list_fp))
    {
        name[strcspn(name, "\n")] = 0; // Remove newline

        struct Patient *newPatient = malloc(sizeof(struct Patient));
        if (newPatient == NULL)
        {
            if (DEBUG)
            {
                printf("[DEBUG] [loadPatients] Failed to allocate memory for new patient\n");
            }
            fclose(list_fp);
            pthread_mutex_unlock(&lock);
            return 1;
        }

        if (loadPatient(name, newPatient) != 0)
        {
            if (DEBUG)
            {
                printf("[DEBUG] [loadPatients] Failed to load patient data for %s\n", name);
            }
            free(newPatient);
            continue;
        }

        // Add to front of list
        newPatient->next = *head;
        *head = newPatient;
        if (DEBUG)
        {
            printf("[DEBUG] [loadPatients] Successfully loaded patient: %s\n", name);
        }
    }

    fclose(list_fp);
    pthread_mutex_unlock(&lock);
    return 0;
}

int addPatient(struct Patient **head)
{
    if (DEBUG) printf("[DEBUG] Starting addPatient function\n");
    
    struct Patient *newPatient = malloc(sizeof(struct Patient));
    if (newPatient == NULL)
    {
        if (DEBUG) printf("[DEBUG] Failed to allocate memory for new patient\n");
        return 1;
    }

    // Get patient info using fgets instead of scanf
    printf("Enter patient name: ");
    if (fgets(newPatient->name, NAME_SIZE, stdin) == NULL)
    {
        if (DEBUG) printf("[DEBUG] Failed to read name\n");
        free(newPatient);
        return 1;
    }
    newPatient->name[strcspn(newPatient->name, "\n")] = 0;
    if (DEBUG) printf("[DEBUG] Name read: %s\n", newPatient->name);

    printf("Enter patient age: ");
    if (scanf("%d", &newPatient->age) != 1)
    {
        if (DEBUG) printf("[DEBUG] Failed to read age\n");
        free(newPatient);
        return 1;
    }
    getchar(); // Consume newline
    if (DEBUG) printf("[DEBUG] Age read: %d\n", newPatient->age);

    // Modified gender input to use enum
    char gender_input;
    printf("Enter patient gender (M/F/O): ");
    if (scanf(" %c", &gender_input) != 1)
    {
        if (DEBUG) printf("[DEBUG] Failed to read gender\n");
        free(newPatient);
        return 1;
    }
    getchar(); // Consume newline

    // Convert character input to Gender enum
    switch(toupper(gender_input)) {
        case 'F': newPatient->gender = FEMALE; break;
        case 'O': newPatient->gender = OTHER; break;
        case 'M': 
        default: newPatient->gender = MALE; break;
    }
    if (DEBUG) printf("[DEBUG] Gender read: %c\n", gender_input);

    // Generate patient ID using the enum
    newPatient->patient_id = encodePatientID(newPatient->name, newPatient->gender);
    if (DEBUG) printf("[DEBUG] Generated patient ID: %u\n", newPatient->patient_id);

    // ... rest of patient data collection ...
    printf("Enter diagnosis: ");
    if (fgets(newPatient->diagnosis, DIAGNOSIS_SIZE, stdin) == NULL)
    {
        if (DEBUG) printf("[DEBUG] Failed to read diagnosis\n");
        free(newPatient);
        return 1;
    }
    newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = 0;
    if (DEBUG) printf("[DEBUG] Diagnosis read: %s\n", newPatient->diagnosis);

    printf("Enter treatment: ");
    if (fgets(newPatient->treatment, TREATMENT_SIZE, stdin) == NULL)
    {
        if (DEBUG) printf("[DEBUG] Failed to read treatment\n");
        free(newPatient);
        return 1;
    }
    newPatient->treatment[strcspn(newPatient->treatment, "\n")] = 0;
    if (DEBUG) printf("[DEBUG] Treatment read: %s\n", newPatient->treatment);

    pthread_mutex_lock(&lock);
    newPatient->next = *head;
    *head = newPatient;
    pthread_mutex_unlock(&lock);

    if (DEBUG) printf("[DEBUG] Successfully added new patient\n");
    return 0;
}

int deletePatient(struct Patient **head, const char *name)
{
    if (*head == NULL || name == NULL)
        return 1;

    pthread_mutex_lock(&lock);

    struct Patient *current = *head;
    struct Patient *prev = NULL;

    while (current != NULL && strcmp(current->name, name) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        pthread_mutex_unlock(&lock);
        return 1;
    }

    if (prev == NULL)
    {
        *head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    free(current);
    pthread_mutex_unlock(&lock);
    return 0;
}

int updatePatient(struct Patient *head, const char *name)
{
    struct Patient *patient = searchPatient(head, name, MALE); // Try male first
    if (patient == NULL) {
        patient = searchPatient(head, name, FEMALE); // Try female
    }
    if (patient == NULL) {
        patient = searchPatient(head, name, OTHER); // Try other
    }
    if (patient == NULL) {
        return 1;
    }

    printf("Enter new diagnosis: ");
    scanf("%s", patient->diagnosis);
    printf("Enter new treatment: ");
    scanf("%s", patient->treatment);

    return savePatient(patient);
}

int displayPatients(struct Patient *head)
{
    pthread_mutex_lock(&lock);
    struct Patient *current = head;

    while (current != NULL)
    {
        printf("Patient ID: %u\n", current->patient_id);
        printf("Name: %s\n", current->name);
        printf("Age: %d\n", current->age);
        // Convert enum to character for display
        char gender_display;
        switch(current->gender) {
            case FEMALE: gender_display = 'F'; break;
            case OTHER: gender_display = 'O'; break;
            case MALE: default: gender_display = 'M'; break;
        }
        printf("Gender: %c\n", gender_display);
        printf("Diagnosis: %s\n", current->diagnosis);
        printf("Treatment: %s\n", current->treatment);
        printf("------------------------\n");
        current = current->next;
    }

    pthread_mutex_unlock(&lock);
    return 0;
}

Patient *searchPatient(Patient *head, const char *name, Gender gender)
{
    // Base case: reached end of list
    if (head == NULL) {
        return NULL;
    }

    // Check if current node matches
    unsigned int target_id = encodePatientID(name, gender);
    if (head->patient_id == target_id) {
        return head;
    }

    // Recursively search rest of list
    return searchPatient(head->next, name, gender);
}

Patient *searchPatientByID(Patient *head, unsigned int id)
{
    // Base case: reached end of list
    if (head == NULL) {
        return NULL;
    }

    // Check if current node matches
    if (head->patient_id == id) {
        return head;
    }

    // Recursively search rest of list
    return searchPatientByID(head->next, id);
}

int ensureDirectoriesExist(void)
{
    printf("\n[DEBUG] [ensureDirectoriesExist] Checking directories...\n");

    struct stat st = {0};
    if (stat("assets", &st) == -1)
    {
        printf("[DEBUG] Creating assets directory\n");
        if (mkdir("assets", 0700) == -1)
        {
            printf("[ERROR] Failed to create assets directory: %s\n", strerror(errno));
            return 1;
        }
    }

    if (stat("assets/patient-data", &st) == -1)
    {
        printf("[DEBUG] Creating patient-data directory\n");
        if (mkdir("assets/patient-data", 0700) == -1)
        {
            printf("[ERROR] Failed to create patient-data directory: %s\n", strerror(errno));
            return 1;
        }
    }

    printf("[DEBUG] Checking patient list file\n");
    FILE *fp = fopen(PATIENT_LIST, "a");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to create/open patient list: %s\n", strerror(errno));
        return 1;
    }
    fclose(fp);

    printf("[DEBUG] Directory structure verified\n");
    return 0;
}
