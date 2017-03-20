#include "defs.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INFINITE 10000000
#define MATE 29000


static void CheckUp(S_SEARCHINFO *info) {
    // .. Check if time up, or interrupt from GUI
    if(info->timeset = TRUE && GetTimeMs() > info->stoptime) {
        info->stopped = TRUE;
    } 
}

static void PickNextMove(int moveNum, S_MOVELIST *list) {
    
    S_MOVE temp;
    int index = 0;
    int bestScore = 0;
    int bestNum = moveNum;
    
    for (index = moveNum; index < list->count; index++) {
        if(list->moves[index].score > bestScore) {
            bestScore = list->moves[index].score;
            bestNum = index;
        }
    }
    
    temp = list->moves[moveNum];
    list->moves[moveNum] = list->moves[bestNum];
    list->moves[bestNum] = temp;

}

static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info) {
    int index = 0;
    int index2 = 0;

    for(index = 0; index < 25; ++index) {
        for(index2 = 0; index2 < BRD_SQ_NUM; ++index2) {
            pos->searchHistory[index][index2] = 0;
        }
    }

    for(index = 0; index < 2; ++index) {
        for(index2 = 0; index2 < MAXDEPTH; ++index2) {
            pos->searchKillers[index][index2] = 0;
        }
    }	

    ClearPvTable(pos->PvTable);	
    pos->ply = 0;

    info->starttime = GetTimeMs();
    info->stopped = 0;
    info->nodes = 0;
    info->fh = 0;
    info->fhf = 0;
}

static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {

    ASSERT(CheckBoard(pos));
    
    info->nodes++;
    
    if(pos->ply > MAXDEPTH - 1) {
        return EvalPosition(pos, WHITE);
    }
    
    int Score = EvalPosition(pos, WHITE);
    
    if(Score >= beta) {
        return beta;
    }
    
    if(Score > alpha) {
        alpha = Score;
    }
    
    S_MOVELIST list[1];
    GenerateAllCaps(pos,list);
      
    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    int BestMove = NOMOVE;
    Score = -INFINITE;
    int PvMove = ProbePvTable(pos);
    
    if( PvMove != NOMOVE ) {
        for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
            if( list->moves[MoveNum].move == PvMove) {
                list->moves[MoveNum].score = 2000000;
                break;
            }
        }
    }
    
    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
            
        PickNextMove(MoveNum, list);
        
        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }
        
        Legal++;
        Score = -Quiescence( -beta, -alpha, pos, info);
        TakeMove(pos);
        
        if(info->stopped == TRUE) {
            return 0;
        }
        
        if(Score > alpha) {
            if(Score >= beta) {
                if(Legal==1) {
                    info->fhf++;
                }
                info->fh++;	
                return beta;
            }
            alpha = Score;
            BestMove = list->moves[MoveNum].move;
        }
    }
    
    if(alpha != OldAlpha) {
        StorePvMove(pos, BestMove);
    }
    
    return alpha;
}

static int maximum(int num1, int num2) {
    if(num1 > num2) {
        return num1;
    } else {
        return num2;
    }
}

static int minimum(int num1, int num2) {
    if(num1 < num2) {
        return num1;
    } else {
        return num2;
    }
}

void shuffle(S_MOVELIST *list)
{
    if (list->count > 1) 
    {
        int i;
        for (i = 0; i < list->count - 1; i++) 
        {
          int j = i + rand() / (RAND_MAX / (list->count - i) + 1);
          S_MOVE t = list->moves[j];
          list->moves[j] = list->moves[i];
          list->moves[i] = t;
        }
    }
}

int GetNumOut(S_BOARD *pos) {
    int count = 0;
    if(pos->whiteOut == TRUE) {
        count++;
    }
    if(pos->blackOut == TRUE) {
        count++;
    }
    if(pos->goldOut == TRUE) {
        count++;
    }
    if(pos->redOut == TRUE) {
        count++;
    }
    return count;
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int color) {

    ASSERT(CheckBoard(pos)); 
    
    if(depth == 0) {
        info->nodes++;
        int val = EvalPosition(pos, color);
        return val;
    }
    
    if(GetNumOut(pos) == 3) {
        info->nodes++;
        return EvalPosition(pos, color);
    }
    
    info->nodes++;
    
    
    if(pos->ply > MAXDEPTH - 1) {
        return EvalPosition(pos, color);
    }
    
    S_MOVELIST list[1];
      
    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    int OldBeta = beta;
    int BestMove = NOMOVE;
    int PvMove = ProbePvTable(pos);
    int side = pos->side;
    if( PvMove != NOMOVE ) {
        for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
            if( list->moves[MoveNum].move == PvMove) {
                list->moves[MoveNum].score = 2000000;
                break;
            }
        }
    }
    
    if(color == pos->side) {
        // printf("MAX\n");
        int v = -INFINITE;
        int val;
//        printf("side %d\n", pos->side);
        GenerateAllMoves(pos, list);
        
        shuffle(list);
//        printf("TOTAL MOVES: %d\n", list->count)
        
        for(MoveNum = 0; MoveNum < list->count; MoveNum++) {
            PickNextMove(MoveNum, list);
            if ( !MakeMove(pos,list->moves[MoveNum].move))  {
                continue;
            }
//            PrintBoard(pos);
            Legal++;
            val = AlphaBeta(alpha, beta, depth-1, pos, info, color);
            
            switch(color) {
                case WHITE:
                    if(CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
                        val += 100000;
                    }
                    break;
                case BLACK:
                    if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
                        val += 100000;
                    }
                    break;
                case GOLD:
                    if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
                        val += 100000;
                    }
                    break;
                case RED:
                    if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK) {
                        val += 100000;
                    }
                    break;
            }
            
            TakeMove(pos);
            
            if(v < val) {
                v = val;
                BestMove = list->moves[MoveNum].move;
            }
            alpha = maximum(alpha, v);
            if(beta <= alpha) {
                if(Legal==1) {
                    info->fhf++;
                }
                info->fh++;
                break;
            }
        }
        
        if(Legal == 0) {
            SetNextTurn(pos);
            int val = AlphaBeta(alpha, beta, depth-1, pos, info, color);
            pos->side = side;
            return val;
        }
        
        if(alpha != OldAlpha) {
            
            StorePvMove(pos, BestMove);
        }        
        return alpha;
        
    } else {
        // printf("\nMIN: \n");
        int v = INFINITE;
        int val;
        GenerateAllMoves(pos, list);
        
        shuffle(list);
        
        for(MoveNum = 0; MoveNum < list->count; MoveNum++) {
            PickNextMove(MoveNum, list);
            
            if ( !MakeMove(pos,list->moves[MoveNum].move))  {
                continue;
            }
//            printf("\n\n Move: %s\n", PrMove(list->moves[MoveNum].move));
//            PrintBoard(pos);
            Legal++;
            val = AlphaBeta(alpha, beta, depth-1, pos, info, color);
            
            switch(color) {
                case WHITE:
                    if(CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
                        val += 100000;
                    }
                    break;
                case BLACK:
                    if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
                        val += 100000;
                    }
                    break;
                case GOLD:
                    if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
                        val += 100000;
                    }
                    break;
                case RED:
                    if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK) {
                        val += 100000;
                    }
                    break;
            }
            
//            printf("Score: %d\n", val);
            TakeMove(pos);
            
            if(val <= v) {
                v = val;
                BestMove = list->moves[MoveNum].move;
            }
            
            beta = minimum(beta, v);
            
            if(beta <= alpha) {
                if(Legal==1) {
                    info->fhf++;
                }
                info->fh++;
                
                break;
            }
        }
        
        if(Legal == 0) {
            SetNextTurn(pos);
            int val = AlphaBeta(alpha, beta, depth-1, pos, info, color);
            pos->side = side;
            return val;
        }
        
        
        if(beta != OldBeta) {
            StorePvMove(pos, BestMove);
        }
        
        return beta;
    
    } 
}


//static int brs(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int turn) {
//
//    ASSERT(CheckBoard(pos)); 
//    
//    if(depth <= 0) {
//        return EvalPosition(pos, turn);
//    }
//    
//    info->nodes++;
//    
//    S_MOVELIST list[1];
//    
//    if(turn == pos->side) {
//        GenerateAllMoves(pos, list);
//        turn = BLACK;
//    } else {
//        GenerateAllOpponents(pos,list);
//        turn = WHITE;
//    }
//
////    PrintMoveList(list);
//    
//    int MoveNum = 0;
//    int Legal = 0;
//    int OldAlpha = alpha;
//    int BestMove = NOMOVE;
////    int Score = -INFINITE;
////    int PvMove = ProbePvTable(pos);
//    int side = pos->side;
//    
//    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
//            
////        PickNextMove(MoveNum, list);
//        
//        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
//            continue;
//        }
////        if(info->nodes == 4)
////            PrintBoard(pos);
//        Legal++;
//        int Score = -brs( -beta, -alpha, depth-1, pos, info, turn);
//        
//        if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
//            Score += 100000;
//        }
//
//        TakeMove(pos);
//        
//        if(Score >= beta) {
//            return Score;
//        }
//        
//        if(Score > alpha ) {
//            BestMove = list->moves[MoveNum].move;
//            alpha = Score;
//        } 
//    }
//    
//    if(alpha != OldAlpha) {
//        StorePvMove(pos, BestMove);
//    }
//    
//    return alpha;
//} 



//static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull) {
//
//    ASSERT(CheckBoard(pos)); 
//    
//    if(depth == 0) {
////        return Quiescence(alpha, beta, pos, info);
////        if(pos->side == BLACK) {
////            return -1 * EvalPosition(pos, WHITE);
////        }
//
//        return EvalPosition(pos, WHITE);
//    }
//    
////    if(( info->nodes & 2047 ) == 0) {
////        CheckUp(info);
////    }
//        
//    info->nodes++;
//
//    if(pos->ply > MAXDEPTH - 1) {
//        return EvalPosition(pos, WHITE);
//    }
//    
//    S_MOVELIST list[1];
////    if(pos->side != WHITE) {
////        pos->side = WHITE;
////        GenerateAllOpponents(pos,list);
////        pos->side = BLACK;
////    } else {
//        GenerateAllMoves(pos, list);
////    }
//      
////    PrintMoveList(list);
//    int MoveNum = 0;
//    int Legal = 0;
//    int OldAlpha = alpha;
//    int BestMove = NOMOVE;
//    int Score = -INFINITE;
//    int PvMove = ProbePvTable(pos);
//    int side = pos->side;
//    
//    if( PvMove != NOMOVE ) {
//        for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
//            if( list->moves[MoveNum].move == PvMove) {
//                list->moves[MoveNum].score = 2000000;
//                break;
//            }
//        }
//    }
//    
//    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
//            
//        PickNextMove(MoveNum, list);
//        
//        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
//            continue;
//        }
//        
//        if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
//            Score += 100000;
//            
////            DoNull = (DoNull) ? FALSE: TRUE;
//        }
//        
//        Legal++;
//        if(DoNull && pos->side == ( (WHITE + 2) % 4) ) {
//            Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, FALSE);
//        } else {
//            Score = AlphaBeta( -beta, -alpha, depth-1, pos, info, TRUE);
//        }
//        
//        TakeMove(pos);
//        
//        if(Score > alpha) {
//            if(Score >= beta) {
//                if(Legal==1) {
//                    info->fhf++;
//                }
//                info->fh++;	
//                
//                if(!(list->moves[MoveNum].move & MFLAGCAP)) {
//                    pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
//                    pos->searchKillers[0][pos->ply] = list->moves[MoveNum].move;
//                }
//                
//                return beta;
//            }
//            alpha = Score;
//            BestMove = list->moves[MoveNum].move;
//            if(!(list->moves[MoveNum].move & MFLAGCAP)) {
//                pos->searchHistory[pos->pieces[FROMSQ(BestMove)]][TOSQ(BestMove)] += depth;
//            }
//        }
//    }
//    
//    if(Legal == 0) {
//        int turnSet = FALSE;
//        while(!turnSet) {
//            switch(pos->side) {
//                case WHITE:
//                    if(pos->goldOut == TRUE) {
//                        turnSet = FALSE;
//                    } else {
//                        turnSet = TRUE;
//                    }
//                    pos->side=GOLD;
//                    break;
//                case BLACK:
//                    if(pos->redOut == TRUE) {
//                        turnSet = FALSE;
//                    } else {
//                        turnSet = TRUE;
//                    }
//                    pos->side=RED;
//                    break;
//                case GOLD:
//                    if(pos->blackOut == TRUE) {
//                        turnSet = FALSE;
//                    } else {
//                        turnSet = TRUE;
//                    }
//                    pos->side=BLACK;
//                    break;
//                case RED:
//                    if(pos->whiteOut == TRUE) {
//                        turnSet = FALSE;
//                    } else {
//                        turnSet = TRUE;
//                    }
//                    pos->side=WHITE;
//                    break;
//            }
//        }
//        
//        if(pos->side != ( (WHITE + 2) % 4) ) {
//            Score = -AlphaBeta( -beta, -alpha, depth-1, pos, info, FALSE);
//        } else {
//            Score = AlphaBeta( beta, alpha, depth-1, pos, info, TRUE);
//        }
//        
//        Score+= 5000;
//        
//        if(CAPTURED(list->moves[MoveNum].move) ==  wK || CAPTURED(list->moves[MoveNum].move) ==  bK || CAPTURED(list->moves[MoveNum].move) ==  gK || CAPTURED(list->moves[MoveNum].move) ==  rK) {
//            Score += 100000;
//        }
//        
//        pos->side = side;
////        TakeMove(pos);
//    
//        if(Score > alpha) {
//            if(Score >= beta) {
//                if(Legal==1) {
//                    info->fhf++;
//                }
//                info->fh++;	
//                
//                if(!(list->moves[MoveNum].move & MFLAGCAP)) {
//                    pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
//                    pos->searchKillers[0][pos->ply] = list->moves[MoveNum].move;
//                }
//                
//                return beta;
//            }
//            alpha = Score;
//            BestMove = list->moves[MoveNum].move;
//            if(!(list->moves[MoveNum].move & MFLAGCAP)) {
//                pos->searchHistory[pos->pieces[FROMSQ(BestMove)]][TOSQ(BestMove)] += depth;
//            }
//        }
//    }
//    
//    if(alpha != OldAlpha) {
//        StorePvMove(pos, BestMove);
//    }
//    
//    return alpha;
//} 

void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) {
    
    int bestMove = NOMOVE;
    int bestScore = -INFINITE;
    int currentDepth = 0;
    int pvMoves = 0;
    int pvNum = 0;
    
    // iterative deepening
    for( currentDepth = 0; currentDepth <= info->depth; ++currentDepth ) {
        
        bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, pos->side);
        
        // out of time?
        
        pvMoves = GetPvLine(currentDepth, pos);
        bestMove = pos->PvArray[0];
        
        printf("Depth:%d score:%d move:%s nodes:%ld ",
            currentDepth,bestScore,PrMove(bestMove),info->nodes);
            
        pvMoves = GetPvLine(currentDepth, pos);	
        printf("pv");
        for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
            printf(" %s",PrMove(pos->PvArray[pvNum]));
        }
        
        printf("\n");
        printf("Ordering: %.2f\n", (info->fhf/info->fh));
    }
    printf("bestmove %s\n",PrMove(bestMove));
}