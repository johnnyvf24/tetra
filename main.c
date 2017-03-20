#include <stdio.h>
#include <stdlib.h>
//extern "C" {
#include "defs.h"
//}

//void PrintBin(int move) {
//
//    int index = 0;
//    printf("As binary:\n");
//    for(index = 27; index >= 0; index--) {
//        if( (1<<index) & move) printf("1");
//        else printf("0");
//        if(index!=28 && index%4==0) printf(" ");
//    }
//    printf("\n");
//}
//
//void ShowSqAtBySide(const int side, const S_BOARD *pos) {
//        
//    int rank = 0;
//    int file = 0;
//    int sq = 0;
//    
//    printf("\n\nSquares attacked by:%c\n",SideChar[side]);
//    for(rank = RANK_14; rank >= RANK_1; --rank) {
//        for(file = FILE_A; file <= FILE_N; ++file) {
//            sq = FR2SQ(file,rank);
//            if(pos->pieces[sq] == OFFBOARD) {
//                printf("^");
//                continue;
//            }
//            if(SqAttacked(sq,side,pos)==TRUE) {
//                printf("X");
//            } else {
//                printf("-");
//            }
//                
//        }
//        printf("\n");
//    }  
//    printf("\n\n");
//
//}

int main() {
    AllInit();
    Uci_Loop();

//    S_BOARD board[1];
//    ParseFen(START_FEN, board);
//    PrintBoard(board);
//    
//    ASSERT(CheckBoard(board));
//    
//    S_MOVELIST list[1];
//
//    GenerateAllOpponents(board, list);
//    
//    int i, move;
//    for(i = 0; i < list->count; i++) {
//        move = list->moves[i].move;
//
//        printf("\nmove: %s", PrMove(move));
//    }  
//    
//    printf("\n Total Moves: %d", list->count);
//
//    int MoveNum = 0;
//    int move = 0;
//    PrintBoard(board);
//    getchar();
//    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
//        move = list->moves[MoveNum].move;
//        
//        if(!MakeMove(board, move)) {
//            continue;
//        }
//        
//        printf("\nMADE: %s\n", PrMove(move));
//        PrintBoard(board);
//        
//        TakeMove(board);
//        printf("\nTAKEN: %s\n", PrMove(move));
//        PrintBoard(board);
//        
//        getchar();
//    }

//    printf("\n\nWhite Attacking:\n");
//    ShowSqAtBySide(WHITE, board);
//    printf("\n\nBlack Attacking:\n");
//    ShowSqAtBySide(BLACK, board);
//    printf("\n\nGold Attacking:\n");
//    ShowSqAtBySide(GOLD, board);
//    printf("\n\nRed Attacking:\n");
//    ShowSqAtBySide(RED, board);

//    int move = 0;
//    int from = 6; int to = 12;
//    int cap  = wR; int prom = bR;
//    
//    move =((from) | ( to << 8 ) | (cap << 16) | (prom << 22));
//    
//    printf("\ndec: %d hex: %X\n", move, move);
//    PrintBin(move);
//    
//    printf("from: %d to %d cap %d prom %d\n", FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move));
//
//    move |= MFLAGPS;
//    
//    printf("is PST: %s\n", (move & MFLAGPS) ? "YES": "NO");

	return 0;
}
