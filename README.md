### Makefile Documentation

This Makefile compiles and runs one of two C++ programs, based on an argument provided by the user:

1. **`birthday_presents_party.cc`** (Argument: `1`)
2. **`atmospheric_temperature_reading_module.cc`** (Argument: `2`)

#### Prerequisites

- GNU Make
- g++ Compiler

#### How to Use

- For `birthday_presents_party.cc`:
  ```sh
  make PROBLEM=1
  ```
- For `atmospheric_temperature_reading_module.cc`:
  ```sh
  make PROBLEM=2
  ```

#### Targets

- `all`: Compiles and runs the selected program.
- `build`: Compiles the selected source file.
- `clean`: Removes executables and object files.
- `run`: Executes the compiled program.

## Problem 1: birthday_presents_party

### Output Formatting:

```
The first ten presents pullout out of the bag are 144373 72127 496154 112587 199680 194214 63265 432421 131379 112581 
Servant 0 has started working.
Servant 2 has started working.
Servant 1 has started working.
Servant 3 has started working.
Servant 2: Present with tag 100000 is in the list.
Servant 2: Present with tag 100010 is NOT in the list.
Servant 2: Wrote a THANK YOU for present 100000!
Servant 0: Present with tag 200000 is in the list.
Servant 0: Present with tag 200010 is NOT in the list.
Servant 0: Wrote a THANK YOU for present 200000!
Servant 0: Present with tag 0 is in the list.
Servant 0: Present with tag 10 is NOT in the list.
Servant 0: Wrote a THANK YOU for present 0!
Servant 0: Present with tag 300000 is in the list.
Servant 0: Present with tag 300010 is NOT in the list.
Servant 0: Wrote a THANK YOU for present 300000!
Servant 3: Present with tag 400000 is in the list.
Servant 3: Present with tag 400010 is NOT in the list.
Servant 3: Wrote a THANK YOU for present 400000!
Servant 2 has completed working. 
Servant 1 has completed working.
Servant 3 has completed working.
Servant 0 has completed working.
```

#### Problem 1: atmospheric_temperature_reading_module

```
Sensor thread 0 starting.
Sensor thread 2 starting.
Sensor thread 3 starting.
Sensor thread 4 starting.
Sensor thread 1 starting.
Sensor thread 5 starting.
Sensor thread 6 starting.
Sensor thread 7 starting.
Report thread starting.
Hour 1 Report:
        Highest temperatures: 70 70 70 70 70
        Lowest temperatures: -100 -100 -100 -99 -99
        10-minute interval with largest difference starts at minute: 31

Hour 2 Report:
        Highest temperatures: 69 69 70 70 70
        Lowest temperatures: -100 -99 -99 -98 -97
        10-minute interval with largest difference starts at minute: 36

Hour 3 Report:
        Highest temperatures: 66 67 67 68 70
        Lowest temperatures: -99 -99 -99 -99 -99
        10-minute interval with largest difference starts at minute: 17

Hour 4 Report:
        Highest temperatures: 69 69 69 70 70
        Lowest temperatures: -100 -100 -100 -100 -99
        10-minute interval with largest difference starts at minute: 3

Sensor thread 3 stopping.
Sensor thread 6 stopping.
Sensor thread 2 stopping.
Sensor thread 5 stopping.
Sensor thread 4 stopping.
Sensor thread 1 stopping.
Sensor thread 0 stopping.
Sensor thread 7 stopping.
Hour 5 Report:
        Highest temperatures: 68 68 68 69 70
        Lowest temperatures: -100 -100 -99 -99 -99
        10-minute interval with largest difference starts at minute: 44

Report thread stopping.
```

### Statement:

#### Problem 1: birthday_presents_party

The program is a multithreaded temperature monitoring simulation where each of 8 sensor threads generates random temperatures within a specified range and stores them in a shared data structure, protected by mutexes to prevent data races. It uses an atomic variable simulation clock, which increments to simulate time passing. A condition variable is used to synchronize sensor readings with the clock, similar to how real clocked sensors would work. Another thread processes and reports the collected data hourly, sorting temperatures to identify extremes and variations. The simulation's time is accelerated by a given factor (in this case x5000) to five hour simulation in less than four seconds. The main function manages the start, synchronization, and conclusion of all threads, demonstrating efficient thread synchronization and data protection in a concurrent environment. The design of the program ensures that reports are only generated following the final hour's sensor reading using one of the eight threads to process. Additionally, a global mutex (console_mutex) manages access to the standard output, ensuring coherent output amidst concurrent operations.

#### Problem 2: atmospheric_temperature_reading_module

This program employs multithreading to simultaneously handle a thread-safe linked list of presents, each marked with a unique identifier. By initiating four threads, it facilitates parallel processing, with each thread assigned a specific quarter of the presents from a shuffled vector (unorderedBag), thus dividing the workload efficiently. The linked list operations—addition, removal, and search—are synchronized via a mutex (list_mutex) to ensure mutual exclusion and prevent data races among threads. Additionally, a global mutex (console_mutex) manages access to the standard output, ensuring coherent output amidst concurrent operations.

### Proof of Correctness:

#### Problem 1: birthday_presents_party

The program's output confirms the random arrangement of present tags, the sequential start and finish of multiple threads, and the traversal of key presents through the linked list across multiple threads.

#### Problem 2: atmospheric_temperature_reading_module

The output showcases the highest and lowest sensor readings along with the commencement and conclusion of thread operations. Tt processes and generates hourly reports to identify the most significant ten-minute intervals for minimum and maximum temperatures. Manual debugging of the output corroborates these findings and the timing process.

### Experimental Evaluations:

#### Problem 1: birthday_presents_party

Minimal experimental evaluations were required for this problem. The focus was on securing the shared memory during access via mutexes. The runtime was so fast, I didn't expiriment with creating exclusive memory segments and merging them as needed.

#### Problem 2: atmospheric_temperature_reading_module

My first approach involved using the sleep function to simulate timing, which faced synchronization challenges between the reporting and sensor threads. I switching to a simulation clock and condition variable, offering a more precise and accurate simulation of real-world scenarios. This adjustment better aligned with the problem's requirements of a clocked sensor sim.

### Report Questions: ### 

#### Problem 1: birthday_presents_party

Unfortunately, the servants realized at the end of the day that they had more
presents than “Thank you” notes. What could have gone wrong? 

The servants were out-of-sync. It is likely that no mutexes were used and two servants grabbed a present off the chain at the same time without realizing it. This is why critical sections should be locked down upon access.

#### Problem 2: atmospheric_temperature_reading_module

In your report, discuss the efficiency, correctness, and progress guarantee of your program.

Efficiency: The program efficiently manages thread synchronization using mutexes and condition variables to minimize active waiting.

Correctness: The program correctly uses mutexes, atomic variables, and condition variables to ensure thread-safe operations and maintain data integrity. The atomic operations on "simulationClock" and proper mutex management prevents data races, ensuring the program behaves as expected.

Progress Guarantee: The design ensures progress by using condition variables to prevent threads from being stuck indefinitely. Sensor threads and the report thread progress based on the simulation clock, which is reliably incremented, allowing all threads to advance through their tasks as the sim progresses.



