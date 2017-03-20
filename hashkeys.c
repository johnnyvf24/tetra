#include "defs.h"

U64 GeneratePosKey(const S_BOARD *pos) {
    
    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;
    
    //pieces
    for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
        piece = pos->pieces[sq];
        if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
            ASSERT(piece >= wP && piece <= rK);
            finalKey ^= PieceKeys[piece][sq];
        }
    }
    
//    if(pos->side == WHITE) {
//        finalKey ^= WhiteSideKey;
//    }
//    
//    if(pos->side == BLACK) {
//        finalKey ^= BlackSideKey;
//    }
//    
//    if(pos->side == GOLD) {
//        finalKey ^= GoldSideKey;
//    }
//    
//    if(pos->side == RED) {
//        finalKey ^= RedSideKey;
//    }
//    
//    finalKey ^= CastleKeys[pos->castlePerm];
    
    return finalKey;
}