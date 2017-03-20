#include "defs.h"
#include <stdlib.h>

#define RAND_64 (   (U64)rand() | \
                    (U64)rand() << 15 | \
                    (U64)rand() << 30 | \
                    (U64)rand() << 45 | \
                    ((U64)rand() & 0xf) << 60 )
                    
int Sq288To196[BRD_SQ_NUM];
int Sq196To288[196];
                    
U64 PieceKeys[25][288];
U64 WhiteSideKey;
U64 BlackSideKey;
U64 GoldSideKey;
U64 RedSideKey;
U64 CastleKeys[256];

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

void InitFilesRanksBrd() {
    
    int i = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq196 = 0;
    
    for(i = 0; i < BRD_SQ_NUM; i++) {
        FilesBrd[i] = OFFBOARD;
        RanksBrd[i] = OFFBOARD;
    }
    
    for(rank = RANK_1; rank <= RANK_14; rank++) {
        for(file = FILE_A; file <= FILE_N; file++) {
            sq = FR2SQ(file, rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }
    
}

void InitSq288To196() {
    int i = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq196 = 0;
    for(i = 0; i < BRD_SQ_NUM; i++) {
        Sq288To196[i] = 197;
    }
    
    for(i = 0; i < 196; i++) {
        Sq196To288[i] = 288;
    }
    
    for(rank = RANK_1; rank <= RANK_14; rank++) {
        for(file = FILE_A; file <= FILE_N; file++) {
            sq = FR2SQ(file, rank);
            Sq196To288[sq196] = sq;
            Sq288To196[sq] = sq196;
            sq196++;
        }
    }
}

void InitHashKeys() {
    int i = 0;
    int j = 0;
    for(i = 0; i < 25; ++i) {
        for(j = 0; j <288; j++) {
            PieceKeys[i][j] = RAND_64;
        }
    }
    
    WhiteSideKey = RAND_64;
    BlackSideKey = RAND_64;
    GoldSideKey = RAND_64;
    RedSideKey = RAND_64;
    
    for(i = 0; i < 32; i++) {
        CastleKeys[i] = RAND_64;
    }
}

void AllInit() {
    InitSq288To196();
    InitHashKeys();
    InitFilesRanksBrd();
    InitMvvLva();
}