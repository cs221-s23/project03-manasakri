#define BOARD_SZ 3
#define EMPTY '_'
#include <stdbool.h>

typedef char  board_t[5][5] ;

struct move_position {
    int row ;
    int col ;
};

// board related
bool is_board_empty(board_t b) ;
void init_board(board_t board, char **argv);
void undo_move(board_t board, struct move_position movep);
void print_board(board_t board);
int  check_board(board_t board);

// move related
int get_possible_moves_count(board_t b);
struct move_position *  get_possible_moves(board_t b, int count);
struct move_position    find_best_move(board_t b);
void update_board(board_t board, struct move_position, char player);
struct move_position    get_next_move_from_player();


// state
bool is_terminal_state(board_t b);

void print_result (int result);
