# Hospital Management System

## Overview

Dynamically store Patient Records

## Features

- **Add a new patient**: Add a new patient to the system.
- **Delete a patient**: Remove a patient from the system by their ID.
- **Update patient information**: Modify the details of an existing patient.
- **Display all patients**: Show a list of all patients currently in the system.
- **Search for a patient**: Find a patient by their ID or name.

## Usage
Create the following folder hierarchy:

```
hospital_management/
├── .gitignore
├── README.md
├── assets/
│   ├── patient-list.txt
│   └── patient-data/
│       └── <patient-name>.txt
└── src/
    ├── hospital.c
    ├── hospital.h
    └── main.c
```

Compile the program:
```sh
gcc -o hospital_management src/main.c src/hospital.c -I.
```

Run the program:
```sh
./hospital_management
```

## Interact with the menu:
- Choose options to add, delete, update, or display patients.

## Debugging
- The program includes debug messages that can be enabled by setting the DEBUG macro to 1.
