#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chess.h"

void print_board(char board[64])
{
	printf("  a b c d e f g h\n");
	printf(" +---------------+\n");

	for (int row = 0; row < 8; row++) {
		printf("%d|", 9 - (row + 1));
		for (int col = 0; col < 8; col++) {
			printf("%c", board[row * 8 + col]);
			if (col != 7) printf(" ");
		}
		printf("|\n");
	}

	printf(" +---------------+\n");
}

int main(void)
{
	// run_tests();

	// return 0;

	srand(time(NULL));

	char board[64];

	char init_board[64] = {
		'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',
		'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
		'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
	};

	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0) putchar(10);
		printf("%3d", i);
	}
	putchar(10);


	printf("Move: `%s`\n", get_move_notation(CREATE_MOVE(52, 36, NORMAL, 0), board));

	return 0;
	bool is_enpassant = false;
	while (!is_enpassant)
	{
		memcpy(board, init_board, sizeof(char) * 64);
		Move valid_moves[256];
		uint8_t count;
		Castle castle = 0x3F;
		Move last_move = 0;
		Player player = WHITE;
		int move_counter = 0;

		for (int i = 0; i < 2*200; i++)
		{
			update_castle(board, &castle);
			generate_valid_moves(board, valid_moves, &count, player, castle, last_move);
			if (count == 0)
			{
				printf("GAME OVER!");
				exit(0);
			}
			last_move = valid_moves[rand() % count];
			make_move(board, last_move);

			printf("%c%d%c%d",
				(GET_COL(GET_FROM(last_move))) + 'a',   // Convert column to 'a' - 'h'
				9 - GET_ROW(GET_FROM(last_move)) - 1,            // Convert row to 1 - 8
				(GET_COL(GET_TO(last_move))) + 'a',     // Convert column to 'a' - 'h'
				9 - GET_ROW(GET_TO(last_move)) - 1             // Convert row to 1 - 8
			);
			if (GET_TYPE(last_move) == PROMOTION || GET_TYPE(last_move) == CASTLE)
			{
				printf("%c", GET_PROM(last_move) == KNIGHT ? 'N' : GET_PROM(last_move) == BISHOP ? 'B' : GET_PROM(last_move) == ROOK ? 'R' : 'Q');
			}

			// if (is_checkmate(board, WHITE, last_move) || is_checkmate(board, BLACK, last_move)
			// 	|| is_stalemate(board, WHITE, last_move) || is_stalemate(board, BLACK, last_move))
			// {
			// 	is_enpassant = true;
			// }

			move_counter++;
			if (move_counter % 2 == 0)
			{
				putchar(10);
			}
			else
			{
				putchar(' ');
			}
			// print_board(board);
			player = SWITCH_PLAYER(player);
		}
		// is_enpassant = true;
		printf("--------------------------------------\n");
	}


	return 0;
}
