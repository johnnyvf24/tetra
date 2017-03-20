#include "defs.h"

const int KnDir[8] = { -14, -31, -33, -18, 14, 31, 33, 18 };
const int RkDir[4] = { -1, -16,	1, 16 };
const int BiDir[4] = { -15, -17, 17, 15 };
const int KiDir[8] = { -1, -16,	1, 16, -15, -17, 17, 15 };

int SqAttacked(const int sq, const int side, const S_BOARD *pos) {
    
    int pce, i , t_sq, dir;
    ASSERT(SqOnBoard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(pos));
    // pawns
    switch(side) {
        case WHITE:
            if(pos->pieces[sq-17] == wP || pos->pieces[sq-15] == wP) {
                return TRUE;
            }
            break;
        case BLACK:
            if(pos->pieces[sq+17] == bP || pos->pieces[sq+15] == bP) {
                return TRUE;
            }
            break;
        case GOLD:
            if(pos->pieces[sq+15] == gP || pos->pieces[sq-17] == gP) {
                return TRUE;
            }
            break;
        case RED:
            if(pos->pieces[sq+17] == rP || pos->pieces[sq-15] == rP) {
                return TRUE;
            }
            break;
        default:
            //printf("Incorrect side input");
            break;
    }
    
    // knights
    for(i = 0; i < 8; i++) {
        pce = pos->pieces[sq + KnDir[i]];
        if(IsKn(pce) && PieceCol[pce] == side && pce != OFFBOARD) {
            return TRUE;
        }
    }
    
    // rooks, queens
    for(i = 0; i < 4; i++) {
        dir = RkDir[i];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while(pce != OFFBOARD) {
            if(pce != EMPTY) {
                if(IsRQ(pce) && PieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }
    
    // bishops, queens
    for(i = 0; i < 4; ++i) {		
        dir = BiDir[i];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while(pce != OFFBOARD) {
            if(pce != EMPTY) {
                if(IsBQ(pce) && PieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }
    
    // kings
    for(i = 0; i < 8; ++i) {		
        pce = pos->pieces[sq + KiDir[i]];
        if(IsKi(pce) && PieceCol[pce]==side) {
            return TRUE;
        }
    }
    
    return FALSE;
}