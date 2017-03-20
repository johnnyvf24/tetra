#include "defs.h"
#include <stdio.h>

#define HASH_PCE(pce, sq) (pos->posKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePerm)]))
//#define HASH_SIDE (pos->posKey ^= (SideKey))

static void ClearPiece(const int sq, S_BOARD *pos) {
    ASSERT(SqOnBoard(sq) && pos->pieces[sq] != OFFBOARD);
    
    int pce = pos->pieces[sq];
    
    ASSERT(PieceValid(pce));
    
    int col = PieceCol[pce];
    int i = 0;
    int t_pceNum = -1;
    
    HASH_PCE(pce, sq);
    
    pos->pieces[sq] = EMPTY;
    pos->material[col] -= PieceVal[pce];
    
    if(PieceBig[pce]) {
        pos->bigPce[col]--;
        if(PieceMaj[pce]) {
            pos->majPce[col]--;
        } else {
            pos->minPce[col]--;
        }
    } else {
        //TODO deal with pawn later
    }
    
    /*
     * Remove a piece from the piece list
     */
     for(i = 0; i < pos->pceNum[pce]; i++) {
         if(pos->pList[pce][i] == sq) {
             t_pceNum = i;
             break;
         }
     }
     
     ASSERT(t_pceNum != -1);
     pos->pceNum[pce]--;
     pos->pList[pce][t_pceNum] = pos->pList[pce][pos->pceNum[pce]];
}

static void AddPiece(const int sq, S_BOARD *pos, const int pce) {

    ASSERT(PieceValid(pce));
    ASSERT(SqOnBoard(sq));

    int col = PieceCol[pce];

    HASH_PCE(pce,sq);

    pos->pieces[sq] = pce;

    if(PieceBig[pce]) {
            pos->bigPce[col]++;
        if(PieceMaj[pce]) {
            pos->majPce[col]++;
        } else {
            pos->minPce[col]++;
        }
    } else {
    }

    pos->material[col] += PieceVal[pce];
    pos->pList[pce][pos->pceNum[pce]++] = sq;
	
}

static void MovePiece(const int from, const int to, S_BOARD *pos) {

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
	
    int index = 0;
    int pce = pos->pieces[from];	
    int col = PieceCol[pce];

#ifdef DEBUG
    int t_PieceNum = FALSE;
#endif

    HASH_PCE(pce,from);
    pos->pieces[from] = EMPTY;
    
    HASH_PCE(pce,to);
    pos->pieces[to] = pce;
    
    for(index = 0; index < pos->pceNum[pce]; ++index) {
        if(pos->pList[pce][index] == from) {
            pos->pList[pce][index] = to;
#ifdef DEBUG
            t_PieceNum = TRUE;
#endif
            break;
        }
    }
    ASSERT(t_PieceNum);
}

int MakeMove(S_BOARD *pos, int move) {
    
    ASSERT(CheckBoard(pos));
    int from  = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;
    
    
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    
    ASSERT(PieceValid(pos->pieces[from]));
    
    pos->history[pos->hisPly].posKey = pos->posKey;
    
    pos->history[pos->hisPly].move = move;
    
    int captured = CAPTURED(move);
    
    
    
    switch(captured) {
        case wK:
            pos->whiteOut = TRUE;
//            printf("King captured");
            break;
        case bK:
            pos->blackOut = TRUE;
//            printf("King captured");
            break;
        case gK: 
            pos->goldOut = TRUE;
//            printf("King captured");
            break;
        case rK:
            pos->redOut = TRUE;
//            printf("King captured");
            break;
    }

    if(captured != EMPTY) {
//        printf("%d", captured);
        ASSERT(PieceValid(captured));
        ClearPiece(to, pos);
    }
    
    pos->hisPly++;
    pos->ply++;
    
    MovePiece(from, to, pos);
    
    if(PieceKing[pos->pieces[to]]) {
        pos->KingsSq[pos->side] = to;
    }

    SetNextTurn(pos);

    switch(pos->side) {
        case WHITE:
            pos->posKey ^= WhiteSideKey;
            break;
        case BLACK:
            pos->posKey ^= BlackSideKey;
            break;
        case GOLD:
            pos->posKey ^= GoldSideKey;
            break;
        case RED:
            pos->posKey ^= RedSideKey;
            break;
    }

    ASSERT(CheckBoard(pos));
    
    
//    if(SqAttacked(pos->KingsSq[side], pos->side, pos)) {
//        TakeMove(pos);
//        return FALSE;
//    }
    
    
    switch(side) {
        case WHITE:
            if(SqAttacked(pos->KingsSq[side], BLACK, pos) || SqAttacked(pos->KingsSq[side], GOLD, pos) || SqAttacked(pos->KingsSq[side], RED, pos)) {
                TakeMove(pos);
                return FALSE;
            }
            break;
        case BLACK:
            if(SqAttacked(pos->KingsSq[side], WHITE, pos) || SqAttacked(pos->KingsSq[side], GOLD, pos) || SqAttacked(pos->KingsSq[side], RED, pos)) {
                TakeMove(pos);
                return FALSE;
            }
            break;
        case GOLD:
            if(SqAttacked(pos->KingsSq[side], BLACK, pos) || SqAttacked(pos->KingsSq[side], WHITE, pos) || SqAttacked(pos->KingsSq[side], RED, pos)) {
                TakeMove(pos);
                return FALSE;
            }
            break;
        case RED:
            if(SqAttacked(pos->KingsSq[side], BLACK, pos) || SqAttacked(pos->KingsSq[side], GOLD, pos) || SqAttacked(pos->KingsSq[side], WHITE, pos)) {
                TakeMove(pos);
                return FALSE;
            }
            break;
    }
    

    
    return TRUE;
}

void TakeMove(S_BOARD * pos) {
    ASSERT(CheckBoard(pos));
    
    pos->hisPly--;
    pos->ply--;
    
    int move = pos->history[pos->hisPly].move;
    
    int out = CAPTURED(move);
    
    int from = FROMSQ(move);
    int to = TOSQ(move);
    
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    
    switch(pos->side) {
        case WHITE:
            pos->posKey ^= WhiteSideKey;
            break;
        case BLACK:
            pos->posKey ^= BlackSideKey;
            break;
        case GOLD:
            pos->posKey ^= GoldSideKey;
            break;
        case RED:
            pos->posKey ^= RedSideKey;
            break;
    }
    
    int turnSet = FALSE;
    while(!turnSet) {
        switch(pos->side) {
            case WHITE:
                if(pos->redOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side = RED;
                break;
            case BLACK:
                if(pos->goldOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side = GOLD;   
                break;
            case GOLD:
                if(pos->whiteOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side = WHITE;
                break;
            case RED:
                if(pos->blackOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side = BLACK;
                break;
        }
    }
    
    switch(out) {
        case wK:
            pos->whiteOut = FALSE;
//            printf("returned w king\n");
            break;
        case bK:
            pos->blackOut = FALSE;
//            printf("returned b king\n");
            break;
        case gK: 
            pos->goldOut = FALSE;
//            printf("returned gk king\n");
            break;
        case rK:
            pos->redOut = FALSE;
//            printf("returned r king\n");
            break;
    }
    
//    switch(pos->side) {
//        case WHITE:
//            pos->side=BLACK;
//            break;
//        case BLACK:
//            pos->side=WHITE;
//            break;
//    }
    
    MovePiece(to, from, pos);
    
    if(PieceKing[pos->pieces[from]]) {
        pos->KingsSq[pos->side] = from;
    }
    
    int captured = CAPTURED(move);
    if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        AddPiece(to, pos, captured);
    }
    
    ASSERT(CheckBoard(pos));
}
