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

#define N 500'000

struct Present {
    unsigned int tag;
};

struct PresentChain {
    unsigned int tag;
    Present* next = nullptr;
};

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
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(unorderedBag.begin(), unorderedBag.end(), g);

    // Display the first 10 presents pulled out of the bag
    for (int i = 0; i < 10; ++i) {
        std::cout << unorderedBag[i]->tag << std::endl;
    }

    /****************************** PLACE PRESENTS ON CHAIN ******************************/

    // Present chain
    std::vector<PresentChain*> presentChain(N);

    return 0;
}