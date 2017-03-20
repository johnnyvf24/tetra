#include <stdio.h>
#include "defs.h"
//
//const int PawnTable[196] = {
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0       
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0,   
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0,   
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0,
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0,   
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0,   
//0,      0,      0,      -10,    10,     0,      0,      0,      10,     0,      0,      0,      0,      0,   
//0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 
//};
//
//const int KnightTable[64] = {
//0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
//0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
//0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
//0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
//5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
//5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
//};
//
//const int BishopTable[64] = {
//0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
//0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
//0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
//0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
//0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
//0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
//0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
//0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
//};
//
//const int RookTable[64] = {
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
//25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
//0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
//};
//
//const int Mirror64[64] = {
//56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
//48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
//40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
//32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
//24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
//16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
//8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
//0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
//};

//#define MIRROR64(sq) (Mirror64[(sq)])

int EvalPosition(const S_BOARD *pos, int color1) {
    int score = 0;
    
    switch(color1) {
        case WHITE:
            score = pos->material[WHITE]*2 - pos->material[BLACK] - pos->material[GOLD] - pos->material[RED];
            break;
        case BLACK:
            score = pos->material[BLACK]*2 - pos->material[GOLD] - pos->material[RED] - pos->material[WHITE];
            break;
        case GOLD:
            score = pos->material[GOLD]*2 - pos->material[BLACK] - pos->material[WHITE] - pos->material[RED];
            break;
        case RED:
            score = pos->material[RED]*2 - pos->material[BLACK] - pos->material[GOLD] - pos->material[WHITE];
            break;
    }
     
    if(pos->whiteOut) {
        score += pos->material[WHITE];
    }
    if(pos->blackOut) {
        score += pos->material[BLACK];
    }
    if(pos->goldOut) {
        score += pos->material[GOLD];
    }
    if(pos->redOut) {
        score += pos->material[RED];
    }
    
    // if(pos->side != color1) {
    //     score *= -1;
    // }
    
    return score;
}