# Linked List & Parking Management

Check-in and Check-out car in a 10 lot parking space.

* Based on singly linked list (C)
* Console Application
* gcc or clang compiler
* regex library
* make

## Features

* Check-in
* Check-out
* Display current status
* Log files for retaining previous day parking details
* Check-out log file containing details of cars checked out.
* Other settings (deleting log files)

### Installation

* Compiled using gcc or clang.
* Change the CC value in makefile

```bash
make carpark
```

* To run the application,

```bash
./carpark
```

#### Branches

* Master: Stable working branch
* refactor: Testing branch
* Legacy: Code before the refactor

#### Variables

|      Variable          | Value |
|------------------------|-------|
|Total parking lots      | 10    |
|Minimum parking amount  | 5     |
|Amount per minute       | 0.25  |
