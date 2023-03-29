#include <stdio.h>
#include <string.h>
#include "lab04.h"
#include <stdlib.h>

int board_size = BOARD_SZ ;
int main(int argc, char *argv[]) {

    board_t b; //initialize 

    int size_in_input_offset = 0;

    if (argc > 1 ) { //conditions for arg count 
        if (strcmp(argv[1], "-s") == 0 ) {
            board_size = atoi(argv[2]) ;
            size_in_input_offset = 2 ;
        }
    }

    if ( argc > 9 ) {

        char *xposition[board_size*board_size]; //variable that determines position for player X

        int total = board_size * board_size;

        for (int slot=0; slot < total ; slot++) {
            xposition[slot] = argv[slot+ size_in_input_offset+1];
        }

        init_board(b, xposition);
        print_board(b);
        if ( !is_board_empty(b) ) {
            struct move_position bestmove = find_best_move(b);
            update_board(b, bestmove, 'O');
            printf("O: %d %d\n", bestmove.row, bestmove.col);
        } else {
            printf("O: %d %d\n",0,0 );
        }

    } else {

        struct move_position bestmove; 

        char *xposition[] = {"_", "_", "_", "_", "_", "_", "_", "_", "_"}; //board layout
        init_board(b, xposition);
        while (1) {
            if ( is_terminal_state(b) ) {
                exit(0);
            }
            struct move_position  next_move_from_player;
            update_board(b, next_move_from_player,'X');
            print_board(b);
            if ( is_terminal_state(b) ) {
                exit(0);
            }
            bestmove = find_best_move(b);
            update_board(b, bestmove,'O');
            print_board(b);
            if ( is_terminal_state(b) ) {
                exit(0);
            }
        }
    }

    return 0 ;

}
