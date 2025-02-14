
/*
 * Test cases sourced from: https://www.chessprogramming.org/Perft_Results
 */

#include <stdint.h>
#include <time.h>

// #include "chess_header_only.h"

#include "chess.h"

#define RESET_COLOR    "\033[0m"
#define GREEN_COLOR    "\033[32m"
#define RED_COLOR      "\033[31m"
#define YELLOW_COLOR   "\033[33m"

void test_passed(const char *test_name, int expected, int actual, double time_taken) {
	printf("%s[PASS]%s %s\t- Expected: %-10d Got: %-10d Time: %.5f seconds\n", GREEN_COLOR, RESET_COLOR, test_name, expected, actual, time_taken);
}

void test_failed(const char *test_name, int expected, int actual, double time_taken) {
	printf("%s[FAIL]%s %s\t- Expected: %-10d Got: %-10d Time: %.5f seconds\n", RED_COLOR, RESET_COLOR, test_name, expected, actual, time_taken);
}

#define assert_equal(actual, expected) \
do { \
	clock_t start_time = clock(); \
	if ((actual) != (expected)) { \
		test_failed(__func__, (expected), (actual), ((double)(clock() - start_time)) / CLOCKS_PER_SEC); \
		/*return;*/ \
	} else { \
		test_passed(__func__, (expected), (actual), ((double)(clock() - start_time)) / CLOCKS_PER_SEC); \
	} \
} while (0)

uint64_t test_initial_position(const int depth, Player player)
{
	char board[64] = {
		'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',
		'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
		'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
	};

	return perft(board, depth, player, INITIAL_CASTLE, 0, true);
}

uint64_t test_double_checks(const int depth, Player player)
{
	char board[64] = {
		'r', ' ', ' ', ' ', 'k', ' ', ' ', 'r',
		'p', ' ', 'p', 'p', 'q', 'p', 'b', ' ',
		'b', 'n', ' ', ' ', 'p', 'n', 'p', ' ',
		' ', ' ', ' ', 'P', 'N', ' ', ' ', ' ',
		' ', 'p', ' ', ' ', 'P', ' ', ' ', ' ',
		' ', ' ', 'N', ' ', ' ', 'Q', ' ', 'p',
		'P', 'P', 'P', 'B', 'B', 'P', 'P', 'P',
		'R', ' ', ' ', ' ', 'K', ' ', ' ', 'R',
	};
	return perft(board, depth, player, INITIAL_CASTLE, 0, true);

}

uint64_t test_checks(const int depth, Player player)
{
	char board[64] = {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', 'p', ' ', ' ', ' ', ' ',
		'K', 'P', ' ', ' ', ' ', ' ', ' ', 'r',
		' ', 'R', ' ', ' ', ' ', 'p', ' ', 'k',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', 'P', ' ', 'P', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	};
	return perft(board, depth, player, INITIAL_CASTLE, 0, true);
}

uint64_t test_promo_white(const int depth, Player player)
{
	char board[64] = {
         'r', ' ', ' ', ' ', 'k', ' ', ' ', 'r',
         'P', 'p', 'p', 'p', ' ', 'p', 'p', 'p',
         ' ', 'b', ' ', ' ', ' ', 'n', 'b', 'N',
         'n', 'P', ' ', ' ', ' ', ' ', ' ', ' ',
         'B', 'B', 'P', ' ', 'P', ' ', ' ', ' ',
         'q', ' ', ' ', ' ', ' ', 'N', ' ', ' ',
         'P', 'p', ' ', 'P', ' ', ' ', 'P', 'P',
         'R', ' ', ' ', 'Q', ' ', 'R', 'K', ' ',
	};
	return perft(board, depth, player, INITIAL_CASTLE, 0, true);
}

uint64_t test_promo_black(const int depth, Player player)
{
	char board[64] = {
		'r', ' ', ' ', 'q', ' ', 'r', 'k', ' ',
		'p', 'P', ' ', 'p', ' ', ' ', 'p', 'p',
		'Q', ' ', ' ', ' ', ' ', 'n', ' ', ' ',
		'b', 'b', 'p', ' ', 'p', ' ', ' ', ' ',
		'N', 'p', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', 'B', ' ', ' ', ' ', 'N', 'B', 'n',
		'p', 'P', 'P', 'P', ' ', 'P', 'P', 'P',
		'R', ' ', ' ', ' ', 'K', ' ', ' ', 'R',
	};
	return perft(board, depth, player, INITIAL_CASTLE, 0, true);
}

uint64_t test_depth_3(const int depth, Player player)
{
	char board[64] = {
		'r', 'n', 'b', 'q', ' ', 'k', ' ', 'r',
		'p', 'p', ' ', 'P', 'b', 'p', 'p', 'p',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'B', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		'P', 'P', 'P', ' ', 'N', 'n', 'P', 'P',
		'R', 'N', 'B', 'Q', 'K', ' ', ' ', 'R',
	};
	return perft(board, depth, player, INITIAL_CASTLE, 0, true);
}


uint64_t test_alt(const int depth, Player player)
{
	char board[64] = {
		'r', ' ', ' ', ' ', ' ', 'r', 'k', ' ',
		' ', 'p', 'p', ' ', 'q', 'p', 'p', 'p',
		'p', ' ', 'n', 'p', ' ', 'n', ' ', ' ',
		' ', ' ', 'b', ' ', 'p', ' ', 'B', ' ',
		' ', ' ', 'B', ' ', 'P', ' ', 'b', ' ',
		'P', ' ', 'N', 'P', ' ', 'N', ' ', ' ',
		' ', 'P', 'P', ' ', 'Q', 'P', 'P', 'P',
		'R', ' ', ' ', ' ', ' ', 'R', 'K', ' ',
	};

	return perft(board, depth, player, INITIAL_CASTLE, 0, true);
}

uint64_t test_knight(const int depth, Player player)
{
	char board[64] = {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N',
	};

	return perft(board, depth, player, INITIAL_CASTLE, 0, false);
}

// Define unit tests
void test_perft_init_position() {
	assert_equal(test_initial_position(1, WHITE), 20);
	assert_equal(test_initial_position(2, WHITE), 400);
	assert_equal(test_initial_position(3, WHITE), 8902);
	assert_equal(test_initial_position(4, WHITE), 197281);
	assert_equal(test_initial_position(5, WHITE), 4865609);
#ifdef ALL_TESTS
	assert_equal(test_initial_position(6, WHITE), 119060324);
#endif
}

void test_perft_double_checks() {
	assert_equal(test_double_checks(1, WHITE), 48);
	assert_equal(test_double_checks(2, WHITE), 2039);
	assert_equal(test_double_checks(3, WHITE), 97862);
	assert_equal(test_double_checks(4, WHITE), 4085603);
#ifdef ALL_TESTS
	assert_equal(test_double_checks(5, WHITE), 193690690);
#endif
}

void test_perft_checks() {
	assert_equal(test_checks(1, WHITE), 14);
	assert_equal(test_checks(2, WHITE), 191);
	assert_equal(test_checks(3, WHITE), 2812);
	assert_equal(test_checks(4, WHITE), 43238);
	assert_equal(test_checks(5, WHITE), 674624);
#ifdef ALL_TESTS
	assert_equal(test_checks(6, WHITE), 11030083);
#endif
}

void test_perft_promos_white()
{
	assert_equal(test_promo_white(1, WHITE), 6);
	assert_equal(test_promo_white(2, WHITE), 264);
	assert_equal(test_promo_white(3, WHITE), 9467);
	assert_equal(test_promo_white(4, WHITE), 422333);
	assert_equal(test_promo_white(5, WHITE), 15833292);
// #ifdef ALL_TESTS
// 	assert_equal(test_promo_black(6, BLACK), 706045033);
// #endif
}

void test_perft_promos_black()
{
	assert_equal(test_promo_black(1, BLACK), 6);
	assert_equal(test_promo_black(2, BLACK), 264);
	assert_equal(test_promo_black(3, BLACK), 9467);
	assert_equal(test_promo_black(4, BLACK), 422333);
	assert_equal(test_promo_black(5, BLACK), 15833292);
// #ifdef ALL_TESTS
// 	assert_equal(test_promo_black(6, BLACK), 706045033);
// #endif
}

void test_perft_depth_3()
{
	assert_equal(test_depth_3(1, WHITE), 44);
	assert_equal(test_depth_3(2, WHITE), 1486);
	assert_equal(test_depth_3(3, WHITE), 62379);
	assert_equal(test_depth_3(4, WHITE), 2103487);
#ifdef ALL_TESTS
	assert_equal(test_depth_3(5, WHITE), 89941194);
#endif
}

void test_preft_alternative()
{
	assert_equal(test_alt(1, WHITE), 46);
	assert_equal(test_alt(2, WHITE), 2079);
	assert_equal(test_alt(3, WHITE), 89890);
	assert_equal(test_alt(4, WHITE), 3894594);
#ifdef ALL_TESTS
	assert_equal(test_alt(5, WHITE), 164075551);
#endif
}

void test_perft_knight()
{
	assert_equal(test_knight(1, WHITE),  2);
	assert_equal(test_knight(2, WHITE),  12);
	assert_equal(test_knight(3, WHITE),  64);
	assert_equal(test_knight(4, WHITE),  404);
	assert_equal(test_knight(5, WHITE),  2352);
	assert_equal(test_knight(6, WHITE),  14396);
	assert_equal(test_knight(7, WHITE),  85608);
	assert_equal(test_knight(8, WHITE),  517796);
	assert_equal(test_knight(9, WHITE),  3100928);
	assert_equal(test_knight(10, WHITE), 18678652);
#ifdef ALL_TESTS
	assert_equal(test_knight(11, WHITE), 112130536);
#endif
}

void run_tests() {
	// Run each test
	test_perft_init_position();  // Initial Position
	test_perft_double_checks();  // Position 2
	test_perft_checks();		 // Position 3

	test_perft_promos_white();   // Position 4 White
	test_perft_promos_black();	 // Position 4 Black

	test_perft_depth_3();		 // Position 5
	test_preft_alternative();	 // Position 6

	test_perft_knight();		 // Knight tests

	printf("Testing process finished.\n");
}
