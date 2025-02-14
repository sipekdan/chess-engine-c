//
// Created by Daniel on 2/1/2025.
//

#include "chess_old.h"

CHESSDEF void make_move(char board[64], const Move move)
{
	const unsigned char from = GET_FROM(move);
	const unsigned char to = GET_TO(move);
	const char piece = board[from];

	switch (GET_TYPE(move))
	{
	case NORMAL:
		{
			board[to] = piece;
			board[from] = ' ';
		}
		break;

	case PROMOTION:
		{
			if (GET_PROM(move) > QUEEN)
			{
				printf("Invalid promotion piece index\n");
				return;
			}

			board[to] = IS_WHITE_PIECE(piece) ?
				(GET_PROM(move) == KNIGHT ? 'N' :
				 GET_PROM(move) == BISHOP ? 'B' :
				 GET_PROM(move) == ROOK ? 'R' : 'Q') :
				 GET_PROM(move) == KNIGHT ? 'n' :
				 GET_PROM(move) == BISHOP ? 'b' :
				 GET_PROM(move) == ROOK ? 'r' : 'q';

			board[from] = ' ';
		}
		break;

	case CASTLE:
		{
			if (piece == 'K' || piece == 'k')
			{
				const bool is_white = (piece == 'K');
				board[to] = piece;
				board[is_white ? 60 : 4] = ' ';
				board[is_white ? (to == 62 ? 61 : 59) : (to == 6 ? 5 : 3)] = is_white ? 'R' : 'r';
				board[is_white ? (to == 62 ? 63 : 56) : (to == 6 ? 7 : 0)] = ' ';
			}
		}
		break;

	case EN_PASSANT:
		{
			board[to] = piece;
			board[from] = ' ';
			board[to - (piece == 'P' ? -1 : 1) * 8] = ' ';
		}
		break;

	default:
		printf("Unreachable");
	}
}

CHESSDEF void undo_move(char board[64], const Move move, const char captured_piece)
{
	const unsigned char from = GET_FROM(move);
	const unsigned char to = GET_TO(move);
	const char moved_piece = board[to];

	switch (GET_TYPE(move))
	{
	case NORMAL:
		{
			board[from] = moved_piece;
			board[to] = captured_piece;
		}
		break;

	case PROMOTION:
		{
			board[to] = captured_piece;
			board[from] = IS_WHITE_PIECE(moved_piece) ? 'P' : 'p';
		}
		break;

	case CASTLE:
		{
			if (moved_piece == 'K' || moved_piece == 'k')
			{
				const bool is_white = (moved_piece == 'K');
				board[is_white ? 60 : 4] = moved_piece;
				board[is_white ? (to == 62 ? 63 : 56) : (to == 6 ? 7 : 0)] = is_white ? 'R' : 'r';
				board[to] = ' ';
				board[is_white ? (to == 62 ? 61 : 59) : (to == 6 ? 5 : 3)] = ' ';
			}
		}
		break;

	case EN_PASSANT:
		{
			board[to - (moved_piece == 'P' ? -1 : 1) * 8] = (moved_piece == 'P') ? 'p' : 'P';
			board[from] = moved_piece;
			board[to] = ' ';
		}
		break;

	default:
		printf("Unreachable");
		break;
	}
}

CHESSDEF void update_castle(const char board[64], Castle* castle)
{
	// Check Black Rooks
	if (board[0] != 'r') { *castle &= ~(1 << 4); } // Bit 4 (Black Rook 1)
	if (board[7] != 'r') { *castle &= ~(1 << 5); } // Bit 5 (Black Rook 2)

	// Check White Rooks
	if (board[56] != 'R') { *castle &= ~(1 << 1); } // Bit 1 (White Rook 1)
	if (board[63] != 'R') { *castle &= ~(1 << 2); } // Bit 2 (White Rook 2)

	// Check Black King
	if (board[4] != 'k') { *castle &= ~(1 << 3); } // Bit 3 (Black King)

	// Check White King
	if (board[60] != 'K') { *castle &= ~(1 << 0); } // Bit 0 (White King)
}

CHESSDEF bool is_attacked(const char board[64], const Square square, const Player player)
{
	if (!IS_VALID_SQUARE(square))
	{
		return false;
	}

	if ((IS_WHITE_PIECE(board[square]) && player == WHITE) || (IS_BLACK_PIECE(board[square]) && player == BLACK))
	{
		return false;
	}

	const char knight = (player == WHITE) ? 'N' : 'n';
	const char pawn   = (player == WHITE) ? 'P' : 'p';
	const char bishop = (player == WHITE) ? 'B' : 'b';
	const char rook   = (player == WHITE) ? 'R' : 'r';
	const char queen  = (player == WHITE) ? 'Q' : 'q';
	const char king   = (player == WHITE) ? 'K' : 'k';

	if ((IS_VALID_SQUARE(square + (player == BLACK ? -7 : 7)) &&
			ABS(GET_COL(square) - GET_COL(square + (player == BLACK ? -7 : 7))) == 1 &&
			board[square + (player == BLACK ? -7 : 7)] == pawn) ||
		(IS_VALID_SQUARE(square + (player == BLACK ? -9 : 9)) &&
			ABS(GET_COL(square) - GET_COL(square + (player == BLACK ? -9 : 9))) == 1 &&
			board[square + (player == BLACK ? -9 : 9)] == pawn))
	{
		return true;
	}

	const char knight_moves[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
	for (unsigned char i = 0; i < 8; i++)
	{
		const Square target_square = square + knight_moves[i];
		if (IS_VALID_SQUARE(target_square) &&
			ABS(GET_COL(target_square) - GET_COL(square)) <= 2 &&
			ABS(GET_ROW(target_square) - GET_ROW(square)) <= 2 &&
			board[target_square] == knight)
		{
			return true;
		}
	}


	const char directions[8] = {1, -1, 8, -8, 9, 7, -9, -7};
	for (unsigned char d = 0; d < 8; ++d)
	{
		for (unsigned char k = 1; k < 8; ++k)
		{
			const Square target_square = square + directions[d] * k;

			if (!IS_VALID_SQUARE(target_square)) break;

			const unsigned char row = GET_ROW(target_square);
			const unsigned char col = GET_COL(target_square);

			// Boundary checks for each direction
			if ((directions[d] == 1 && col == 0) || // Right edge, wrap around
				(directions[d] == -1 && col == 7) || // Left edge, wrap around
				(directions[d] == 8 && row == 0) || // Bottom edge, wrap around
				(directions[d] == -8 && row == 7) || // Top edge, wrap around
				(directions[d] == 9 && (col == 0 || row == 0)) || // Bottom-right
				(directions[d] == -9 && (col == 7 || row == 7)) || // Top-left
				(directions[d] == 7 && (col == 7 || row == 0)) || // Bottom-left
				(directions[d] == -7 && (col == 0 || row == 7))) // Top-right
			{
				break;
			}

			const char piece = board[target_square];
			if (piece != ' ')
			{
				if (piece == queen ||
					(d <= 3 && piece == rook) ||
					(d > 3 && piece == bishop) ||
					(piece == king && k == 1))
				{
					return true;
				}
				break;
			}
		}
	}
	return false;
}

CHESSDEF bool is_in_check(const char board[64], const Player player)
{
	const char king = player == WHITE ? 'K' : 'k';

	for (Square square = 0; square < 64; square++)
	{
		if (board[square] == king) return is_attacked(board, square, SWITCH_PLAYER(player));
	}
	return false;
}

CHESSDEF bool is_checkmate(char board[64], const Player player, const Move last_move)
{
	if (is_in_check(board, player))
	{
		Move valid_moves[256];
		unsigned char count;

		// Needs to use last move for en-passant (case where en-passant will save you from checkmate)
		// https://chess.stackexchange.com/questions/22006/en-passant-checkmate#:~:text=The%20answer%20in%20both%20cases%20is%20no%2C%20since,probably%20win%20the%20game%20via%20the%20clock%20%3A%29
		// Castle is not necessary because if the player is in check he can't castle

		generate_valid_moves(board, valid_moves, &count, player, 0, last_move);

		if (count == 0) return true;
	}
	return false;
}

CHESSDEF bool is_stalemate(char board[64], const Player player, const Move last_move)
{
	if (!is_in_check(board, player))
	{
		Move valid_moves[256];
		unsigned char count;

		// Needs to use last move for en-passant (case where en-passant is the only left move)
		// Castle is not needed because if the player's only move is castle, then he could move the rook also, so it won't be a stalemate
		// => Castle can't be only left move at any given position (hope so)

		generate_valid_moves(board, valid_moves, &count, player, 0, last_move);

		if (count == 0) return true;
	}
	return false;
}

CHESSDEF bool can_en_passant(const char board[64], const Player player, const Move last_move)
{
	for (Square square = 24; square < 40; square++)
	{
		if ((player == WHITE && board[square] == 'P') || (player == BLACK && board[square] == 'B'))
		{
			if (ABS(GET_FROM(last_move) - GET_TO(last_move)) == 16 && board[GET_TO(last_move)] == (player == WHITE ? 'p' : 'P'))
			{
				if (GET_ROW(square) == (player == WHITE ? 3 : 4) && ABS(GET_COL(square) - GET_COL(GET_TO(last_move))) == 1)
				{
					return true;
				}
			}
		}
	}
	return false;
}

CHESSDEF bool can_castle(const char board[64], const Player player, Castle* castle)
{
	update_castle(board, castle);
	return player == WHITE ? GET_CASTLE_WK(*castle) && (GET_CASTLE_WR1(*castle) || GET_CASTLE_WR2(*castle))
						   : GET_CASTLE_BK(*castle) && (GET_CASTLE_BR1(*castle) || GET_CASTLE_BR2(*castle));
}

static void add_move(char board[64], Move valid_moves[256], const Move move, unsigned char* count, const Player player)
{
	const char captured_piece = board[GET_TO(move)];
	make_move(board, move);

	if (!is_in_check(board, player))
	{
		valid_moves[(*count)++] = move;
	}

	undo_move(board, move, captured_piece);
}
CHESSDEF void generate_valid_moves(char board[64], Move valid_moves[256], unsigned char* count, const Player player, Castle castle, const Move last_move)
{
	*count = 0;
	for (Square square = 0; square < 64; ++square)
	{
		const char piece = board[square];

		if ((player == WHITE && IS_WHITE_PIECE(piece)) || (player == BLACK && IS_BLACK_PIECE(piece)))
		{
			switch (piece)
			{
			case 'p':
			case 'P':
				{
					const char direction = piece == 'P' ? -8 : 8;
					const Square target_square = square + direction;

					// Move forward
					if (IS_VALID_SQUARE(target_square) && board[target_square] == ' ')
					{
						if ((piece == 'P' && GET_ROW(target_square) == 0) || (piece == 'p' && GET_ROW(target_square) ==
							7))
						{
							for (unsigned char promotion_piece = 0; promotion_piece < 4; ++promotion_piece)
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, PROMOTION, promotion_piece), count, player);
							}
						}
						else
						{
							add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count, player);
						}

						// Double move from starting position
						if ((piece == 'P' && GET_ROW(square) == 6) || (piece == 'p' && GET_ROW(square) == 1))
						{
							const Square double_target = square + 2 * direction;
							if (board[double_target] == ' ')
							{
								add_move(board, valid_moves, CREATE_MOVE(square, double_target, NORMAL, 0), count,
								         player);
							}
						}
					}

					// Captures
					for (unsigned char i = 0; i < 2; i++)
					{
						const Square capture_square = square + (i == 0 ? direction - 1 : direction + 1);

						if (IS_VALID_SQUARE(capture_square) &&
							ABS(GET_COL(capture_square) - GET_COL(square)) == 1 &&
							((player == WHITE && IS_BLACK_PIECE(board[capture_square])) ||
								(player == BLACK && IS_WHITE_PIECE(board[capture_square]))))
						{
							if ((piece == 'P' && GET_ROW(capture_square) == 0) || (piece == 'p' && GET_ROW(
								capture_square) == 7))
							{
								for (unsigned char promotion_piece = 0; promotion_piece < 4; ++promotion_piece)
								{
									add_move(board, valid_moves, CREATE_MOVE(square, capture_square, PROMOTION, promotion_piece), count, player);
								}

							}
							else
							{
								add_move(board, valid_moves, CREATE_MOVE(square, capture_square, NORMAL, 0), count,
								         player);
							}
						}
					}

					// En passant
					if (last_move != 0)
					{
						if (ABS(GET_FROM(last_move) - GET_TO(last_move)) == 16 && board[GET_TO(last_move)] == (
							player == WHITE ? 'p' : 'P'))
						{
							if (GET_ROW(square) == (player == WHITE ? 3 : 4) && ABS(
								GET_COL(square) - GET_COL(GET_TO(last_move))) == 1)
							{
								add_move(board, valid_moves,
								         CREATE_MOVE(square, GET_TO(last_move) + direction, EN_PASSANT, 0), count,
								         player);
							}
						}
					}
				}
				break;

			case 'n':
			case 'N':
				{
					const char knight_moves[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
					for (unsigned char i = 0; i < 8; i++)
					{
						const Square target_square = square + knight_moves[i];
						if (IS_VALID_SQUARE(target_square) &&
							ABS(GET_COL(target_square) - GET_COL(square)) <= 2 &&
							ABS(GET_ROW(target_square) - GET_ROW(square)) <= 2 &&
							(board[target_square] == ' ' || (IS_BLACK_PIECE(board[target_square]) && player == WHITE) ||
								(IS_WHITE_PIECE(board[target_square]) && player == BLACK)))
						{
							add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count, player);
						}
					}
				}
				break;

			case 'b':
			case 'B':
				{
					const char bishop_moves[4] = {-9, -7, 7, 9};
					for (unsigned char i = 0; i < 4; i++)
					{
						for (unsigned char step = 1; step < 8; step++)
						{
							const Square target_square = square + bishop_moves[i] * step;
							if (!IS_VALID_SQUARE(target_square)) break;

							// Ensure new_pos is on the same diagonal
							if (ABS(GET_COL(target_square) - GET_COL(square)) != ABS(
								GET_ROW(target_square) - GET_ROW(square)))
								break;

							if (board[target_square] == ' ')
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
							}
							else if ((player == WHITE && IS_BLACK_PIECE(board[target_square])) || (player == BLACK && IS_WHITE_PIECE(
								board[target_square])))
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				break;

			case 'r':
			case 'R':
				{
					const char rook_moves[4] = {-8, 8, -1, 1}; // Up, Down, Left, Right

					for (unsigned char i = 0; i < 4; i++)
					{
						for (unsigned char step = 1; step < 8; step++)
						{
							const Square target_square = square + rook_moves[i] * step;

							// Ensure target_square is valid
							if (!IS_VALID_SQUARE(target_square)) break;

							// Ensure rook doesn't wrap around the board
							if ((i > 1) && GET_ROW(target_square) != GET_ROW(square)) break;

							if (board[target_square] == ' ')
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
							}
							else if ((player == WHITE && IS_BLACK_PIECE(board[target_square])) ||
								(player == BLACK && IS_WHITE_PIECE(board[target_square])))
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				break;

			case 'q':
			case 'Q':
				{
					const char queen_moves[8] = {-8, 8, -1, 1, -9, -7, 7, 9};

					for (unsigned char i = 0; i < 8; i++)
					{
						for (unsigned char step = 1; step < 8; step++)
						{
							const Square target_square = square + queen_moves[i] * step;

							// Ensure target_square is valid
							if (!IS_VALID_SQUARE(target_square)) break;

							// Prevent wrapping around board on horizontal moves
							if ((queen_moves[i] == -1 || queen_moves[i] == 1) && GET_ROW(target_square) != GET_ROW(
								square))
								break;

							// Ensure diagonal moves stay on the same diagonal
							if ((queen_moves[i] == -9 || queen_moves[i] == -7 || queen_moves[i] == 7 || queen_moves[i]
									== 9) &&
								ABS(GET_COL(target_square) - GET_COL(square)) != ABS(
									GET_ROW(target_square) - GET_ROW(square)))
								break;

							if (board[target_square] == ' ')
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
							}
							else if ((player == WHITE && IS_BLACK_PIECE(board[target_square])) ||
								(player == BLACK && IS_WHITE_PIECE(board[target_square])))
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
								break;
							}
							else
							{
								break;
							}
						}
					}
				}
				break;

			case 'k':
			case 'K':
				{
					const char king_moves[8] = {-1, 1, -8, 8, -9, -7, 7, 9};
					for (unsigned char i = 0; i < 8; i++)
					{
						const Square target_square = square + king_moves[i];
						if (IS_VALID_SQUARE(target_square) && ABS(GET_COL(target_square) - GET_COL(square)) <= 1)
						{
							if (board[target_square] == ' ' ||
								(IS_BLACK_PIECE(board[target_square]) && player == WHITE) ||
								(IS_WHITE_PIECE(board[target_square]) && player == BLACK))
							{
								add_move(board, valid_moves, CREATE_MOVE(square, target_square, NORMAL, 0), count,
								         player);
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}

	// Castling moves
	if (player == WHITE)
	{
		if (!is_in_check(board, player))
		{
			if (board[61] == ' ' && board[62] == ' ' && !is_attacked(board, 61, SWITCH_PLAYER(player)) && !
				is_attacked(board, 62, SWITCH_PLAYER(player)) && GET_CASTLE_WK(castle) && GET_CASTLE_WR2(castle))
			{
				add_move(board, valid_moves, CREATE_MOVE(60, 62, CASTLE, 0), count, player);
			}

			if (board[57] == ' ' && board[58] == ' ' && board[59] == ' ' && !is_attacked(
					board, 58, SWITCH_PLAYER(player)) && !
				is_attacked(board, 59, SWITCH_PLAYER(player)) && GET_CASTLE_WK(castle) && GET_CASTLE_WR1(castle))
			{
				add_move(board, valid_moves, CREATE_MOVE(60, 58, CASTLE, 0), count, player);
			}
		}
	}
	else
	{
		if (!is_in_check(board, player))
		{
			if (board[5] == ' ' && board[6] == ' ' && !is_attacked(board, 5, SWITCH_PLAYER(player)) && !
				is_attacked(board, 6, SWITCH_PLAYER(player)) && GET_CASTLE_BK(castle) && GET_CASTLE_BR2(castle))
			{
				add_move(board, valid_moves, CREATE_MOVE(4, 6, CASTLE, 0), count, player);
			}

			if (board[3] == ' ' && board[2] == ' ' && board[1] == ' ' && !is_attacked(board, 3, SWITCH_PLAYER(player))
				&& !
				is_attacked(board, 2, SWITCH_PLAYER(player)) && GET_CASTLE_BK(castle) && GET_CASTLE_BR1(castle))
			{
				add_move(board, valid_moves, CREATE_MOVE(4, 2, CASTLE, 0), count, player);
			}
		}
	}
}


CHESSDEF unsigned long long perft(char board[64], const int depth, const Player player, const Castle castle, const Move last_move, const bool switch_player)
{
	if (depth == 0) return 1;

	Move valid_moves[256];
	unsigned char move_count = 0;
	unsigned long long total_moves = 0;

	Castle new_castle = castle;

	update_castle(board, &new_castle);
	generate_valid_moves(board, valid_moves, &move_count, player, new_castle, last_move);

	for (int i = 0; i < move_count; i++)
	{
		char new_board[64] = {0};
		COPY_BOARD(new_board, board);

		make_move(new_board, valid_moves[i]);

		const unsigned long long level_total = perft(new_board, depth - 1, switch_player ? SWITCH_PLAYER(player) : player, new_castle, valid_moves[i], switch_player);

		total_moves += level_total;
	}

	return total_moves;
}

CHESSDEF bool is_attacked_by_piece(char board[64], const Square square, char piece)
{
	if (!IS_VALID_SQUARE(square))
	{
		return false;
	}

	switch (piece)
	{
	case 'p':
	case 'P':
		if ((IS_VALID_SQUARE(square + (IS_BLACK_PIECE(piece) ? -7 : 7)) &&
				ABS(GET_COL(square) - GET_COL(square + (IS_BLACK_PIECE(piece) ? -7 : 7))) == 1 &&
				board[square + (IS_BLACK_PIECE(piece) ? -7 : 7)] == piece) ||
			(IS_VALID_SQUARE(square + (IS_BLACK_PIECE(piece) ? -9 : 9)) &&
				ABS(GET_COL(square) - GET_COL(square + (IS_BLACK_PIECE(piece) ? -9 : 9))) == 1 &&
				board[square + (IS_BLACK_PIECE(piece) ? -9 : 9)] == piece))
		{
			return true;
		}
		break;

	case 'n':
	case 'N':
		const char knight_moves[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
		for (unsigned char i = 0; i < 8; i++)
		{
			const Square target_square = square + knight_moves[i];
			if (IS_VALID_SQUARE(target_square) &&
				ABS(GET_COL(target_square) - GET_COL(square)) <= 2 &&
				ABS(GET_ROW(target_square) - GET_ROW(square)) <= 2 &&
				board[target_square] == piece)
			{
				return true;
			}
		}

		break;

	case 'b':
	case 'B':
		const char bishop_directions[4] = {9, -9, 7, -7};
		for (unsigned char i = 0; i < 4; ++i)
		{
			for (unsigned char k = 1; k < 8; ++k)
			{
				const Square target_square = square + bishop_directions[i] * k;
				if (!IS_VALID_SQUARE(target_square)) break;

				const unsigned char row = GET_ROW(target_square);
				const unsigned char col = GET_COL(target_square);

				// Check boundaries for bishop moves
				if ((bishop_directions[i] == 9 && (col == 0 || row == 0)) ||
					(bishop_directions[i] == -9 && (col == 7 || row == 7)) ||
					(bishop_directions[i] == 7 && (col == 7 || row == 0)) ||
					(bishop_directions[i] == -7 && (col == 0 || row == 7)))
				{
					break;
				}

				const char piece_at_target = board[target_square];
				if (piece_at_target != ' ')
				{
					if (piece_at_target == piece)
					{
						return true;
					}
					break;
				}
			}
		}
		break;

	case 'r':
	case 'R':
		const char rook_directions[4] = {1, -1, 8, -8};
		for (unsigned char i = 0; i < 4; ++i)
		{
			for (unsigned char k = 1; k < 8; ++k)
			{
				const Square target_square = square + rook_directions[i] * k;
				if (!IS_VALID_SQUARE(target_square)) break;

				const unsigned char row = GET_ROW(target_square);
				const unsigned char col = GET_COL(target_square);

				// Boundary checks for rook moves
				if ((rook_directions[i] == 1 && col == 0) ||
					(rook_directions[i] == -1 && col == 7) ||
					(rook_directions[i] == 8 && row == 0) ||
					(rook_directions[i] == -8 && row == 7))
				{
					break;
				}

				const char piece_at_target = board[target_square];
				if (piece_at_target != ' ')
				{
					if (piece_at_target == piece)
					{
						return true;
					}
					break;
				}
			}
		}
		break;

	case 'q':
	case 'Q':
		if (is_attacked_by_piece(board, square, piece == 'q' ? 'r' : 'R')) return true;
		if (is_attacked_by_piece(board, square, piece == 'Q' ? 'b' : 'B')) return true;
		break;

	case 'k':
	case 'K':
		const char king_moves[8] = {-9, -8, -7, -1, 1, 7, 8, 9};
		for (unsigned char i = 0; i < 8; i++)
		{
			const Square target_square = square + king_moves[i];

			if (!IS_VALID_SQUARE(target_square) ||
				(ABS(GET_COL(square) - GET_COL(target_square)) > 1 || ABS(GET_ROW(square) - GET_ROW(target_square)) > 1))
			{
				continue;
			}

			if (board[target_square] == piece)
			{
				return true;
			}
		}
		break;

	default:
		printf("Unreachable\n");
		break;
	}

	return false;
}


CHESSDEF bool is_check_move(char board[64], Move move)
{
	char temp_board[64];
	COPY_BOARD(temp_board, board);
	make_move(temp_board, move);

	return is_in_check(temp_board, WHITE) || is_in_check(temp_board, BLACK);
}

CHESSDEF bool is_capture_move(const char board[64], const Move move)
{
	return board[GET_TO(move)] != ' ' || GET_TYPE(move) == EN_PASSANT;
}

CHESSDEF void filter_moves(char board[64], Move valid_moves[256], unsigned char *count, bool (*filter)(const char board[64], const Move move))
{
	unsigned char write_index = 0;
	for (unsigned char read_index = 0; read_index < *count; read_index++)
	{
		if (filter(board, valid_moves[read_index]))
		{
			valid_moves[write_index++] = valid_moves[read_index];
		}
	}
	*count = write_index;
}

// CHESSDEF void move_to_PGN(Move move, char board[64], Move valid_moves[256], unsigned char count, char *dest)
// {
// 	char *notation = &dest[0];
// 	size_t notation_length = 0;
//
// 	char piece = board[GET_FROM(move)];
//
// 	if (GET_TYPE(move) == CASTLE)
// 	{
// 		// TODO: Castle
// 		return;
// 	}
//
// 	if (piece != 'p' && piece != 'P')
// 	{
// 		notation[notation_length++] = (char)(piece - 32);
// 	}
//
// 	// 2 or more piece on same square move
// 	bool needs_file = false, needs_rank = false;
// 	for (unsigned char i = 0; i < count; i++) {
// 		const Move temp_move = valid_moves[i];
// 		if (i != GET_FROM(temp_move) && board[GET_FROM(temp_move)] == piece) {
// 			if (GET_ROW(move) == GET_ROW(temp_move))
// 			{
// 				needs_rank = true;
// 			} else if (GET_COL(move) == GET_COL(temp_move))
// 			{
// 				needs_file = true;
// 			}
// 		}
// 	}
//
// 	if (needs_file) notation[notation_length++] = GET_COL(GET_FROM(move)) + 'a';
// 	if (needs_rank) notation[notation_length++] = '1' + GET_ROW(GET_FROM(move));
//
// 	// Capture
// 	if (board[GET_TO(move)] != ' ')
// 	{
// 		notation[notation_length++] = 'x';
// 	}
//
// 	// Destination
// 	notation[notation_length++] = GET_COL(GET_TO(move)) + 'a';
// 	notation[notation_length++] = (char)(9 - GET_ROW(GET_TO(move)) - 1);
//
// 	if (GET_TYPE(move) == PROMOTION)
// 	{
// 		// TODO: promotion
// 	}
//
// 	notation[notation_length] = '\0';
// }

#include <ctype.h>
#include <string.h>

/*
 * Use https://lichess.org/analysis for test, paste it to the PGN input
 */
CHESSDEF void move_to_PGN(Move move, char board[64], Move valid_moves[256], unsigned char count, char *dest)
{
    char *notation = &dest[0];
    size_t notation_length = 0;

    char piece = board[GET_FROM(move)];

    if (GET_TYPE(move) == CASTLE)
    {
        if (GET_TO(move) == 62 || GET_TO(move) == 6) {
            strcpy(dest, "O-O");
        } else if (GET_TO(move) == 58 || GET_TO(move) == 2) {
            strcpy(dest, "O-O-O");
        }
        return;
    }

    if (piece != 'p' && piece != 'P')
    {
        notation[notation_length++] = (char)(toupper(piece));
    }

    // Disambiguation: Check if another piece of the same type can move to the same square
    bool needs_file = false, needs_rank = false;
    for (unsigned char i = 0; i < count; i++) {
        Move temp_move = valid_moves[i];

        if (move == temp_move) continue;  // Skip self
        if (GET_TO(temp_move) == GET_TO(move) && board[GET_FROM(temp_move)] == piece)
        {
            if (GET_ROW(GET_FROM(move)) == GET_ROW(GET_FROM(temp_move)))
            {
                needs_file = true;
            }
            if (GET_COL(GET_FROM(move)) == GET_COL(GET_FROM(temp_move)))
            {
                needs_rank = true;
            }
        }
    }

    // if (needs_file) notation[notation_length++] = GET_COL(GET_FROM(move)) + 'a' - 1;
    // if (needs_rank) notation[notation_length++] = '1' + GET_ROW(GET_FROM(move));

	notation[notation_length++] = GET_COL(GET_FROM(move)) + 'a';
	notation[notation_length++] = '0' + 8 - GET_ROW(GET_FROM(move));

	// Capture Handling
    // if (board[GET_TO(move)] != ' ' || GET_TYPE(move) == EN_PASSANT)
    // {
    //     if (piece == 'p' || piece == 'P')
    //     {
    //         notation[notation_length++] = GET_COL(GET_FROM(move)) + 'a';  // Pawn capture includes file
    //     }
    //     notation[notation_length++] = 'x';
    // }

    // Destination Square
    notation[notation_length++] = GET_COL(GET_TO(move)) + 'a';
    notation[notation_length++] = '0' + 8 - GET_ROW(GET_TO(move));

    // Promotion Handling
    if (GET_TYPE(move) == PROMOTION)
    {
        notation[notation_length++] = '=';
    	char promoted_piece;
    	switch (GET_PROM(move)) {
    		case KNIGHT: promoted_piece = 'N'; break;
    		case BISHOP: promoted_piece = 'B'; break;
    		case ROOK:   promoted_piece = 'R'; break;
    		case QUEEN:  promoted_piece = 'Q'; break;
    		default:     promoted_piece = '\0'; // Shouldn't happen
    	}

    	if (promoted_piece) {
    		notation[notation_length++] = promoted_piece;
    	}
    }

    notation[notation_length] = '\0';
}
