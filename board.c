#include <assert.h>
#include <stdio.h>
#include "defs.h"
#include <string.h>

char * str_replace ( const char *string, const char *substr, const char *replacement ){
  char *tok = NULL;
  char *newstr = NULL;
  char *oldstr = NULL;
  char *head = NULL;
 
  /* if either substr or replacement is NULL, duplicate string a let caller handle it */
  if ( substr == NULL || replacement == NULL ) return strdup (string);
  newstr = strdup (string);
  head = newstr;
  while ( (tok = strstr ( head, substr ))){
    oldstr = newstr;
    newstr = malloc ( strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) + 1 );
    /*failed to alloc mem, free old string and return NULL */
    if ( newstr == NULL ){
      free (oldstr);
      return NULL;
    }
    memcpy ( newstr, oldstr, tok - oldstr );
    memcpy ( newstr + (tok - oldstr), replacement, strlen ( replacement ) );
    memcpy ( newstr + (tok - oldstr) + strlen( replacement ), tok + strlen ( substr ), strlen ( oldstr ) - strlen ( substr ) - ( tok - oldstr ) );
    memset ( newstr + strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) , 0, 1 );
    /* move back head right after the last replacement */
    head = newstr + (tok - oldstr) + strlen( replacement );
    free (oldstr);
  }
  return newstr;
}

/*
 * Split a string by a delimiter.
 *
 * This function writes the beginning of each item to @pointers_out
 * (forming an array of C strings), and writes the actual string bytes
 * to @bytes_out.  Both buffers are assumed to be big enough for all of the
 * strings.
 *
 * Returns the number of strings written to @pointers_out.
 */
size_t explode(const char *delim, const char *str,
               char **pointers_out, char *bytes_out)
{
    size_t  delim_length        = strlen(delim);
    char   **pointers_out_start = pointers_out;

    assert(delim_length > 0);

    for (;;) {
        /* Find the next occurrence of the item delimiter. */
        const char *delim_pos = strstr(str, delim);

        /*
         * Emit the current output buffer position, since that is where the
         * next item will be written.
         */
        *pointers_out++ = bytes_out;

        if (delim_pos == NULL) {
            /*
             * No more item delimiters left.  Treat the rest of the input
             * string as the last item.
             */
            strcpy(bytes_out, str);
            return pointers_out - pointers_out_start;
        } else {
            /*
             * Item delimiter found.  The bytes leading up to it form the next
             * string.
             */
            while (str < delim_pos)
                *bytes_out++ = *str++;

            /* Don't forget the NUL terminator. */
            *bytes_out++ = '\0';

            /* Skip over the delimiter. */
            str += delim_length;
        }
    }
}

void SetNextTurn(S_BOARD *pos) {
    int turnSet = FALSE;
    while(!turnSet) {
        switch(pos->side) {
            case WHITE:
                if(pos->goldOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side=GOLD;
                break;
            case BLACK:
                if(pos->redOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side=RED;
                break;
            case GOLD:
                if(pos->blackOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side=BLACK;
                break;
            case RED:
                if(pos->whiteOut == TRUE) {
                    turnSet = FALSE;
                } else {
                    turnSet = TRUE;
                }
                pos->side=WHITE;
                break;
        }
    }
}

int CheckBoard(const S_BOARD *pos) {
    int t_pceNum[25] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int t_bigPce[4] = { 0, 0, 0, 0 };
    int t_majPce[4] = { 0, 0, 0, 0 };
    int t_minPce[4] = { 0, 0, 0, 0 };
    int t_material[4] = { 0, 0, 0, 0 };
    
    int sq196, t_piece, t_pce_num, sq288, colour, pcount;
    
    //TODO alternative to pawn bitboards
    
    // check piece lists
    for(t_piece = wP; t_piece <= rK; t_piece++) {
        for(t_pce_num = 0; t_pce_num < pos->pceNum[t_piece]; t_pce_num++) {
            sq288 = pos->pList[t_piece][t_pce_num];
            ASSERT(pos->pieces[sq288] == t_piece);
        }
    }
    
    // check piece count
    for(sq196 = 0; sq196 < 196; sq196++) {
        sq288 = SQ288(sq196);
        t_piece = pos->pieces[sq288];
        if(t_piece == OFFBOARD) {
            continue;
        }
        t_pceNum[t_piece]++;
        colour = PieceCol[t_piece];
        if( PieceBig[t_piece] == TRUE) t_bigPce[colour]++;
        if( PieceMin[t_piece] == TRUE) t_minPce[colour]++;
        if( PieceMaj[t_piece] == TRUE) t_majPce[colour]++;
        
        t_material[colour] += PieceVal[t_piece];
    }
    
    for(t_piece = wP; t_piece <= rK; t_piece++) {
        ASSERT(t_pceNum[t_piece] == pos->pceNum[t_piece]);
    }
    
    ASSERT(t_material[WHITE] == pos->material[WHITE] && t_material[BLACK] == pos->material[BLACK]);
    ASSERT(t_material[GOLD] == pos->material[GOLD] && t_material[RED] == pos->material[RED]);
    
    ASSERT(t_minPce[WHITE] == pos->minPce[WHITE] && t_minPce[BLACK] == pos->minPce[BLACK]);
    ASSERT(t_minPce[GOLD] == pos->minPce[GOLD] && t_minPce[RED] == pos->minPce[RED]);
    
    ASSERT(t_majPce[WHITE] == pos->majPce[WHITE] && t_majPce[BLACK] == pos->majPce[BLACK]);
    ASSERT(t_majPce[GOLD] = pos->majPce[GOLD] && t_majPce[RED] == pos->majPce[RED]);
    
    ASSERT(t_bigPce[WHITE] == pos->bigPce[WHITE] && t_bigPce[BLACK] == pos->bigPce[BLACK]);
    ASSERT(t_bigPce[GOLD] == pos->bigPce[GOLD] && t_bigPce[RED] == pos->bigPce[RED]);
    
    ASSERT (pos->side == WHITE || pos->side == BLACK || pos->side == GOLD || pos->side == RED);
    ASSERT(GeneratePosKey(pos) == pos->posKey);
    
    
    //TODO add conditions if the king is out or not
//    ASSERT(pos->pieces[pos->KingsSq[WHITE]] == wK);
//    ASSERT(pos->pieces[pos->KingsSq[BLACK]] == bK);
//    ASSERT(pos->pieces[pos->KingsSq[GOLD]] == gK);
//    ASSERT(pos->pieces[pos->KingsSq[RED]] == rK);
    
    
    return TRUE;
}

void UpdateListsMaterial(S_BOARD *pos) {
    int piece, sq, i, colour;
    
    for(i = 0; i <BRD_SQ_NUM; i++) {
        sq = i;
        piece = pos->pieces[i];
        if(piece != OFFBOARD && piece != EMPTY) {
            colour = PieceCol[piece];
            if(PieceBig[piece] == TRUE) pos->bigPce[colour]++;
            if(PieceMin[piece] == TRUE) pos->minPce[colour]++;
            if(PieceMaj[piece] == TRUE) pos->majPce[colour]++;
            
            pos->material[colour] += PieceVal[piece];
            
            // piece list
            pos->pList[piece][pos->pceNum[piece]] = sq;
            pos->pceNum[piece]++;
            
            if(piece == wK) pos->KingsSq[WHITE] = sq;
            if(piece == bK) pos->KingsSq[BLACK] = sq;
            if(piece == gK) pos->KingsSq[GOLD] = sq;
            if(piece == rK) pos->KingsSq[RED] = sq;
        }
    }
}

int ParseFen(char * fen, S_BOARD *pos) {
    ASSERT (fen != NULL);
    ASSERT (pos != NULL);
    
    int rank = RANK_14;
    char * srank;
    int file = FILE_A;
    char *piece;
    int cpiece;
    int count = 0;
    int i = 0, j = 0, k = 0, isnum = 0;
    int sq196 = 0;
    int sq288 = 0;
    
    ResetBoard(pos);
    
    char *ranks[2000];
    char rank_bytes[2000];
    char *files[2000];
    char file_bytes[2000];
    
    fen = str_replace(fen, ",/", "/" );
    
    count = explode("/", fen, ranks, rank_bytes);

    int emptySquares, fileIndex = 0, square;

    //Go through each rank
    for(i = RANK_1; i <= RANK_14; i++) {
        
        srank = ranks[i];
        
        count = explode(",", srank, files, file_bytes);
        fileIndex = 0;
        
        //Go through each file
        for (j = 0; j < count; j++) { 
            piece = files[j];
            isnum = atoi(piece);
            
            if(isnum) {
                emptySquares = isnum;
                fileIndex += emptySquares;
            } else {
                switch(piece[0]) {
                    case 'w':
                        switch(piece[1]) {
                            case 'P': cpiece = wP; break;
                            case 'R': cpiece = wR; break;
                            case 'N': cpiece = wN; break;
                            case 'B': cpiece = wB; break;
                            case 'K': cpiece = wK; break;
                            case 'Q': cpiece = wQ; break;
                        }
                        break;
                    case 'b':
                        switch(piece[1]) {
                            case 'P': cpiece = bP; break;
                            case 'R': cpiece = bR; break;
                            case 'N': cpiece = bN; break;
                            case 'B': cpiece = bB; break;
                            case 'K': cpiece = bK; break;
                            case 'Q': cpiece = bQ; break;
                        }
                        break;
                    case 'g':
                        switch(piece[1]) {
                            case 'P': cpiece = gP; break;
                            case 'R': cpiece = gR; break;
                            case 'N': cpiece = gN; break;
                            case 'B': cpiece = gB; break;
                            case 'K': cpiece = gK; break;
                            case 'Q': cpiece = gQ; break;
                        }
                        break;
                    case 'r':
                        switch(piece[1]) {
                            case 'P': cpiece = rP; break;
                            case 'R': cpiece = rR; break;
                            case 'N': cpiece = rN; break;
                            case 'B': cpiece = rB; break;
                            case 'K': cpiece = rK; break;
                            case 'Q': cpiece = rQ; break;
                        }
                        break;
                    default:
                        return 1;
                }
                
                sq196 = (13-i) * 14 + (fileIndex);
                sq288 = SQ288(sq196);
                pos->pieces[sq288] = cpiece; 
                fileIndex++;
            }
        }
    }
    
    /** REMOVE THE FOUR CORNERS **/
    pos->pieces[A1] = OFFBOARD;
    pos->pieces[A2] = OFFBOARD;
    pos->pieces[A3] = OFFBOARD;
    pos->pieces[B1] = OFFBOARD;
    pos->pieces[B2] = OFFBOARD;
    pos->pieces[B3] = OFFBOARD;
    pos->pieces[C1] = OFFBOARD;
    pos->pieces[C2] = OFFBOARD;
    pos->pieces[C3] = OFFBOARD;
    
    pos->pieces[A14] = OFFBOARD;
    pos->pieces[A13] = OFFBOARD;
    pos->pieces[A12] = OFFBOARD;
    pos->pieces[B14] = OFFBOARD;
    pos->pieces[B13] = OFFBOARD;
    pos->pieces[B12] = OFFBOARD;
    pos->pieces[C14] = OFFBOARD;
    pos->pieces[C13] = OFFBOARD;
    pos->pieces[C12] = OFFBOARD;
    
    pos->pieces[N14] = OFFBOARD;
    pos->pieces[N13] = OFFBOARD;
    pos->pieces[N12] = OFFBOARD;
    pos->pieces[M14] = OFFBOARD;
    pos->pieces[M13] = OFFBOARD;
    pos->pieces[M12] = OFFBOARD;
    pos->pieces[L14] = OFFBOARD;
    pos->pieces[L13] = OFFBOARD;
    pos->pieces[L12] = OFFBOARD;
    
    pos->pieces[N1] = OFFBOARD;
    pos->pieces[N2] = OFFBOARD;
    pos->pieces[N3] = OFFBOARD;
    pos->pieces[M1] = OFFBOARD;
    pos->pieces[M2] = OFFBOARD;
    pos->pieces[M3] = OFFBOARD;
    pos->pieces[L1] = OFFBOARD;
    pos->pieces[L2] = OFFBOARD;
    pos->pieces[L3] = OFFBOARD;
    
    
    pos->side = WHITE;
    pos->posKey = GeneratePosKey(pos);
    
    UpdateListsMaterial(pos);

    return 0;
}

void ResetBoard(S_BOARD *pos) {
    
    int i, j;
    for(i = 0; i < BRD_SQ_NUM; i++) {
        pos->pieces[i] = OFFBOARD;
    }
    
    for(i = 0; i < 196; i++) {
        pos->pieces[SQ288(i)] = EMPTY;
    }
    
    for(i = 0; i < 4; i++) {
        pos->bigPce[i] = 0;
        pos->majPce[i] = 0;
        pos->minPce[i] = 0;
        pos->material[i] = 0;
        for(j = 0; j < 196; j++) {
            pos->pawns[i][j] = 0;
        }
    }
    
    for(i = 0; i < 25; i++) {
        pos->pceNum[i] = 0;
    }
    
    pos->KingsSq[WHITE] = pos->KingsSq[BLACK] = pos->KingsSq[GOLD] = pos->KingsSq[RED] = NO_SQ;
    
    pos->side = ALL;
    
    pos->ply = 0;
    pos->hisPly = 0;
    
    pos->castlePerm = 0;
    
    pos->posKey = 0ULL;
    
    pos->whiteOut = FALSE;
    pos->blackOut = FALSE;
    pos->goldOut = FALSE;
    pos->redOut = FALSE;
}

void PrintBoard(const S_BOARD *pos) {
    int sq, file, rank, piece;
    printf("\nGame Board:\n\n");
    
    for(rank = RANK_14; rank >= RANK_1; rank--) {
        printf("%d  ", rank + 1);
        if(rank < RANK_10) {
            printf(" ");
        }
        
        for(file = FILE_A; file <= FILE_N; file++) {
            
            sq = FR2SQ(file, rank);
            piece = pos->pieces[sq];
            
            if(piece == OFFBOARD) {
                printf("  ^");
            } else {
                printf("%3c", PceChar[piece]);
            }
        }
        printf("\n");
    }
    
    printf("\n  ");
    for(file = FILE_A; file <= FILE_N; file++) {
        printf("%3c", FileChar[file]);
    }
    printf("\n");
    printf("side:%c\n", SideChar[pos->side]);
    
    printf("PosKey: %llX\n", pos->posKey);
}