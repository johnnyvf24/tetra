#include <stdio.h>
#include <string.h>
#include "defs.h"

char *PrSq(const int sq) {

    static char SqStr[3];

    int file = FilesBrd[sq];
    int rank = RanksBrd[sq];



    sprintf(SqStr, "%c%c", ('a'+file), ('1'+rank));
    return SqStr;

}

char *PrMove(const int move) {

    static char MvStr[7];

    int ff = FilesBrd[FROMSQ(move)];
    int rf = RanksBrd[FROMSQ(move)];
    int ft = FilesBrd[TOSQ(move)];
    int rt = RanksBrd[TOSQ(move)];
    

    int promoted = PROMOTED(move);

    if(promoted) {
//        char pchar = 'q';
//        if(IsKn(promoted)) {
//            pchar = 'n';
//        } else if(IsRQ(promoted) && !IsBQ(promoted))  {
//            pchar = 'r';
//        } else if(!IsRQ(promoted) && IsBQ(promoted))  {
//            pchar = 'b';
//        }
//        sprintf(MvStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
    } else {
        if(rf > RANK_9 && rt > RANK_9) {
            rf = rf-266;
            rt = rt-266;
            sprintf(MvStr, "%c1%c-%c1%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
        } else if(rt > RANK_9) {
            rt = rt-266;
            sprintf(MvStr, "%c%c-%c1%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
        }   else if(rf > RANK_9) {
            rf = rf-266;
            sprintf(MvStr, "%c1%c-%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
        } else {
             sprintf(MvStr, "%c%c-%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
        }
    }

    return MvStr;
}

void PrintMoveList(const S_MOVELIST *list) {
    int i = 0;
    int score = 0;
    int move = 0;
    printf("Movelist:\n", list->count);
    
    for(i = 0; i < list->count; i++) {
        move = list->moves[i].move;
        score = list->moves[i].score;
        
        printf("Move: %d > %s (score: %d)\n", i+1, PrMove(move), score);
    }
    printf("MoveList Total %d Moves:\n\n", list->count);
    
}

int ParseMove(char *move, S_BOARD *pos) {
    
    char * fromTo[10];
    char fromTobytes[10];
    
    int count = explode("-", move, fromTo, fromTobytes);
    
    char * fromstr = fromTo[0];
    char * tostr = fromTo[1];
    
//    int i;
//    for(i = 0; i  < count; i++) {
//        printf("fromTo[%d]: %s\n", i, fromTo[i]);
//    }
    
//    printf("fromTo[1]: %s\n", fromTo[1]);
    
    int fileFrom = fromstr[0] - 'a';
    int fileTo = tostr[0] - 'a';
    
    int rankFrom, rankTo;
    
    fromstr++; tostr++;
    
    rankFrom = atoi(fromstr) - 1;
    rankTo = atoi(tostr) - 1;
    
//    printf("FileFrom: %d FileTo: %d\n", fileFrom, fileTo);
//    printf("fromstr: %s tostr: %d\n", fromstr, tostr);
    
    int from = FR2SQ(fileFrom, rankFrom);
    int to = FR2SQ(fileTo, rankTo);
    
//    printf("move: %s from: %d to %d\n", move, from, to);
    
    ASSERT(SqOnBoard(from) && SqOnBoard(to));
    
    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    int MoveNum = 0;
    int Move = 0;
    int PromPce = EMPTY;
    
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        Move = list->moves[MoveNum].move;

//        printf("from: %d, to: %d\n", FROMSQ(Move), TOSQ(Move));
        if(FROMSQ(Move) == from && TOSQ(Move) == to) {
            return Move;
        }
    }
    
    return NOMOVE;
}