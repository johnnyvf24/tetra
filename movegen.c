#include <stdio.h>
#include "defs.h"

#define MOVE(f, t, ca, pro, fl) ((f) | ((t) << 8) | ( (ca) << 16) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFFBOARD)

int LoopSlidePce[16] = {
    wB, wR, wQ, 0, bB, bR, bQ, 0, gB, gR, gQ, 0, rB, rR, rQ, 0
};

int LoopNonSlidePce[12] = {
    wN, wK, 0, bN, bK, 0, gN, gK, 0, rN, rK, 0
};

int LoopSlideIndex[4] = { 0, 4, 8, 12 };
int LoopNonSlideIndex[4] = { 0, 3, 6, 9 };

int PceDir[25][8] = {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { -14, -31, -33, -18, 14, 31, 33, 18 },
    { -15, -17, 17, 15, 0, 0, 0, 0 },
    { -1, -16, 1, 16, 0, 0, 0, 0 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { -14, -31, -33, -18, 14, 31, 33, 18 },
    { -15, -17, 17, 15, 0, 0, 0, 0 },
    { -1, -16, 1, 16, 0, 0, 0, 0 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { -14, -31, -33, -18, 14, 31, 33, 18 },
    { -15, -17, 17, 15, 0, 0, 0, 0 },
    { -1, -16, 1, 16, 0, 0, 0, 0 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { -14, -31, -33, -18, 14, 31, 33, 18 },
    { -15, -17, 17, 15, 0, 0, 0, 0 },
    { -1, -16, 1, 16, 0, 0, 0, 0 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
    { -1, -16, 1, 16, -15, -17, 17, 15 },
};

int NumDir[25] = {
    0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
};

const int VictimScore[25] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };
static int MvvLvaScores[25][25];

int InitMvvLva() {
    int Attacker;
    int Victim;
    for(Attacker = wP; Attacker <= rK; ++Attacker) {
        for(Victim = wP; Victim <= rK; ++Victim) {
            MvvLvaScores[Victim][Attacker] = VictimScore[Victim] + 6 - ( VictimScore[Attacker] / 100);
        }
    }
}

int MoveExists(S_BOARD *pos, const int move) {

    S_MOVELIST list[1];
    GenerateAllMoves(pos,list);
      
    int MoveNum = 0;
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {	
       
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }        
        TakeMove(pos);
        if(list->moves[MoveNum].move == move) {
            return TRUE;
        }
    }
    return FALSE;
}

void AddQuietMove(const S_BOARD * pos, int move, S_MOVELIST *list ) {
    list->moves[list->count].move = move;
    
    if(pos->searchKillers[0][pos->ply] == move) {
        list->moves[list->count].score = 900000;
    } else if(pos->searchKillers[0][pos->ply] == move) {
        list->moves[list->count].score = 800000;
    } else {
        list->moves[list->count].score = pos->searchHistory[pos->pieces[FROMSQ(move)]][pos->pieces[TOSQ(move)]];
    }
    list->count++;
}

void AddCaptureMove(const S_BOARD * pos, int move, S_MOVELIST *list ) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = MvvLvaScores[CAPTURED(move)][pos->pieces[FROMSQ(move)]] + 1000000;
    list->count++;
}

void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    
    if(RanksBrd[from] == RANK_13) {
        AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
    } else {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

void AddBlackPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    
    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(RanksBrd[from] == RANK_2) {
        AddCaptureMove(pos, MOVE(from, to, cap, bQ, 0), list);
    } else {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

void AddGoldPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(FilesBrd[from] == FILE_M) {
        AddCaptureMove(pos, MOVE(from, to, cap, gQ, 0), list);
    } else {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

void AddRedPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(FilesBrd[from] == FILE_B) {
        AddCaptureMove(pos, MOVE(from, to, cap, rQ, 0), list);
    } else {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(RanksBrd[from] == RANK_13) {
        AddQuietMove(pos, MOVE(from, to, cap, wQ, 0), list);
    } else {
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(RanksBrd[from] == RANK_2) {
        AddQuietMove(pos, MOVE(from, to, cap, bQ, 0), list);
    } else {
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void AddGoldPawnMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(FilesBrd[from] == FILE_M) {
        AddQuietMove(pos, MOVE(from, to, cap, gQ, 0), list);
    } else {
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void AddRedPawnMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST * list) {
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    if(FilesBrd[from] == FILE_B) {
        AddQuietMove(pos, MOVE(from, to, cap, rQ, 0), list);
    } else {
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void GenerateAllMoves(const S_BOARD * pos, S_MOVELIST *list) {
    list->count = 0;
    
    ASSERT(CheckBoard(pos));
    
    list->count = 0;
    
    int pce = EMPTY;
    int side = pos->side;
    int sq = 0; int t_sq = 0;
    int pceNum = 0;
    int dir = 0;
    int i = 0;
    int pceIndex = 0;
    
    switch(side) {
        case WHITE:
            for(pceNum = 0; pceNum < pos->pceNum[wP]; pceNum++) {
                sq = pos->pList[wP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq + 16] == EMPTY) {
                    AddWhitePawnMove(pos, sq, sq+16, EMPTY, list);
                    if(RanksBrd[sq] == RANK_2 && pos->pieces[sq+32] == EMPTY) {
                        AddQuietMove(pos, MOVE(sq, (sq+32), EMPTY, EMPTY, MFLAGPS), list);
                    }
                }
                
                if(pos->pieces[sq + 15] != OFFBOARD) {
                    
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 15]] == BLACK || 
                        PieceCol[pos->pieces[sq + 15]] == GOLD || 
                        PieceCol[pos->pieces[sq + 15]] == RED)) {
                        
                        AddWhitePawnCapMove(pos, sq, sq+15, pos->pieces[sq+15], list);
                    }
                }
                
                if(pos->pieces[sq + 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 17]] == BLACK || 
                        PieceCol[pos->pieces[sq + 17]] == GOLD || 
                        PieceCol[pos->pieces[sq + 17]] == RED)) {
                        
                        AddWhitePawnCapMove(pos, sq, sq+17, pos->pieces[sq+17], list);
                    }
                }
            }
            break;
        case BLACK:
            for(pceNum = 0; pceNum < pos->pceNum[bP]; pceNum++) {
                sq = pos->pList[bP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq - 16] == EMPTY) {
                    AddBlackPawnMove(pos, sq, sq-16, EMPTY, list);
                    if(RanksBrd[sq] == RANK_13 && pos->pieces[sq-32] == EMPTY) {
                        AddQuietMove(pos, MOVE(sq, (sq-32), EMPTY, EMPTY, MFLAGPS), list);
                    }
                }
                
                if(pos->pieces[sq - 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 17]] == WHITE || 
                        PieceCol[pos->pieces[sq - 17]] == GOLD || 
                        PieceCol[pos->pieces[sq - 17]] == RED) {
                        
                        AddBlackPawnCapMove(pos, sq, sq-17, pos->pieces[sq-17], list);
                    }
                }
                
                if(pos->pieces[sq - 15] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 15]] == WHITE || 
                        PieceCol[pos->pieces[sq - 15]] == GOLD || 
                        PieceCol[pos->pieces[sq - 15]] == RED) {
                        
                        AddBlackPawnCapMove(pos, sq, sq-15, pos->pieces[sq-15], list);
                    }
                }
            }
            break;
        case GOLD:
            for(pceNum = 0; pceNum < pos->pceNum[gP]; pceNum++) {
                sq = pos->pList[gP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq + 1] == EMPTY) {
                    AddGoldPawnMove(pos, sq, sq+1, EMPTY, list);
                    if(FilesBrd[sq] == FILE_B && pos->pieces[sq+2] == EMPTY) {
                        AddQuietMove(pos, MOVE(sq, (sq+2), EMPTY, EMPTY, MFLAGPS), list);
                    }
                }
                
                if(pos->pieces[sq + 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq + 17]] == WHITE || 
                        PieceCol[pos->pieces[sq + 17]] == BLACK || 
                        PieceCol[pos->pieces[sq + 17]] == RED) {
                        
                        AddGoldPawnCapMove(pos, sq, sq+17, pos->pieces[sq+17], list);
                    }
                }
                
                if(pos->pieces[sq - 15] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 15]] == WHITE || 
                        PieceCol[pos->pieces[sq - 15]] == BLACK || 
                        PieceCol[pos->pieces[sq - 15]] == RED) {
                        
                        AddGoldPawnCapMove(pos, sq, sq-15, pos->pieces[sq-15], list);
                    }
                }
            }
            break;
        case RED:
            for(pceNum = 0; pceNum < pos->pceNum[rP]; pceNum++) {
                sq = pos->pList[rP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq - 1] == EMPTY) {
                    AddRedPawnMove(pos, sq, sq-1, EMPTY, list);
                    if(FilesBrd[sq] == FILE_M && pos->pieces[sq-2] == EMPTY) {
                        AddQuietMove(pos, MOVE(sq, (sq-2), EMPTY, EMPTY, MFLAGPS), list);
                    }
                }
                
                if(pos->pieces[sq + 15] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 15]] == WHITE || 
                        PieceCol[pos->pieces[sq + 15]] == BLACK || 
                        PieceCol[pos->pieces[sq + 15]] == GOLD)) {
                        
                        AddRedPawnCapMove(pos, sq, sq+15, pos->pieces[sq+15], list);
                    }
                }
                
                if(pos->pieces[sq - 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq - 17]] == WHITE || 
                        PieceCol[pos->pieces[sq - 17]] == BLACK || 
                        PieceCol[pos->pieces[sq - 17]] == GOLD)) {
                        
                        AddRedPawnCapMove(pos, sq, sq-17, pos->pieces[sq-17], list);
                    }
                }
            }
            break;
    }
    
    /* Loop for slide pieces */
    pceIndex = LoopSlideIndex[side];
    pce = LoopSlidePce[pceIndex++];
    while(pce != 0) {
        ASSERT(PieceValid(pce));
        
        for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) {
            sq = pos->pList[pce][pceNum];
            ASSERT(SqOnBoard(sq));
            
            for(i = 0; i < NumDir[pce]; i++) {
                dir = PceDir[pce][i];
                t_sq = sq + dir;
                
                while(!SQOFFBOARD(t_sq) && pos->pieces[t_sq] != OFFBOARD) {
                    
                    if(pos->pieces[t_sq] != EMPTY) {
                        switch(side) {
                            case WHITE:
                                if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                    || PieceCol[pos->pieces[t_sq]] == GOLD 
                                    || PieceCol[pos->pieces[t_sq]] == RED) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                            case BLACK:
                                if(PieceCol[pos->pieces[t_sq]] == WHITE 
                                    || PieceCol[pos->pieces[t_sq]] == GOLD 
                                    || PieceCol[pos->pieces[t_sq]] == RED) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                            case GOLD:
                                if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                    || PieceCol[pos->pieces[t_sq]] == WHITE 
                                    || PieceCol[pos->pieces[t_sq]] == RED) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                            case RED:
                                if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                    || PieceCol[pos->pieces[t_sq]] == GOLD 
                                    || PieceCol[pos->pieces[t_sq]] == WHITE) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                        }

                        break;
                    }
                    
                    AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
                    t_sq += dir;
                }
            }
        }
        
        pce = LoopSlidePce[pceIndex++];
    }
    
    /* Loop for Non sliding pieces */
    pceIndex = LoopNonSlideIndex[side];
    pce = LoopNonSlidePce[pceIndex++];
    
    while(pce != 0) {
        ASSERT(PieceValid(pce));
        
        for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) {
            sq = pos->pList[pce][pceNum];
            ASSERT(SqOnBoard(sq));
            
            for(i = 0; i < NumDir[pce]; i++) {
                dir = PceDir[pce][i];
                t_sq = sq + dir;
                
                if(SQOFFBOARD(t_sq) || pos->pieces[t_sq] == OFFBOARD) {
                    continue;
                }
                
                if(pos->pieces[t_sq] != EMPTY) {
                    switch(side) {
                        case WHITE:
                            if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                || PieceCol[pos->pieces[t_sq]] == GOLD 
                                || PieceCol[pos->pieces[t_sq]] == RED) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        case BLACK:
                            if(PieceCol[pos->pieces[t_sq]] == WHITE 
                                || PieceCol[pos->pieces[t_sq]] == GOLD 
                                || PieceCol[pos->pieces[t_sq]] == RED) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        case GOLD:
                            if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                || PieceCol[pos->pieces[t_sq]] == WHITE 
                                || PieceCol[pos->pieces[t_sq]] == RED) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        case RED:
                            if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                || PieceCol[pos->pieces[t_sq]] == GOLD 
                                || PieceCol[pos->pieces[t_sq]] == WHITE) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                    }

                    continue;
                }
                
                AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
            }
        }
        
        pce = LoopNonSlidePce[pceIndex++];
    }
}

void GenerateAllOpponents(const S_BOARD *pos, S_MOVELIST *list) {
    list->count = 0;
    
    ASSERT(CheckBoard(pos));
    
    list->count = 0;
    
    int pce = EMPTY;
    int side = pos->side;
    int sq = 0; int t_sq = 0;
    int pceNum = 0;
    int dir = 0;
    int i = 0;
    int pceIndex = 0;
    
    for(i = WHITE; i <= RED; i++) {
        if(i != pos->side) {
            switch(i) {
                case WHITE:
                    for(pceNum = 0; pceNum < pos->pceNum[wP]; pceNum++) {
                        sq = pos->pList[wP][pceNum];
                        
                        ASSERT(SqOnBoard(sq));
                        
                        if(pos->pieces[sq + 16] == EMPTY) {
                            AddWhitePawnMove(pos, sq, sq+16, EMPTY, list);
                            if(RanksBrd[sq] == RANK_2 && pos->pieces[sq+32] == EMPTY) {
                                AddQuietMove(pos, MOVE(sq, (sq+32), EMPTY, EMPTY, MFLAGPS), list);
                            }
                        }
                        
                        if(pos->pieces[sq + 15] != OFFBOARD) {
                            
                            if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 15]] == BLACK || 
                                PieceCol[pos->pieces[sq + 15]] == GOLD || 
                                PieceCol[pos->pieces[sq + 15]] == RED)) {
                                
                                AddWhitePawnCapMove(pos, sq, sq+15, pos->pieces[sq+15], list);
                            }
                        }
                        
                        if(pos->pieces[sq + 17] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 17]] == BLACK || 
                                PieceCol[pos->pieces[sq + 17]] == GOLD || 
                                PieceCol[pos->pieces[sq + 17]] == RED)) {
                                
                                AddWhitePawnCapMove(pos, sq, sq+17, pos->pieces[sq+17], list);
                            }
                        }
                    }
                    break;
                case BLACK:
                    for(pceNum = 0; pceNum < pos->pceNum[bP]; pceNum++) {
                        sq = pos->pList[bP][pceNum];
                        
                        ASSERT(SqOnBoard(sq));
                        
                        if(pos->pieces[sq - 16] == EMPTY) {
                            AddBlackPawnMove(pos, sq, sq-16, EMPTY, list);
                            if(RanksBrd[sq] == RANK_13 && pos->pieces[sq-32] == EMPTY) {
                                AddQuietMove(pos, MOVE(sq, (sq-32), EMPTY, EMPTY, MFLAGPS), list);
                            }
                        }
                        
                        if(pos->pieces[sq - 17] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 17]] == WHITE || 
                                PieceCol[pos->pieces[sq - 17]] == GOLD || 
                                PieceCol[pos->pieces[sq - 17]] == RED) {
                                
                                AddBlackPawnCapMove(pos, sq, sq-17, pos->pieces[sq-17], list);
                            }
                        }
                        
                        if(pos->pieces[sq - 15] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 15]] == WHITE || 
                                PieceCol[pos->pieces[sq - 15]] == GOLD || 
                                PieceCol[pos->pieces[sq - 15]] == RED) {
                                
                                AddBlackPawnCapMove(pos, sq, sq-15, pos->pieces[sq-15], list);
                            }
                        }
                    }
                    break;
                case GOLD:
                    for(pceNum = 0; pceNum < pos->pceNum[gP]; pceNum++) {
                        sq = pos->pList[gP][pceNum];
                        
                        ASSERT(SqOnBoard(sq));
                        
                        if(pos->pieces[sq + 1] == EMPTY) {
                            AddGoldPawnMove(pos, sq, sq+1, EMPTY, list);
                            if(FilesBrd[sq] == FILE_B && pos->pieces[sq+2] == EMPTY) {
                                AddQuietMove(pos, MOVE(sq, (sq+2), EMPTY, EMPTY, MFLAGPS), list);
                            }
                        }
                        
                        if(pos->pieces[sq + 17] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq + 17]] == WHITE || 
                                PieceCol[pos->pieces[sq + 17]] == BLACK || 
                                PieceCol[pos->pieces[sq + 17]] == RED) {
                                
                                AddGoldPawnCapMove(pos, sq, sq+17, pos->pieces[sq+17], list);
                            }
                        }
                        
                        if(pos->pieces[sq - 15] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 15]] == WHITE || 
                                PieceCol[pos->pieces[sq - 15]] == BLACK || 
                                PieceCol[pos->pieces[sq - 15]] == RED) {
                                
                                AddGoldPawnCapMove(pos, sq, sq-15, pos->pieces[sq-15], list);
                            }
                        }
                    }
                    break;
                case RED:
                    for(pceNum = 0; pceNum < pos->pceNum[rP]; pceNum++) {
                        sq = pos->pList[rP][pceNum];
                        
                        ASSERT(SqOnBoard(sq));
                        
                        if(pos->pieces[sq - 1] == EMPTY) {
                            AddRedPawnMove(pos, sq, sq-1, EMPTY, list);
                            if(FilesBrd[sq] == FILE_M && pos->pieces[sq-2] == EMPTY) {
                                AddQuietMove(pos, MOVE(sq, (sq-2), EMPTY, EMPTY, MFLAGPS), list);
                            }
                        }
                        
                        if(pos->pieces[sq + 15] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 15]] == WHITE || 
                                PieceCol[pos->pieces[sq + 15]] == BLACK || 
                                PieceCol[pos->pieces[sq + 15]] == GOLD)) {
                                
                                AddRedPawnCapMove(pos, sq, sq+15, pos->pieces[sq+15], list);
                            }
                        }
                        
                        if(pos->pieces[sq - 17] != OFFBOARD) {
                            if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq - 17]] == WHITE || 
                                PieceCol[pos->pieces[sq - 17]] == BLACK || 
                                PieceCol[pos->pieces[sq - 17]] == GOLD)) {
                                
                                AddRedPawnCapMove(pos, sq, sq-17, pos->pieces[sq-17], list);
                            }
                        }
                    }
                    break;
            }
        }
    }
    
    int j;
    for(j = WHITE; j <= RED; j++) {
        if(j == side) {
            continue;
        }
        /* Loop for slide pieces */
        pceIndex = LoopSlideIndex[j];
        pce = LoopSlidePce[pceIndex++];
        while(pce != 0) {
            ASSERT(PieceValid(pce));
            
            for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) {
                sq = pos->pList[pce][pceNum];
                ASSERT(SqOnBoard(sq));
                
                for(i = 0; i < NumDir[pce]; i++) {
                    dir = PceDir[pce][i];
                    t_sq = sq + dir;
                    
                    while(!SQOFFBOARD(t_sq) && pos->pieces[t_sq] != OFFBOARD) {
                        
                        if(pos->pieces[t_sq] != EMPTY) {
                            
                            if(PieceCol[pos->pieces[t_sq]] == side) {
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        }
                        
                        AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
                        t_sq += dir;
                    }
                }
            }
            
            pce = LoopSlidePce[pceIndex++];
        }
    
    
        /* Loop for Non sliding pieces */
        pceIndex = LoopNonSlideIndex[j];
        pce = LoopNonSlidePce[pceIndex++];
        
        while(pce != 0) {
            ASSERT(PieceValid(pce));
            
            for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) {
                sq = pos->pList[pce][pceNum];
                ASSERT(SqOnBoard(sq));
                
                for(i = 0; i < NumDir[pce]; i++) {
                    dir = PceDir[pce][i];
                    t_sq = sq + dir;
                    
                    if(SQOFFBOARD(t_sq) || pos->pieces[t_sq] == OFFBOARD) {
                        continue;
                    }
                    
                    if(pos->pieces[t_sq] != EMPTY) {
                        
                        if(PieceCol[pos->pieces[t_sq]] == side) {
                            AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                        }

                        continue;
                    }
                    
                    AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
                }
            }
            
            pce = LoopNonSlidePce[pceIndex++];
        }
    }
}

void GenerateAllCaps(const S_BOARD * pos, S_MOVELIST *list) {
    list->count = 0;
    
    ASSERT(CheckBoard(pos));
    
    list->count = 0;
    
    int pce = EMPTY;
    int side = pos->side;
    int sq = 0; int t_sq = 0;
    int pceNum = 0;
    int dir = 0;
    int i = 0;
    int pceIndex = 0;
    
    switch(side) {
        case WHITE:
            for(pceNum = 0; pceNum < pos->pceNum[wP]; pceNum++) {
                sq = pos->pList[wP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq + 15] != OFFBOARD) {
                    
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 15]] == BLACK || 
                        PieceCol[pos->pieces[sq + 15]] == GOLD || 
                        PieceCol[pos->pieces[sq + 15]] == RED)) {
                        
                        AddWhitePawnCapMove(pos, sq, sq+15, pos->pieces[sq+15], list);
                    }
                }
                
                if(pos->pieces[sq + 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 17]] == BLACK || 
                        PieceCol[pos->pieces[sq + 17]] == GOLD || 
                        PieceCol[pos->pieces[sq + 17]] == RED)) {
                        
                        AddWhitePawnCapMove(pos, sq, sq+17, pos->pieces[sq+17], list);
                    }
                }
            }
            break;
        case BLACK:
            for(pceNum = 0; pceNum < pos->pceNum[bP]; pceNum++) {
                sq = pos->pList[bP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq - 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 17]] == WHITE || 
                        PieceCol[pos->pieces[sq - 17]] == GOLD || 
                        PieceCol[pos->pieces[sq - 17]] == RED) {
                        
                        AddBlackPawnCapMove(pos, sq, sq-17, pos->pieces[sq-17], list);
                    }
                }
                
                if(pos->pieces[sq - 15] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 15]] == WHITE || 
                        PieceCol[pos->pieces[sq - 15]] == GOLD || 
                        PieceCol[pos->pieces[sq - 15]] == RED) {
                        
                        AddBlackPawnCapMove(pos, sq, sq-15, pos->pieces[sq-15], list);
                    }
                }
            }
            break;
        case GOLD:
            for(pceNum = 0; pceNum < pos->pceNum[gP]; pceNum++) {
                sq = pos->pList[gP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq + 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq + 17]] == WHITE || 
                        PieceCol[pos->pieces[sq + 17]] == BLACK || 
                        PieceCol[pos->pieces[sq + 17]] == RED) {
                        
                        AddGoldPawnCapMove(pos, sq, sq+17, pos->pieces[sq+17], list);
                    }
                }
                
                if(pos->pieces[sq - 15] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && PieceCol[pos->pieces[sq - 15]] == WHITE || 
                        PieceCol[pos->pieces[sq - 15]] == BLACK || 
                        PieceCol[pos->pieces[sq - 15]] == RED) {
                        
                        AddGoldPawnCapMove(pos, sq, sq-15, pos->pieces[sq-15], list);
                    }
                }
            }
            break;
        case RED:
            for(pceNum = 0; pceNum < pos->pceNum[rP]; pceNum++) {
                sq = pos->pList[rP][pceNum];
                
                ASSERT(SqOnBoard(sq));
                
                if(pos->pieces[sq + 15] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq + 15]] == WHITE || 
                        PieceCol[pos->pieces[sq + 15]] == BLACK || 
                        PieceCol[pos->pieces[sq + 15]] == GOLD)) {
                        
                        AddRedPawnCapMove(pos, sq, sq+15, pos->pieces[sq+15], list);
                    }
                }
                
                if(pos->pieces[sq - 17] != OFFBOARD) {
                    if(!SQOFFBOARD(sq) && (PieceCol[pos->pieces[sq - 17]] == WHITE || 
                        PieceCol[pos->pieces[sq - 17]] == BLACK || 
                        PieceCol[pos->pieces[sq - 17]] == GOLD)) {
                        
                        AddRedPawnCapMove(pos, sq, sq-17, pos->pieces[sq-17], list);
                    }
                }
            }
            break;
    }
    
    /* Loop for slide pieces */
    pceIndex = LoopSlideIndex[side];
    pce = LoopSlidePce[pceIndex++];
    while(pce != 0) {
        ASSERT(PieceValid(pce));
        
        for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) {
            sq = pos->pList[pce][pceNum];
            ASSERT(SqOnBoard(sq));
            
            for(i = 0; i < NumDir[pce]; i++) {
                dir = PceDir[pce][i];
                t_sq = sq + dir;
                
                while(!SQOFFBOARD(t_sq) && pos->pieces[t_sq] != OFFBOARD) {
                    
                    if(pos->pieces[t_sq] != EMPTY) {
                        switch(side) {
                            case WHITE:
                                if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                    || PieceCol[pos->pieces[t_sq]] == GOLD 
                                    || PieceCol[pos->pieces[t_sq]] == RED) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                            case BLACK:
                                if(PieceCol[pos->pieces[t_sq]] == WHITE 
                                    || PieceCol[pos->pieces[t_sq]] == GOLD 
                                    || PieceCol[pos->pieces[t_sq]] == RED) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                            case GOLD:
                                if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                    || PieceCol[pos->pieces[t_sq]] == WHITE 
                                    || PieceCol[pos->pieces[t_sq]] == RED) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                            case RED:
                                if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                    || PieceCol[pos->pieces[t_sq]] == GOLD 
                                    || PieceCol[pos->pieces[t_sq]] == WHITE) {
                                    
                                    AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                                }
                                break;
                        }

                        break;
                    }
                    
                    t_sq += dir;
                }
            }
        }
        
        pce = LoopSlidePce[pceIndex++];
    }
    
    /* Loop for Non sliding pieces */
    pceIndex = LoopNonSlideIndex[side];
    pce = LoopNonSlidePce[pceIndex++];
    
    while(pce != 0) {
        ASSERT(PieceValid(pce));
        
        for(pceNum = 0; pceNum < pos->pceNum[pce]; pceNum++) {
            sq = pos->pList[pce][pceNum];
            ASSERT(SqOnBoard(sq));
            
            for(i = 0; i < NumDir[pce]; i++) {
                dir = PceDir[pce][i];
                t_sq = sq + dir;
                
                if(SQOFFBOARD(t_sq) || pos->pieces[t_sq] == OFFBOARD) {
                    continue;
                }
                
                if(pos->pieces[t_sq] != EMPTY) {
                    switch(side) {
                        case WHITE:
                            if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                || PieceCol[pos->pieces[t_sq]] == GOLD 
                                || PieceCol[pos->pieces[t_sq]] == RED) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        case BLACK:
                            if(PieceCol[pos->pieces[t_sq]] == WHITE 
                                || PieceCol[pos->pieces[t_sq]] == GOLD 
                                || PieceCol[pos->pieces[t_sq]] == RED) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        case GOLD:
                            if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                || PieceCol[pos->pieces[t_sq]] == WHITE 
                                || PieceCol[pos->pieces[t_sq]] == RED) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                        case RED:
                            if(PieceCol[pos->pieces[t_sq]] == BLACK 
                                || PieceCol[pos->pieces[t_sq]] == GOLD 
                                || PieceCol[pos->pieces[t_sq]] == WHITE) {
                                
                                AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
                            }
                            break;
                    }

                    continue;
                }
            }
        }
        
        pce = LoopNonSlidePce[pceIndex++];
    }
}