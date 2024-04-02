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

#define N 500'000 // C++ 14 notation!

struct Present {
    unsigned int tag;
    Present* next;
};

int main(void) {

    /************************** CREATE GUEST PRESENTS **************************/

    // There are N guest presents
    std::vector<Present> presents(N);

    for (unsigned int i = 0; i < N; ++i) {
        presents[i].tag = i;
    }

    /************************** PLACE PRESENTS IN UNORDERED BAG **************************/

    // Place the N guests' presents into the unordered bag
    Present* unorderedBag[N] = {};

    // Seed random number gen
    std::random_device rd;
    std::mt19937 g(rd());

    // Efficient random shuffling of 0 to N
    std::shuffle(presents.begin(), presents.end(), g);

    // Assign each present in chain after they have been swapped up
    for (unsigned int i = 0; i < (N-1); ++i) {
        unorderedBag[i] = &presents[i];
        unorderedBag[i]->next = &presents[i+1];
    }
    unorderedBag[N-1]->next = NULL;

    // Display the first then presents pulled out of the bag
    for (int i = 0; i < 10; ++i) {
        std::cout << unorderedBag[i]->tag << std::endl;
    }


    return 0;
}