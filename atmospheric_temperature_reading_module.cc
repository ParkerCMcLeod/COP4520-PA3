/*
COP4520-24Spring 0V01: Concept of Parallel and Distributed Processing
Programming Assignment Problem 2
Instructor: Juan Parra
Due date: Friday, April 5th by 11:59 PM
*/

#include <iostream>
#include <vector>
#include <random>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <thread>
#include <chrono>
#include <condition_variable>

// Simulation parameters
constexpr int NUM_SENSORS = 8;
constexpr int TEMP_MIN = -100;
constexpr int TEMP_MAX = 70;
constexpr int READINGS_PER_HOUR = 60;
constexpr int SIMULATION_HOURS = 5;
constexpr int SIMULATION_SPEEDUP_FACTOR = 5000;
constexpr int MILLISECONDS_PER_SIMULATED_MINUTE = 1000 * 60 / SIMULATION_SPEEDUP_FACTOR;
constexpr int MILLISECONDS_PER_SIMULATED_HOUR = 1000 * 60 * 60 / SIMULATION_SPEEDUP_FACTOR;

// Global storage for temperature readings
std::vector<std::vector<int>> hourlyTemperatures(SIMULATION_HOURS, std::vector<int>(READINGS_PER_HOUR * NUM_SENSORS));
std::mutex tempMutex; // Mutex for temperature data access synchronization
std::mutex consoleLock; // Added mutex for console output synchronization
std::condition_variable cv; // Condition variable for timing synchronization
std::atomic<int> simulationClock(0); // Atomic simulation clock

// Thread function for sensor simulation
void sensorThread(int sensorID) {
    {
        std::lock_guard<std::mutex> console(consoleLock);
        std::cout << "Sensor thread " << sensorID << " starting.\n";
    }

    std::random_device rd;
    auto seed = rd() ^ (
        std::hash<std::thread::id>()(std::this_thread::get_id()) +
        std::chrono::system_clock::now().time_since_epoch().count()
    );
    std::mt19937 gen(seed); // Generator for random temperature
    std::uniform_int_distribution<> distr(TEMP_MIN, TEMP_MAX); // Distribution for temperature

    int lastClockCheck = 0;
    for (int hour = 0; hour < SIMULATION_HOURS; ++hour) {
        for (int reading = 0; reading < READINGS_PER_HOUR; ++reading) {
            std::unique_lock<std::mutex> lk(tempMutex);

            // Wait for the next minute in the simulation time
            cv.wait(lk, [&] { return simulationClock.load(std::memory_order_acquire) >= lastClockCheck + 1; });
            lastClockCheck = simulationClock.load(std::memory_order_acquire); // Ensures synchronization with current simulation time
            lk.unlock();

            int temp = distr(gen); // Generate random temperature
            // Update the global temperature readings in a thread-safe way
            {
                std::lock_guard<std::mutex> lock(tempMutex);
                hourlyTemperatures[hour][sensorID + reading * NUM_SENSORS] = temp;
            }
        }
    }

    {
        std::lock_guard<std::mutex> console(consoleLock);
        std::cout << "Sensor thread " << sensorID << " stopping.\n";
    }
}

// Thread function for generating hourly reports
void reportThread() {
    {
        std::lock_guard<std::mutex> console(consoleLock);
        std::cout << "Report thread starting.\n";
    }

    int lastClockCheck = 0;
    for (int hour = 0; hour < SIMULATION_HOURS; ++hour) {
        std::unique_lock<std::mutex> lk(tempMutex);

        // Wait until the end of the current hour in simulation time
        cv.wait(lk, [&] { return simulationClock.load(std::memory_order_acquire) >= lastClockCheck + 60; });
        lastClockCheck += 60;
        lk.unlock();

        // Sort temperatures to prepare for report generation
        std::sort(hourlyTemperatures[hour].begin(), hourlyTemperatures[hour].end());

        // Find lowest and highest temperatures for the report
        std::vector<int> lowest(hourlyTemperatures[hour].begin(), hourlyTemperatures[hour].begin() + 5);
        std::vector<int> highest(hourlyTemperatures[hour].end() - 5, hourlyTemperatures[hour].end());

        // Find the 10-minute interval with the largest temperature difference
        int maxDiff = 0, intervalStart = 0;
        for (size_t i = 0; i <= hourlyTemperatures[hour].size() - 10; ++i) {
            int diff = hourlyTemperatures[hour][i + 9] - hourlyTemperatures[hour][i];
            if (diff > maxDiff) {
                maxDiff = diff;
                intervalStart = i;
            }
        }

        // Print the hourly report
        {
            std::lock_guard<std::mutex> console(consoleLock);
            std::cout << "Hour " << hour + 1 << " Report:";
            std::cout << "\n\tHighest temperatures: ";
            for (int temp : highest) std::cout << temp << " ";
            std::cout << "\n\tLowest temperatures: ";
            for (int temp : lowest) std::cout << temp << " ";
            std::cout << "\n\t10-minute interval with largest difference starts at minute: " << intervalStart / NUM_SENSORS << std::endl << std::endl;
        }
    }

    {
        std::lock_guard<std::mutex> console(consoleLock);
        std::cout << "Report thread stopping.\n";
    }
}

int main(void) {
    std::cout << std::endl;

    std::vector<std::thread> threads; // Store threads for each sensor

    // Create sensor threads
    for (int i = 0; i < NUM_SENSORS; ++i) {
        threads.emplace_back(sensorThread, i);
    }

    // Create the report generator thread
    std::thread reporter(reportThread);

    // Simulate passage of time and notify threads if timing updates
    for (int minute = 0; minute < SIMULATION_HOURS * 60; ++minute) {
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_PER_SIMULATED_MINUTE));
        simulationClock.fetch_add(1, std::memory_order_release);
        cv.notify_all();
    }

    // Wait for threads to complete
    for (auto& t : threads) {
        t.join();
    }
    reporter.join();

    return 0;
    std::cout << std::endl;
}
