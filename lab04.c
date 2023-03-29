#include <stdio.h>
#include <string.h>
#include "lab04.h"
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#define DEBUG 1

extern int board_size ;

int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}
int min(int num1, int num2)
{
    return (num1 > num2 ) ? num2 : num1;
}

// board related
bool is_board_empty(board_t b) {
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if ( b[i][j] != '_' ) return false;
        }
    }
    return true;
}
void init_board(board_t b, char **argv){

    int i, j;

    int k = 0;

    for (i = 0; i < board_size; i++) {
        for (j = 0; j < board_size; j++) {
            b[i][j] = *argv[i+j+k];
        }
        k = k + board_size-1;
    }
}

void undo_move(board_t board, struct move_position movep) {
    board[movep.row][movep.col] = '_';
}

void print_board(board_t b){

    int i, j;


    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            printf(" %c", b[i][j]);
            if (j != board_size-1) printf(" |");
        }

        if (i != board_size-1) {
            printf("\n");
            for (int k = 0; k < board_size; k++) {
                printf("---");
                if (k != board_size - 1) printf("+");
            }
        }
        printf("\n");
    }

}

void update_board(board_t board, struct move_position movep, char  player) {

    board[movep.row][movep.col] = (char) player;
}

bool is_player_win(board_t b, char player) {

    bool win = false;
    for (int i = 0; i < board_size; i++) {
        win = true;
        for (int j = 0; j < board_size; j++) {

            if ( b[i][j] !=  player)  {
                win = false;
            }
        }
        if ( win == true ) return 1 ;
    }


    win = false ;
    for (int i = 0; i < board_size; i++) {
        win = true ;
        for (int j = 0; j < board_size; j++) {

            if ( b[j][i] !=  player)  {
                win = false;
            }
        }
        if ( win == true ) return 1 ;
    }



    win = true;
    for (int i = 0; i < board_size; i++) {
        if ( b[i][i] !=  player)  {
            win = false;

        }
    }
    if ( win == true ) return 1;



    win = true ;
    for (int i = 0; i < board_size; i++) {
        if ( b[i][3-1-i] != player)  {
            win = false ;

        }
    }
    if ( win == true ) return 1;

    return 0;

}

int check_board(board_t board){

    if ( is_player_win(board, 'X') ) return 1;
    if ( is_player_win(board, 'O') ) return -1;
    return 0;


}


// move related
int get_possible_moves_count(board_t b) {

    int i = 0;
    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size; col++) {
            //printf("\n%d %d", b[row][col], '_');
            if (b[row][col] == '_') {
                i = i + 1;
            }
        }
    }
    return i;
}

struct move_position* get_possible_moves(board_t b, int count) {

    struct move_position *items = malloc(  sizeof (struct move_position) *count );

    int i=0 ;
    for (int row=0; row< board_size ;row ++) {
        for (int col = 0; col < board_size; col++) {
            if (b[row][col] == '_') {
                items[i].row = row;
                items[i].col = col;
                i = i + 1;
            }
        }
    }
    return items;

}

struct move_position get_random_position(board_t b) {
    while (1) {

        int row = rand() % 3;
        int col = rand() % 3;

        if (b[row][col] == '_') {
            struct move_position random_pos;
            random_pos.row = row;
            random_pos.col = col;
            return random_pos;
        }
    }
}

int minimax(board_t b, char player, int depth ) {

    int score = 0;
    
    if ( is_terminal_state(b) ) {
        int state = check_board(b);
        if (state ==0) return 0;
        if (state == -1 && player =='O') return 10;
        return -10;

    }

    if ( player == 'X') {

        score = 0;
        int best_score = 0;
        struct move_position best_move_position;

        int moves_count = get_possible_moves_count(b);
        struct move_position *possible_move = get_possible_moves(b,moves_count);

        best_move_position = possible_move[0];
        int i = 0;
        while (i < moves_count) {
            struct move_position movep = possible_move[i];
            update_board(b,movep, 'O');
            score = max(score, minimax(b, 'O', depth));
            if ( score > best_score) {
                best_score = score ;
                best_move_position = movep;
            }
            i = i + 1;
            undo_move(b,movep);
        }
    } else {

        score = 0;
        int best_score = 0;
        struct move_position best_move_position;

        int moves_count = get_possible_moves_count(b);
        struct move_position *possible_move = get_possible_moves(b,moves_count);

        best_move_position = possible_move[0];
        int i = 0 ;
        while (i < moves_count) {
            struct move_position movep = possible_move[i];
            update_board(b,movep, 'X');
            score = min(score, minimax(b, 'X', depth));
            if ( score < best_score) {
                best_score = score;
                best_move_position = movep;
            }
            i = i + 1;
            undo_move(b,movep);
        }
    }
    return score;
}

struct move_position find_best_move(board_t b) {

    struct move_position movep;

    int score = 0;
    int best_score = -INT_MAX;
    struct move_position best_move_position;

    int moves_count = get_possible_moves_count(b);
    struct move_position *possible_move = get_possible_moves(b,moves_count);

    int i = 0 ;
    while (i < moves_count) {
        struct move_position movep = possible_move[i];

        update_board(b,movep, 'O');

        score = minimax(b, 'O', 0);

        if ( score > best_score) {
            best_score = score ;
            best_move_position = movep ;
        }
        undo_move(b,movep);
        i=i+1 ;
    }

    return  best_move_position;
}

// state related
bool is_terminal_state(board_t b) {

    if ( check_board(b) == 1 || check_board(b) == 1 || check_board(b) == -1 ) {
        return true;
    } else {
        return false;
    }
}

struct move_position get_next_move_from_player() {

    struct move_position next_move_from_player;
    next_move_from_player.row = 2;
    next_move_from_player.col = 1;
    return  next_move_from_player;
}

void print_result (int result) {
    switch (result) {
        case 1: printf("X wins\n"); break;
        case -1: printf("O wins\n"); break;
        default: printf("draw\n");
    }
}
