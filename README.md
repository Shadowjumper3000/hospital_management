# Hospital Management System

## Overview

Dynamically store Patient Records

## Features

- **Add a new patient** Add a new patient to the system.
- **Delete a patient** Remove a patient from the system by their ID.
- **Update patient information** Modify the details of an existing patient.
- **Display all patients** Show a list of all patients currently in the system.

## Usage
Create the following folder hierarchy:

```
hospital_management/
├── src/
│   ├── main.c
│   ├── hospital.c
│   └── hospital.h
└── assets/
    └── patient-data/
    └── patient-list.txt
```

Compile the program:
```sh
gcc -o hospital_management main.c -I.
```

Run the program:
```sh
./hospital_management
```

## Interact with the menu:
- Choose options to add, delete, update, or display patients.

## Debugging
- The program includes debug messages that can be enabled by setting the DEBUG macro to 1.
