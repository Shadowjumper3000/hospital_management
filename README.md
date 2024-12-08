# Hospital Management System

## Overview

A C program for managing hospital patient records with thread-safe operations and file persistence.

## Features

- **Add a new patient**: Add a new patient with name, age, gender, diagnosis, and treatment.
- **Delete a patient**: Remove a patient from the system.
- **Update patient information**: Modify diagnosis and treatment details.
- **Display all patients**: Show a list of all patients with their complete information.
- **Search for patients**: 
  - Search by name and gender (uses ID encoding)
  - Search by patient ID

## Technical Details

- Thread-safe operations using mutex locks
- File-based persistence for patient records
- Unique ID generation using name hashing and gender encoding
- Recursive search implementation
- Dynamic memory management
- Debug logging support

## Project Structure

```
hospital_management/
├── .gitignore
├── README.md
├── CMakeLists.txt
├── assets/
│   ├── patient-list.txt
│   └── patient-data/
│       └── <patient-name>.txt
└── src/
    ├── hospital.c
    ├── hospital.h
    └── main.c
```

## Building the Project

### Using CMake (Recommended)

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

### Manual Compilation

```sh
gcc -o hospital_management src/main.c src/hospital.c -I. -pthread
```

### Windows-specific
Ensure you have MinGW or Visual Studio with C compiler installed.

## Running the Program

```sh
./hospital_management  # Linux/MacOS
hospital_management.exe  # Windows
```

## Development

### Prerequisites
- C compiler (GCC/Clang/MSVC)
- CMake 3.5.0 or higher
- pthread support (included in Windows builds)

### Debug Mode
Set `DEBUG` macro to 1 in hospital.h to enable detailed logging:

```c
#define DEBUG 1
```

## Data Storage
- Patient list maintained in `assets/patient-list.txt`
- Individual patient records stored in `assets/patient-data/<patient-name>.txt`
- Automatic directory creation on first run

## Thread Safety
All file operations and linked list modifications are protected by mutex locks for concurrent access safety.
