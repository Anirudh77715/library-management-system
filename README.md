﻿# Library Management System
Sure! Here is a consolidated version of the `README.md` file that includes all the instructions and details in one place:

```markdown
# Library Management System

This project provides a Library Management System that allows you to manage library operations efficiently. Below are the instructions to compile, set up, and run the program.

## Prerequisites

Make sure you have the necessary tools installed:
- `make` (for building the project)
- A Unix-based environment (Linux/macOS) or Windows with a tool like Git Bash

## Setup and Run Instructions

Follow these steps to compile, set up, and run the program:

### 1. Compile the Program
To compile the project, run:
```bash
make
```

### 2. Ensure the Data Directory Exists
Make sure the `data` directory exists by running:
```bash
mkdir -p data
```

### 3. Set Permissions for the Executable
Ensure that the executable file has the correct permissions to run:
```bash
chmod +x ./library_system
```

### 4. Run the Program
Now, you can run the compiled program:
```bash
./library_system
```

### Combined Command
To perform all of the above steps in a single command, you can use:
```bash
make clean && make && mkdir -p data && chmod +x ./library_system && ./library_system
```

This command will:
- Clean the previous build,
- Rebuild the project using `make`,
- Create the `data` directory,
- Ensure the executable has proper permissions, and
- Run the program.

### Restart the Program
If the program is already set up, and you simply need to restart it, run:
```bash
./library_system
```

## Troubleshooting

- If you encounter any issues with the compilation, ensure that the `Makefile` is correctly configured and that all required dependencies are installed.
- For permission-related issues, make sure you have the necessary rights to execute the program.
- If there is an issue with the `make` process, check for missing files or errors in the `Makefile`.


