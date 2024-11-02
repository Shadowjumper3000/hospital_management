#include "hospital.h"


int savePatient(struct Patient *patient) {
    char filePath[100];
    snprintf(filePath, sizeof(filePath), "%s%s.txt", PATIENT_DIR, patient->name);
    FILE *fp = fopen(filePath, "w");
    if (fp == NULL) {
        if (DEBUG) {printf("[DEBUG] [savePatient] Error opening file\n");};
        return 1;
    }
    fprintf(fp, "name: %s\n", patient->name);
    fprintf(fp, "age: %d\n", patient->age);
    fprintf(fp, "gender: %c\n", patient->gender);
    fprintf(fp, "diagnosis: %s\n", patient->diagnosis);
    fprintf(fp, "treatment: %s\n", patient->treatment);
    fclose(fp);
    return 0;
}

int loadPatient(const char *name, struct Patient *patient) {
    char filePath[100];
    snprintf(filePath, sizeof(filePath), "%s%s.txt", PATIENT_DIR, name);
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        if (DEBUG) {printf("[DEBUG] [loadPatient] Error opening file\n");}
        return 1;
    }
    fscanf(fp, "name: %[^\n]\n", patient->name);
    fscanf(fp, "age: %d\n", &patient->age);
    fscanf(fp, "gender: %c\n", &patient->gender);
    fscanf(fp, "diagnosis: %[^\n]\n", patient->diagnosis);
    fscanf(fp, "treatment: %[^\n]\n", patient->treatment);
    fclose(fp);
    return 0;
}

int loadPatients(struct Patient **head) {
    FILE *list_fp = fopen(PATIENT_LIST, "r");
    if (list_fp == NULL) {
        if (DEBUG) {printf("[DEBUG] [loadPatients] Error opening patient list file\n");};
        return 1;
    }

    char line[NAME_SIZE];
    while (fgets(line, sizeof(line), list_fp)) {
        line[strcspn(line, "\n")] = 0;

        struct Patient *newPatient = (struct Patient *)malloc(sizeof(struct Patient));
        if (newPatient == NULL) {
            if (DEBUG) {printf("[DEBUG] [loadPatients] Memory allocation error\n");};
            fclose(list_fp);
            return 1;
        }

        if (loadPatient(line, newPatient) != 0) {
            free(newPatient);
            continue;
        }

        newPatient->next = *head;
        *head = newPatient;
    }

    fclose(list_fp);
    return 0;
}

int addPatient(struct Patient **head) {
    struct Patient *newPatient = (struct Patient *)malloc(sizeof(struct Patient));
    if (newPatient == NULL) {
        if (DEBUG) {printf("[DEBUG] [addPatient] Memory allocation error\n");};
        return 1;
    }

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

    newPatient->next = *head;
    *head = newPatient;

    FILE *list_fp = fopen(PATIENT_LIST, "a");
    if (list_fp == NULL) {
        if (DEBUG) {printf("[DEBUG] [addPatient] Error opening patient list file\n");};
        return 1;
    }
    fprintf(list_fp, "%s\n", newPatient->name);
    fclose(list_fp);

    return savePatient(newPatient);
}

int deletePatient(struct Patient **head, const char *name) {
    struct Patient *current = *head;
    struct Patient *previous = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        if (DEBUG) {printf("[DEBUG] [deletePatient] Patient not found\n");};
        return 1;
    }

    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);

    FILE *list_fp = fopen(PATIENT_LIST, "r");
    if (list_fp == NULL) {
        if (DEBUG) {printf("[DEBUG] [deletePatient] Error opening patient list file\n");};
        return 1;
    }

    FILE *temp_fp = fopen(PATIENT_LIST ".tmp", "w");
    if (temp_fp == NULL) {
        if (DEBUG) {printf("[DEBUG] [deletePatient] Error opening temporary file\n");};
        fclose(list_fp);
        return 1;
    }

    char line[NAME_SIZE];
    while (fgets(line, sizeof(line), list_fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, name) != 0) {
            fprintf(temp_fp, "%s\n", line);
        }
    }

    fclose(list_fp);
    fclose(temp_fp);

    remove(PATIENT_LIST);
    rename(PATIENT_LIST ".tmp", PATIENT_LIST);

    char filePath[100];
    snprintf(filePath, sizeof(filePath), "%s%s.txt", PATIENT_DIR, name);
    remove(filePath);

    printf("Patient deleted\n");
    return 0;
}

int updatePatient(struct Patient *head, const char *name) {
    struct Patient *current = head;

    while (current != NULL && strcmp(current->name, name) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        if (DEBUG) {printf("[DEBUG] [updatePatient] Patient not found\n");};
        return 1;
    }

    printf("Enter new diagnosis\n->");
    fgets(current->diagnosis, sizeof(current->diagnosis), stdin);
    current->diagnosis[strcspn(current->diagnosis, "\n")] = 0;

    printf("Enter new treatment\n->");
    fgets(current->treatment, sizeof(current->treatment), stdin);
    current->treatment[strcspn(current->treatment, "\n")] = 0;

    return savePatient(current);
}

int displayPatients(struct Patient *head) {
    struct Patient *current = head;

    if (current == NULL) {
        if (DEBUG) {printf("[DEBUG] [displayPatients] No patients found\n");};
        return 1;
    }

    printf("\nPatient Records:\n");
    while (current != NULL) {
        printf("Name: %s, Age: %d, Gender: %c, Diagnosis: %s, Treatment: %s\n",
               current->name, current->age, current->gender, current->diagnosis, current->treatment);
        current = current->next;
    }

    return 0;
}

int searchPatient(struct Patient *head, const char *name) {
    struct Patient *current = head;

    while (current != NULL && strcmp(current->name, name) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Patient not found\n");
        return 1;
    }

    printf("Name: %s\n", current->name);
    printf("Age: %d\n", current->age);
    printf("Gender: %c\n", current->gender);
    printf("Diagnosis: %s\n", current->diagnosis);
    printf("Treatment: %s\n", current->treatment);
    return 0;
}