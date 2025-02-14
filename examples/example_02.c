//
// Created by Daniel on 2/14/2025.
//

/*
 * WARNING: Needs to be linked with `test_perft.c`
 */


// Needs to be defined, `test_perft.c` uses just #include "chess.h" without the implementation
#define CHESS_IMPLEMENTATION
#include "chess.h"

// Defines extern definition of `run_tests` function
extern void run_tests();

int main(void)
{
    // Run all test cases
    run_tests();

    return 0;
}