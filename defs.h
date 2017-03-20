#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>
#include <string.h>

//#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed", #n); \
printf("On %s ", __DATE__); \
printf("At %s ", __TIME__); \
printf("In file %s ", __FILE__); \
printf("At Line %d\n", __LINE__); \
exit(1); }
#endif

typedef unsigned long long U64;

#define NAME "ENGINE 1.0"
#define BRD_SQ_NUM 288

#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 512
#define MAXDEPTH 64

#define START_FEN "3,bR,bN,bB,bK,bQ,bB,bN,bR,3/3,bP,bP,bP,bP,bP,bP,bP,bP,3/14/gR,gP,10,rP,rR/gN,gP,10,rP,rN/gB,gP,10,rP,rB/gK,gP,10,rP,rQ/gQ,gP,10,rP,rK/gB,gP,10,rP,rB/gN,gP,10,rP,rN/gR,gP,10,rP,rR/14/3,wP,wP,wP,wP,wP,wP,wP,wP,3/3,wR,wN,wB,wQ,wK,wB,wN,wR,3"
#define TEST_FEN "9,wQ,4/3,bP,1,bK,3,bR,4/6,bP,1,bP,5/4,gP,1,bP,3,bP,3/gK,8,bP,3,wP,/14/1,gP,10,rP,1/3,bN,gP,9/1,wP,6,gR,4,wB,/14/6,wB,3,rP,3/7,wP,6/3,wP,1,wP,wP,3,wP,3/3,wR,wN,2,wK,2,wR,3"
#define TEST_WIN "3,bR,bN,bB,bK,1,bB,bN,bR,3/7,bP,6/3,bP,1,bP,2,bP,bP,4/gR,gP,2,bP,5,bP,rP,1,rR,/gN,11,rP,1/wQ,1,gP,8,rN,rP,1/2,gP,8,rP,rK,rQ,/gK,gP,bQ,9,rP,1/gB,gP,10,rP,rB,/gN,1,gP,9,rP,1/gR,gP,8,wP,1,rR,1/3,wP,2,wP,wP,2,wP,3/4,wP,wP,3,wP,4/3,wR,wN,wB,1,wK,wB,wN,wR,3"
#define TRICKY_FEN "3,bR,2,bK,rB,bB,bN,wB,3/4,bP,bP,8/3,bP,2,bP,bP,6/gR,2,gP,7,rP,1,rR,/gN,gP,9,rP,rB,bN,/gB,2,gP,gP,9/gK,11,rP,rQ,/2,gP,8,rP,rN,1/1,gP,10,rP,1/3,gP,6,rP,1,rK,1/1,gP,1,wP,1,wP,wP,5,rP,rR,/4,wP,3,wQ,5/7,wP,wP,wP,wP,3/3,wR,wN,2,wK,wB,wN,wR,3"
#define TRICKY_2 "3,bR,2,bK,rB,2,wB,3/5,bP,8/3,bP,2,bP,wQ,6/1,gR,1,gP,bP,2,bP,3,rP,1,rR,/gN,3,gP,6,rP,2/gB,2,gP,gP,6,rB,1,rR,/gK,11,rP,1/2,gP,8,rP,rN,1/2,gP,9,rP,1/3,gP,6,rP,1,rK,1/1,gP,1,wP,1,wP,wP,5,rP,1/4,wP,9/7,wP,wP,wP,wP,3/4,wR,2,wK,wB,wN,wR,3"
#define TEST_MATE "5,bR,2,bB,bN,bR,3/3,bP,1,bK,1,bP,bP,5/3,bN,gN,bP,bP,2,bP,4/gR,2,gP,6,bP,1,rP,rR,/4,rB,5,rP,2,rN,/gK,1,gP,9,rP,1/1,gP,10,rP,rQ,/2,gQ,gP,8,wP,1/gR,gP,gN,5,rP,2,wQ,2/2,gP,10,rK,/1,gP,6,gB,1,rP,3/7,wP,6/3,wP,wP,wP,wP,1,wP,1,wP,3/3,wR,wN,wB,1,wK,wB,1,wR,3"

//values for all different types of pieces
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, gP, gN, gB, gR, gQ, gK, rP, rN, rB, rR, rQ, rK };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_I, FILE_J, FILE_K, FILE_L, FILE_M, FILE_N, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_9, RANK_10, RANK_11, RANK_12, RANK_13, RANK_14 };

enum { WHITE, BLACK, GOLD, RED, ALL };

//BOARD is 16x18 in total, but game size is 14x14, extra cols and rows are for move generation
enum {
    A1 = 33, B1, C1, D1, E1, F1, G1, H1, I1, J1, K1, L1, M1, N1,
    A2 = 49, B2, C2, D2, E2, F2, G2, H2, I2, J2, K2, L2, M2, N2,
    A3 = 65, B3, C3, D3, E3, F3, G3, H3, I3, J3, K3, L3, M3, N3,
    A4 = 81, B4, C4, D4, E4, F4, G4, H4, I4, J4, K4, L4, M4, N4,
    A5 = 97, B5, C5, D5, E5, F5, G5, H5, I5, J5, K5, L5, M5, N5,
    A6 = 113, B6, C6, D6, E6, F6, G6, H6, I6, J6, K6, L6, M6, N6,
    A7 = 129, B7, C7, D7, E7, F7, G7, H7, I7, J7, K7, L7, M7, N7,
    A8 = 145, B8, C8, D8, E8, F8, G8, H8, I8, J8, K8, L8, M8, N8,
    A9 = 161, B9, C9, D9, E9, F9, G9, H9, I9, J9, K9, L9, M9, N9,
    A10 = 177, B10, C10, D10, E10, F10, G10, H10, I10, J10, K10, L10, M10, N10,
    A11 = 193, B11, C11, D11, E11, F11, G11, H11, I11, J11, K11, L11, M11, N11,
    A12 = 209, B12, C12, D12, E12, F12, G12, H12, I12, J12, K12, L12, M12, N12,
    A13 = 225, B13, C13, D13, E13, F13, G13, H13, I13, J13, K13, L13, M13, N13,
    A14 = 241, B14, C14, D14, E14, F14, G14, H14, I14, J14, K14, L14, M14, N14, NO_SQ, OFFBOARD
};

enum { FALSE, TRUE };

//different bits will hold the castling permissions
enum { WKCA=1, WQCA=2, BKCA=4, BQCA=8, GKCA=16, GQCA=32, RKCA=64, RQCA=128};

typedef struct {
    int move;
    int score;
} S_MOVE;

typedef struct {
    S_MOVE moves[MAXPOSITIONMOVES];
    int count;
} S_MOVELIST;

typedef struct {
    U64 posKey;
    int move;
} S_PVENTRY;

typedef struct {
    S_PVENTRY *pTable;
    int numEntries;
} S_PVTABLE;

typedef struct {
    int move;
    int castlePerm;
    U64 posKey;
    
} S_UNDO;

typedef struct {
    
    int pieces[BRD_SQ_NUM];
    int pawns[5][196];  //5 is for white, black, gold, red, all pawns; 196 is for all the squares in the board (14x14)
    
    int KingsSq[4]; //Kings are in the following order white, black, gold, red
    
    int side;
    
    int ply;
    int hisPly;
    
    int castlePerm;
    
    U64 posKey;
    
    int pceNum[25];
    int bigPce[4];
    int majPce[4];
    int minPce[4];
    int material[4];
    
    S_UNDO history[MAXGAMEMOVES];
    
    //piece list
    int pList[25][10];
    
    S_PVTABLE PvTable[1];
    int PvArray[MAXDEPTH];
    
    int searchHistory[25][BRD_SQ_NUM];
    int searchKillers[2][MAXDEPTH];
    
    int whiteOut;
    int blackOut;
    int goldOut;
    int redOut;
    
} S_BOARD;

typedef struct {
    
    int starttime;
    int stoptime;
    int depth;
    int depthset;
    int movestogo;
    int infinite;
    int timeset;
    
    
    long nodes;
    
    int quit;
    int stopped;
    
    float fh;
    float fhf;
    
} S_SEARCHINFO;

/* GAME MOVE */


/*
 * 0000 0000 0000 0000 0000 0000 1111 1111 -> FROM (8 bits), 0xFF
 *
 * 0000 0000 0000 0000 1111 1111 0000 0000 -> TO (8 bits), >> 8, 0xFF
 * 
 * 0000 0000 0001 1111 0000 0000 0000 0000 -> Type Captured (5 bits), >> 16, 0x1F
 * 
 * 0000 0000 0010 0000 0000 0000 0000 0000 -> Pawn start 0x200000
 * 
 * 0000 0011 1100 0000 0000 0000 0000 0000 -> Promotion to a N, B, R, Q (4 bits) >> 22, 0xF
 * 
 */

#define FROMSQ(m) ((m) & 0xFF)
#define TOSQ(m) (((m) >> 8) & 0xFF)
#define CAPTURED(m) (((m) >> 16) & 0x1F)
#define PROMOTED(m) (((m) >> 22) & 0xF)

#define MFLAGPS 0x200000

#define MFLAGCAP 0x1F0000

#define NOMOVE 0

/* MACROS */
#define FR2SQ(f, r) ( (33 + (f) )  + ( (r) * 16) )
#define SQ196(sq288) (Sq288To196[(sq288)])
#define SQ288(sq196) (Sq196To288[(sq196)])
#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])


/* GLOBALS */
extern int Sq288To196[BRD_SQ_NUM];
extern int Sq196To288[196];

extern U64 PieceKeys[25][288];
extern U64 WhiteSideKey;
extern U64 BlackSideKey;
extern U64 GoldSideKey;
extern U64 RedSideKey;
extern U64 CastleKeys[256];
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[25];
extern int PieceMaj[25];
extern int PieceMin[25];
extern int PieceVal[25];
extern int PieceCol[25];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

extern int PieceKnight[25];
extern int PieceKing[25];
extern int PieceRookQueen[25];
extern int PieceBishopQueen[25];


/* FUNCTIONS */

//init.c
extern void AllInit();

//hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos);

//board.c
extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char * fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern void UpdateListsMaterial(S_BOARD *pos);
extern int CheckBoard(const S_BOARD *pos);
extern void SetNextTurn(S_BOARD *pos);

extern size_t explode(const char *delim, const char *str, char **pointers_out, char *bytes_out);

//attack.c
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

//io.c

extern int ParseMove(char *move, S_BOARD *pos);

//validate.c
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid (const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

//movegen.c
extern void GenerateAllOpponents(const S_BOARD *pos, S_MOVELIST *list);
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);
extern void GenerateAllCaps(const S_BOARD * pos, S_MOVELIST *list);
extern int MoveExists(S_BOARD *pos, const int move);
extern int InitMvvLva();

//makemove.c
extern int MakeMove(S_BOARD *pos, int move);
extern void TakeMove(S_BOARD * pos);

//misc.c
extern int GetTimeMs();
extern void ReadInput(S_SEARCHINFO *info);

//pvtable.c
extern void InitPvTable(S_PVTABLE *table);
extern void StorePvMove(const S_BOARD *pos, const int move);
extern int ProbePvTable(const S_BOARD *pos);

//search.c
extern void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info);

//evaluation.c
extern int EvalPosition(const S_BOARD *pos, int color1);

//uci.c
extern void Uci_Loop();

#endif