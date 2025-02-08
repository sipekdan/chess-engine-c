//
// Created by Daniel on 1/31/2025.
//

#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

typedef uint16_t Move;
typedef uint8_t Square;
typedef uint8_t Castle;
typedef enum { BLACK, WHITE } Player;

/*
 * from square:  6 bits (2^6 = 64)
 * to square:    6 bits (2^6 = 64)
 * move type:    2 bits (2^2 =  4) 0 -> normal, 1 -> promotion, 2 -> castle, 3 -> en-passant
 * promo. piece: 2 bits (2^2 =  4) 0 -> knight, 1 -> bishop,    2 -> rook,   3 -> queen
 */

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

#define IS_WHITE_PIECE(piece) (isupper(piece))
#define IS_BLACK_PIECE(piece) (islower(piece))

#define SWITCH_PLAYER(player) ((player) == WHITE ? BLACK : WHITE)

#define COPY_BOARD(dest, src) memcpy((dest), (src), sizeof(char) * 64)

enum { NORMAL = 0, PROMOTION = 1, CASTLE = 2, EN_PASSANT = 3, };
enum { KNIGHT = 0, BISHOP = 1, ROOK = 2, QUEEN = 3 };

/**
 * Makes a move on the board.
 * @param board The chessboard represented as a 64-character array.
 * @param move The move to be applied.
 */
void make_move(char board[64], const Move move);

/**
 * Undoes a move on the board.
 * @param board The chessboard represented as a 64-character array.
 * @param move The move to be undone.
 * @param captured_piece The piece that was captured, if any. Use ' ' for non captured piece
 */
void undo_move(char board[64], const Move move, const char captured_piece);

/**
 * Updates castling rights based on the current board position.
 * Do this before each move generating (generate_valid_moves don't do this for you)
 * @param board The chessboard represented as a 64-character array.
 * @param castle Pointer to the castle rights variable.
 */
void update_castle(const char board[64], Castle* castle);

/**
 * Checks if a player is in check.
 * @param board The chessboard represented as a 64-character array.
 * @param player The player to check for check status.
 * @return True if the player is in check, otherwise false.
 */
bool is_in_check(const char board[64], const Player player);

/**
 * Determines if a player is in checkmate.
 * @param board The chessboard represented as a 64-character array.
 * @param player The player to check for checkmate.
 * @param last_move The last move played. (Only for en-passant)
 * @return True if the player is in checkmate, otherwise false.
 */
bool is_checkmate(char board[64], const Player player, const Move last_move);

/**
 * Determines if a player is in stalemate.
 * @param board The chessboard represented as a 64-character array.
 * @param player The player to check for stalemate.
 * @param last_move The last move played. (Only for en-passant)
 * @return True if the player is in stalemate, otherwise false.
 */
bool is_stalemate(char board[64], const Player player, const Move last_move);

/**
 * Generates all valid moves for a player.
 * @param board The chessboard represented as a 64-character array.
 * @param valid_moves Array to store valid moves.
 * @param count Pointer to store the count of valid moves.
 * @param player The player whose moves are being generated.
 * @param castle Castling rights.
 * @param last_move The last move played. (Only used for en-passant)
 */
void generate_valid_moves(char board[64], Move valid_moves[256], uint8_t* count, const Player player, const uint8_t castle, const uint16_t last_move);

/**
 * Adds a move to the list of valid moves. Used only in generate_valid_moves!
 * @param board The chessboard represented as a 64-character array.
 * @param valid_moves Array to store valid moves.
 * @param move The move to be added.
 * @param count Pointer to the count of valid moves.
 * @param player The player whose move is being added.
 */
void add_move(char board[64], Move valid_moves[256], const Move move, uint8_t* count, const Player player);

/**
 * Checks if a `square` is attacked on `board` by `player`.
 * @param board The chessboard represented as a 64-character array.
 * @param square The square to check.
 * @param player The player being attacked.
 * @return True if the square is attacked, otherwise false.
 */
bool is_attacked(const char board[64], const Square square, const Player player);

/**
 * Runs a performance test (Perft) for move generation.
 * @param board The chessboard represented as a 64-character array.
 * @param depth The depth to search.
 * @param player The current player.
 * @param castle Castling rights.
 * @param last_move The last move played.
 * @return The number of legal moves at the given depth.
 */
uint64_t perft(char board[64], const int depth, const Player player, const Castle castle, const Move last_move);

/**
 * Runs test cases for the chess engine.
 */
void run_tests();

// TODO: is_move_legal(const char[64], const Move);

// Not tested yet!
bool can_en_passant(const char board[64], const Player player, const Move last_move);
bool can_castle(const char board[64], const Player player, Castle* castle);

// In develop
bool is_check_move(char board[64], Move move);
bool is_attacked_by_piece(char board[64], const Square square, char piece);
bool is_capture_move(const char board[64], const Move move);
void filter_moves(char board[64], Move valid_moves[256], uint8_t *count, bool (*filter)(const char board[64], const Move move));

// Testing
#define INIT_COPY_BOARD(dest, src) \
	char dest[64];\
	COPY_BOARD(dest, src);

#define MOVE_TO_STRING(move) \
	({ \
		static char move_str[6]; \
		int len = snprintf(move_str, sizeof(move_str), "%c%d%c%d", \
			(GET_COL(GET_FROM(move))) + 'a',  /* Convert starting column to 'a' - 'h' */ \
			9 - GET_ROW(GET_FROM(move)) - 1,   /* Convert starting row to 1 - 8 */ \
			(GET_COL(GET_TO(move))) + 'a',    /* Convert ending column to 'a' - 'h' */ \
			9 - GET_ROW(GET_TO(move)) - 1     /* Convert ending row to 1 - 8 */ \
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



#endif //CHESS_H
