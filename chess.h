//
// Created by Daniel on 1/31/2025.
//

#ifndef CHESS_H
#define CHESS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHESSDEF
#ifdef CHESS_STATIC
#define CHESSDEF static
#else
#define CHESSDEF extern
#endif
#endif

#include <stdio.h>
#include <stdbool.h>

typedef unsigned short Move;
typedef unsigned char Square;
typedef unsigned char Castle;
typedef enum { BLACK, WHITE } Player;

/*
 * from square:  6 bits (2^6 = 64)
 * to square:    6 bits (2^6 = 64)
 * move type:    2 bits (2^2 =  4) 0 -> normal, 1 -> promotion, 2 -> castle, 3 -> en-passant
 * promo. piece: 2 bits (2^2 =  4) 0 -> knight, 1 -> bishop,    2 -> rook,   3 -> queen
 */

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define CREATE_MOVE(from, to, type, promotion) (((from) << 10) | ((to) << 4) | ((type) << 2) | (promotion))

#define GET_FROM(move) (((move) >> 10) & 0x3F)
#define GET_TO(move)   (((move) >> 4)  & 0x3f)
#define GET_TYPE(move) (((move) >> 2)  & 0x3) // 0 - normal, 1 - promotion, 2 - castle, 3 - en-passant
#define GET_PROM(move) (((move) >> 0)  & 0x3) // 0 - knight, 1 - bishop, 2 - rook, 3 - queen

#define GET_CASTLE_WK(castle)  (((castle) >> 0) & 0x01)
#define GET_CASTLE_WR1(castle) (((castle) >> 1) & 0x01)
#define GET_CASTLE_WR2(castle) (((castle) >> 2) & 0x01)
#define GET_CASTLE_BK(castle)  (((castle) >> 3) & 0x01)
#define GET_CASTLE_BR1(castle) (((castle) >> 4) & 0x01)
#define GET_CASTLE_BR2(castle) (((castle) >> 5) & 0x01)

#define INITIAL_CASTLE 0x3F // 0b00111111

#define GET_ROW(square) ((square) >> 3) // square / 8
#define GET_COL(square) ((square) & 7)  // square % 8

#define IS_VALID_SQUARE(square) ((square >= 0) && (square < 64))

#define IS_WHITE_PIECE(piece) ((piece) >= 'A' && (piece) <= 'Z') // (isupper(piece))
#define IS_BLACK_PIECE(piece) ((piece) >= 'a' && (piece) <= 'z') // (islower(piece))

#define SWITCH_PLAYER(player) ((player) == WHITE ? BLACK : WHITE)

#define COPY_BOARD(dest, src) \
	for (int i = 0; i < 64; ++i) { \
		(dest)[i] = (src)[i]; \
	}

// Testing
#define INIT_COPY_BOARD(dest, src) \
	char (dest)[64];\
	COPY_BOARD(dest, src);

#define MOVE_TO_STRING(move) \
	({ \
		static char move_str[6]; \
		int len = snprintf(move_str, sizeof(move_str), "%c%d%c%d", \
			(GET_COL(GET_FROM(move))) + 'a',  /* Convert starting column to 'a' - 'h' */ \
			8 - GET_ROW(GET_FROM(move)),   /* Convert starting row to 1 - 8 */ \
			(GET_COL(GET_TO(move))) + 'a',    /* Convert ending column to 'a' - 'h' */ \
			8 - GET_ROW(GET_TO(move))     /* Convert ending row to 1 - 8 */ \
		); \
		if (GET_TYPE(move) == PROMOTION) { \
			char prom_piece = (GET_PROM(move) == KNIGHT) ? 'N' : \
			(GET_PROM(move) == BISHOP) ? 'B' : \
			(GET_PROM(move) == ROOK) ? 'R' : 'Q'; \
			move_str[len++] = prom_piece; \
			move_str[len] = '\0'; \
		} \
		move_str; \
	})

enum { NORMAL = 0, PROMOTION = 1, CASTLE = 2, EN_PASSANT = 3, };
enum { KNIGHT = 0, BISHOP = 1, ROOK = 2, QUEEN = 3 };

CHESSDEF void make_move(char board[64], const Move move);
CHESSDEF void undo_move(char board[64], const Move move, const char captured_piece);
CHESSDEF void update_castle(const char board[64], Castle* castle);
CHESSDEF bool is_in_check(const char board[64], const Player player);
CHESSDEF bool is_checkmate(char board[64], const Player player, const Move last_move);
CHESSDEF bool is_stalemate(char board[64], const Player player, const Move last_move);
CHESSDEF void generate_valid_moves(char board[64], Move valid_moves[256], unsigned char* count, const Player player, const unsigned char castle, const Move last_move);
CHESSDEF bool is_attacked(const char board[64], const Square square, const Player player);
CHESSDEF unsigned long long perft(char board[64], const int depth, const Player player, const Castle castle, const Move last_move, const bool switch_player);

void run_tests();

// TODO: is_move_legal(const char[64], const Move);

// Not tested yet!
CHESSDEF bool can_en_passant(const char board[64], const Player player, const Move last_move);
CHESSDEF bool can_castle(const char board[64], const Player player, Castle* castle);

// In develop
CHESSDEF bool is_check_move(char board[64], Move move);
CHESSDEF bool is_attacked_by_piece(char board[64], const Square square, char piece);
CHESSDEF bool is_capture_move(const char board[64], const Move move);
CHESSDEF void filter_moves(char board[64], Move valid_moves[256], unsigned char *count, bool (*filter)(const char board[64], const Move move));

CHESSDEF void move_to_PGN(Move move, char board[64], Move valid_moves[256], unsigned char count, char *dest);

#ifdef __cplusplus
}
#endif

#endif //CHESS_H
