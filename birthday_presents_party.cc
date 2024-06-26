/*
COP4520-24Spring 0V01: Concept of Parallel and Distributed Processing
Programming Assignment Problem 1
Instructor: Juan Parra
Due date: Thursday, April 13th by 11:59 PM
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <optional>
#include <mutex>
#include <thread>
#include <ctime> 

#define N 500'000
#define MOD_PRINT 100'000

std::mutex console_mutex; // Global mutex for cout control (just for pretty)

struct Present {
    unsigned int tag;
    Present* next;
};

class PresentLLChain {
public:
    Present* head;
    std::mutex list_mutex;

    PresentLLChain() : head(nullptr) {}

    void addPresent(Present* newPresent) {
        std::lock_guard<std::mutex> lock(list_mutex);
        
        // If the list is empty or the new present tag is less than the head tag, insert the new present at the beginning.
        if (head == nullptr || newPresent->tag < head->tag) {
            newPresent->next = head;
            head = newPresent;
        } else {
            // Find the insert pos
            Present* current = head;
            while (current->next != nullptr && current->next->tag < newPresent->tag) {
                current = current->next;
            }

            // Insert the new present after the current present
            newPresent->next = current->next;
            current->next = newPresent;
        }
    }

    Present* removePresent() {
        std::lock_guard<std::mutex> lock(list_mutex);
        if (head == nullptr) {
            return nullptr;
        }
        Present* temp = head;
        head = head->next;
        return temp;
    }

    bool search(unsigned int tag) {
        std::lock_guard<std::mutex> lock(list_mutex);
        Present* current = head;
        while (current != nullptr) {
            if (current->tag == tag) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

void processPresents(PresentLLChain& chain, std::vector<Present*>& unorderedBag, int thread_id) {
    std::unique_lock<std::mutex> consoleLock(console_mutex);    
    std::cout << "Servant " << thread_id << " has started working. " << std::endl;
    consoleLock.unlock();

    for (unsigned int i = thread_id; i < N; i += 4) { // Each thread processes a fourth of the presents (proud of this one)
        chain.addPresent(unorderedBag[i]); // Add a present
        unsigned int tag = unorderedBag[i]->tag;
        if (tag % MOD_PRINT == 0) {
            if (chain.search(tag)) {
                std::unique_lock<std::mutex> consoleLock(console_mutex);
                std::cout << "Servant " << thread_id << ": Present with tag " << tag << " is in the list.\n";
                consoleLock.unlock();
            } else {
                std::unique_lock<std::mutex> consoleLock(console_mutex);
                std::cout << "Servant " << thread_id << ": Present with tag " << tag << " is NOT in the list.\n";
                consoleLock.unlock();
            }
            if (chain.search(tag + 10)) {
                std::unique_lock<std::mutex> consoleLock(console_mutex);
                std::cout << "Servant " << thread_id << ": Present with tag " << tag + 10 << " is in the list.\n";
                consoleLock.unlock();
            } else {
                std::unique_lock<std::mutex> consoleLock(console_mutex);
                std::cout << "Servant " << thread_id << ": Present with tag " << tag + 10 << " is NOT in the list.\n";
                consoleLock.unlock();
            }
        }
        auto removedPresent = chain.removePresent(); // Remove a present
        if (removedPresent && tag % MOD_PRINT == 0) { // Check if removedPresent is not nullptr and a meets conditions
            std::unique_lock<std::mutex> consoleLock(console_mutex);
            std::cout << "Servant " << thread_id << ": Wrote a THANK YOU for present " << tag << "!\n";
            consoleLock.unlock();
        }
    }
    std::cout << "Servant " << thread_id << " has completed working. " << std::endl;
}

int main(void) {

    /******************************* CREATE GUEST PRESENTS *******************************/

    // There are N guest presents
    std::vector<Present> presents(N);

    for (unsigned int i = 0; i < N; ++i) {
        presents[i].tag = i;
    }

    /************************** PLACE PRESENTS IN UNORDERED BAG **************************/

    // Place the N guests' presents into the bag
    std::vector<Present*> unorderedBag(N);
    for (int i = 0; i < N; ++i) {
        unorderedBag[i] = &presents[i];
    }

    // Shuffle presents in bag
    auto seed = std::time(0);
    std::mt19937 g(seed);
    std::shuffle(unorderedBag.begin(), unorderedBag.end(), g);

    // Display the first 10 presents pulled out of the bag
    std::cout << "The first ten presents pullout out of the bag are ";
    for (int i = 0; i < 10; ++i) {
        std::cout << unorderedBag[i]->tag << " ";
    }
    std::cout << std::endl;

    /**************************** CHAIN MANAGEMENT BY SERVANTS ****************************/
    PresentLLChain chain;

    // Create and start threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(processPresents, std::ref(chain), std::ref(unorderedBag), i);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}