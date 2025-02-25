#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// #include "chess.h"

#define CHESS_IMPLEMENTATION
#define USE_CAMEL_CASE
#include "chess.h"

extern void run_tests();

bool is_capture_move_better(char board[64], const Move move)
{
	// printf("from: %d\n", GET_FROM(move));
	char temp = board[GET_TO(move)];
	board[GET_TO(move)] = ' ';
	bool is_not_attacked = !is_attacked(board, GET_TO(move), IS_WHITE_PIECE(board[GET_FROM(move)]) ? BLACK : WHITE);
	board[GET_TO(move)] = temp;
	return is_not_attacked;
}

bool is_capture_move_better_ult(char board[64], const Move move)
{
	// printf("from: %d\n", GET_FROM(move));
	char temp = board[GET_TO(move)];
	board[GET_TO(move)] = ' ';
	bool is_not_attacked = !is_attacked(board, GET_TO(move), IS_WHITE_PIECE(board[GET_FROM(move)]) ? BLACK : WHITE);
	board[GET_TO(move)] = temp;
	return is_not_attacked && board[GET_TO(move)] != ' ';
}

int main(void)
{
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) putchar(10);
        printf("%3d", i);
    }
    putchar(10);


    run_tests();
	return 0;


    srand(time(NULL));
	char board[64] = {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
	};


//    printf("Attacked: %s\n", is_attacked_by_piece(board, 0, 'n') ? "true" : "false");

    return 0;
	Move valid_moves[MAX_VALID_MOVES];
	unsigned char count;

	COPY_BOARD(board, INITIAL_BOARD);
	generate_valid_moves(board, valid_moves, &count, BOTH, INITIAL_CASTLE, 0);
	print_valid_moves(valid_moves, count);

	return 0;

	// printf("Move: %s\n", MOVE_TO_STRING(CREATE_MOVE(0, 63, NORMAL, 0)));
	// printf("Move: `%s`\n", get_move_notation(CREATE_MOVE(52, 36, NORMAL, 0), board));

	bool run_games = true;
	while (run_games)
	{
		COPY_BOARD(board, INITIAL_BOARD);

		Move valid_moves[MAX_VALID_MOVES];
		uint8_t count;
		Castle castle = INITIAL_CASTLE;
		Move last_move = 0;
		Player player = WHITE;
		int move_counter = 0;

		for (int i = 0; i < 1024; i++)
		{
			update_castle(board, &castle);
			generate_valid_moves(board, valid_moves, &count, player, castle, last_move);
			if (count == 0)
			{
				printf("\n----------------------");
				printf("\nGAME OVER!");
				exit(0);
			}

			last_move = valid_moves[rand() % count];

			filter_moves(board, valid_moves, &count, is_capture_move_better_ult);

			if (count != 0)
			{
				last_move = valid_moves[rand() % count];
			}

			// char notation[16] = {0};
			// move_to_PGN(last_move, board, valid_moves, count, notation);
			// if (move_counter % 2 == 0) printf("%d. ", move_counter / 2 + 1);
			//
			// printf("%s ", notation);

			// if (move_counter % 2 == 0) printf("\n");

			// printf("`%s`", MOVE_TO_STRING(last_move));

			make_move(board, last_move);

			// printf("%c%d%c%d",
			// 	(GET_COL(GET_FROM(last_move))) + 'a',   // Convert column to 'a' - 'h'
			// 	9 - GET_ROW(GET_FROM(last_move)) - 1,            // Convert row to 1 - 8
			// 	(GET_COL(GET_TO(last_move))) + 'a',     // Convert column to 'a' - 'h'
			// 	9 - GET_ROW(GET_TO(last_move)) - 1             // Convert row to 1 - 8
			// );
			// if (GET_TYPE(last_move) == PROMOTION)
			// {
			// 	printf("%c", GET_PROM(last_move) == KNIGHT ? 'N' : GET_PROM(last_move) == BISHOP ? 'B' : GET_PROM(last_move) == ROOK ? 'R' : 'Q');
			// }
			printf("%s", MOVE_TO_STRING(last_move));

			move_counter++;
			putchar(move_counter % 2 == 0  ? 10 : ' ');

			// print_board(board);
			player = SWITCH_PLAYER(player);
		}
		// is_enpassant = true;
		run_games = false;
	}


	return 0;
}
