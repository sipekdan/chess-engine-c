//
// Created by Daniel on 2/14/2025.
//

#define CHESS_IMPLEMENTATION
#include "chess.h"

int main(void)
{
    // Initialize storage for board
    char board[64];

    // Copies the starting board to the `board`
    COPY_BOARD(board, INITIAL_BOARD);

    // Prints board to stdout
    print_board(board);

    // Destination that will be filled with valid moves
    Move valid_moves[256];

    // Counter of valid moves
    unsigned char count;

    // Generates valid moves for current board state for `white` player
    generate_valid_moves(board, valid_moves, &count, WHITE, INITIAL_CASTLE, NO_MOVE);

    // Prints the valid moves to stdout
    print_valid_moves(valid_moves, count);

    return 0;
}
