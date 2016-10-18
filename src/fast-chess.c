/*
 ============================================================================
 Name        : fast-chess.c
 Author      : Frederico Jordan <fredericojordan@gmail.com>
 Version     :
 Copyright   : Copyright (c) 2016 Frederico Jordan
 Description : Simple chess game!
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "fast-chess.h"

char FILES[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char RANKS[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

Bitboard FILES_BB[8] = { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
Bitboard RANKS_BB[8] = { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

int INITIAL_BOARD[NUM_SQUARES] = { WHITE|ROOK, WHITE|KNIGHT, WHITE|BISHOP, WHITE|QUEEN, WHITE|KING, WHITE|BISHOP, WHITE|KNIGHT, WHITE|ROOK,
                                   WHITE|PAWN, WHITE|PAWN,   WHITE|PAWN,   WHITE|PAWN,  WHITE|PAWN, WHITE|PAWN,   WHITE|PAWN,   WHITE|PAWN,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   EMPTY,      EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY,
                                   BLACK|PAWN, BLACK|PAWN,   BLACK|PAWN,   BLACK|PAWN,  BLACK|PAWN, BLACK|PAWN,   BLACK|PAWN,   BLACK|PAWN,
                                   BLACK|ROOK, BLACK|KNIGHT, BLACK|BISHOP, BLACK|QUEEN, BLACK|KING, BLACK|BISHOP, BLACK|KNIGHT, BLACK|ROOK };

char INITIAL_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int PIECE_VALUES[] = { 0, 100, 300, 300, 500, 900, 42000 };

int PAWN_BONUS[] = {0,   0,   0,   0,   0,   0,   0,   0,
					0,   0,   0, -40, -40,   0,   0,   0,
					1,   2,   3, -10, -10,   3,   2,   1,
					2,   4,   6,   8,   8,   6,   4,   2,
					3,   6,   9,  12,  12,   9,   6,   3,
					4,   8,  12,  16,  16,  12,   8,   4,
					5,  10,  15,  20,  20,  15,  10,   5,
					0,   0,   0,   0,   0,   0,   0,   0};

int KNIGHT_BONUS[] = {-10, -30, -10, -10, -10, -10, -30, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10, -10, -10, -10, -10, -10, -10, -10};

int BISHOP_BONUS[] = {-10, -10, -20, -10, -10, -20, -10, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,  10,  10,   5,   0, -10,
					  -10,   0,   5,   5,   5,   5,   0, -10,
					  -10,   0,   0,   0,   0,   0,   0, -10,
					  -10, -10, -10, -10, -10, -10, -10, -10};

int KING_BONUS[] = { 0,  20,  40, -20,   0, -20,  40,  20,
				   -20, -20, -20, -20, -20, -20, -20, -20,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40,
				   -40, -40, -40, -40, -40, -40, -40, -40};

int KING_ENDGAME_BONUS[] = { 0,  10,  20,  30,  30,  20,  10,   0,
							10,  20,  30,  40,  40,  30,  20,  10,
							20,  30,  40,  50,  50,  40,  30,  20,
							30,  40,  50,  60,  60,  50,  40,  30,
							30,  40,  50,  60,  60,  50,  40,  30,
							20,  30,  40,  50,  50,  40,  30,  20,
							10,  20,  30,  40,  40,  30,  20,  10,
							 0,  10,  20,  30,  30,  20,  10,   0};

int FLIP[] = {56,  57,  58,  59,  60,  61,  62,  63,
			  48,  49,  50,  51,  52,  53,  54,  55,
			  40,  41,  42,  43,  44,  45,  46,  47,
			  32,  33,  34,  35,  36,  37,  38,  39,
			  24,  25,  26,  27,  28,  29,  30,  31,
			  16,  17,  18,  19,  20,  21,  22,  23,
			   8,   9,  10,  11,  12,  13,  14,  15,
			   0,   1,   2,   3,   4,   5,   6,   7};


void getInitialGame(Game * game) {
	memcpy(game->board, INITIAL_BOARD, sizeof(game->board));
	game->toMove = WHITE;
	game->epSquare = -1;
	game->castlingRights = CASTLE_KINGSIDE_WHITE|CASTLE_QUEENSIDE_WHITE|CASTLE_KINGSIDE_BLACK|CASTLE_QUEENSIDE_BLACK;
	game->halfmoveClock = 0;
	game->fullmoveNumber = 1;

	game->moveListLen = 0;
	memset(game->moveList, 0, MAX_PLYS_PER_GAME*sizeof(int));
	memset(game->positionHistory, 0, MAX_PLYS_PER_GAME*MAX_FEN_LEN*sizeof(char));
	memcpy(game->positionHistory[0], INITIAL_FEN, sizeof(INITIAL_FEN));
}

void getFenGame(Game * game, char fen[]) {
	int fenLen = loadFen(game, fen);

	game->moveListLen = 0;
	memset(game->moveList, 0, MAX_PLYS_PER_GAME*sizeof(int));
	memset(game->positionHistory, 0, MAX_PLYS_PER_GAME*MAX_FEN_LEN*sizeof(char));
	memcpy(game->positionHistory[0], fen, fenLen);
}

int loadFen(Game * game, char fen[]) {
	// ===== BOARD =====
	memset(game->board, EMPTY, sizeof(game->board));

	int rank = 7;
	int boardPos = rank*8;
	char * charPos = fen;

	char pieceCode = *(charPos);

	while(pieceCode != ' ') {
		if (pieceCode == '/') {
			rank--;
			boardPos = rank*8;
		} else if (isdigit(pieceCode)) {
			int emptySquares = atoi(charPos);
			boardPos += emptySquares;
		} else {
			game->board[boardPos++] = char2piece(pieceCode);
		}

		pieceCode = *(++charPos);
	}


	// ===== TO MOVE =====
	char *nextFenField = strchr(fen, ' ') + 1;

	if (*nextFenField == 'b') {
		game->toMove = BLACK;
	} else {
		game->toMove = WHITE;
	}


	// ===== CASTLING RIGHTS =====
	nextFenField = strchr(nextFenField, ' ') + 1;

	if (*nextFenField == '-') {
		game->castlingRights = 0;
	} else {
		if (strchr(nextFenField, 'K'))
			game->castlingRights |= CASTLE_KINGSIDE_WHITE;
		if (strchr(nextFenField, 'Q'))
			game->castlingRights |= CASTLE_QUEENSIDE_WHITE;
		if (strchr(nextFenField, 'k'))
			game->castlingRights |= CASTLE_KINGSIDE_BLACK;
		if (strchr(nextFenField, 'q'))
			game->castlingRights |= CASTLE_QUEENSIDE_BLACK;
	}


	// ===== EN PASSANT =====
	nextFenField = strchr(nextFenField, ' ') + 1;

	if (*nextFenField == '-') {
		game->epSquare = -1;
	} else {
		game->epSquare = str2index(nextFenField);
	}

	// ===== HALF MOVE CLOCK =====
	nextFenField = strchr(nextFenField, ' ') + 1;

	game->halfmoveClock = atoi(nextFenField);

	// ===== FULL MOVE NUMBER =====
	nextFenField = strchr(nextFenField, ' ') + 1;

	game->fullmoveNumber = atoi(nextFenField);

	return 1+nextFenField-fen;
}

int toFen(char * fen, Game * game) {
	int charCount = 0;

	// ===== BOARD =====
	int rank = 7;
	int file = 0;
	int emptySquares = 0;

	while(rank >= 0) {
		int piece = game->board[8*rank+file];

		if ( piece == EMPTY ) {
			emptySquares++;
		} else {
			if (emptySquares != 0) {
				itoa(emptySquares, &fen[charCount++], 10);
				emptySquares = 0;
			}
			fen[charCount++] = piece2char(piece);
		}

		file++;
		if ( file > 7 ) {
			if (emptySquares != 0) {
				itoa(emptySquares, &fen[charCount++], 10);
				emptySquares = 0;
			}
			file = 0;
			rank--;
			fen[charCount++] = '/';
		}
	}
	fen[charCount-1] = ' ';


	// ===== TO MOVE =====
	if (game->toMove == BLACK) {
		fen[charCount++] = 'b';
	} else {
		fen[charCount++] = 'w';
	}
	fen[charCount++] = ' ';

	// ===== CASTLING RIGHTS =====
	if (game->castlingRights == 0) {
			fen[charCount++] = '-';
	} else {
		if (game->castlingRights & CASTLE_KINGSIDE_WHITE) {
			fen[charCount++] = 'K';
		}
		if (game->castlingRights & CASTLE_QUEENSIDE_WHITE) {
				fen[charCount++] = 'Q';
		}
		if (game->castlingRights & CASTLE_KINGSIDE_BLACK) {
			fen[charCount++] = 'k';
		}
		if (game->castlingRights & CASTLE_QUEENSIDE_BLACK) {
			fen[charCount++] = 'q';
		}
	}
	fen[charCount++] = ' ';

	// ===== EN PASSANT =====
	if (game->epSquare == -1) {
			fen[charCount++] = '-';
	} else {
		fen[charCount++] = getFile(game->epSquare);
		fen[charCount++] = getRank(game->epSquare);
	}
	fen[charCount++] = ' ';

	// ===== HALF MOVE CLOCK =====
	itoa(game->halfmoveClock, &fen[charCount++], 10);
	if (game->halfmoveClock >= 10) {
		charCount++;
		if (game->halfmoveClock >= 100) {
			charCount++;
		}
	}
	fen[charCount++] = ' ';

	// ===== FULL MOVE NUMBER =====
	itoa(game->fullmoveNumber, &fen[charCount++], 10);
	if (game->fullmoveNumber >= 10) {
		charCount++;
		if (game->fullmoveNumber >= 100) {
			charCount++;
		}
	}

	return charCount;
}

// ========= UTILITY =========

BOOL fromInitial(Game * game) {
	if ( strcmp(game->positionHistory[0], INITIAL_FEN) == 0 )
		return TRUE;
	else
		return FALSE;
}

Bitboard index2bb(int index) {
	Bitboard bb = 1;
	return bb << index;
}

int str2index(char *str) {
	int i, file_num=0, rank_num=0;
	for(i=0; i<8; i++) {
		if ( str[0] == FILES[i] )
			file_num = i;
		if ( str[1] == RANKS[i] )
			rank_num = i;
	}
	return 8*rank_num + file_num;
}

Bitboard str2bb(char *str) {
	return index2bb(str2index(str));
}

BOOL isSet(Bitboard bb, int index) {
	if (bb & index2bb(index))
		return TRUE;
	else
		return FALSE;
}

Bitboard lsb(Bitboard bb) {
	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		Bitboard bit = index2bb(i);
		if (bb & bit)
			return bit;
	}
	return 0;
}

Bitboard msb(Bitboard bb) {
	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		Bitboard bit = index2bb(63-i);
		if (bb & bit)
			return bit;
	}
	return 0;
}

int bb2index(Bitboard bb) {
	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		Bitboard bit = index2bb(i);
		if (bb & bit)
			return i;
	}
	return -1;
}

char * movelist2str(Game * game) {
	char * movestr = NULL;

	if (game->moveListLen == 0) {
		movestr = (char*) malloc(sizeof(char));
		movestr[0] = 0;
		return movestr;
	}

	movestr = (char*) malloc (5*game->moveListLen);

	int i;
	for (i=0;i<game->moveListLen;i++) {
		int leaving = getFrom(game->moveList[i]);
		int arriving = getTo(game->moveList[i]);
		movestr[5*i] = getFile(leaving);
		movestr[5*i+1] = getRank(leaving);
		movestr[5*i+2] = getFile(arriving);
		movestr[5*i+3] = getRank(arriving);
		movestr[5*i+4] = ' ';
	}

	movestr[5*game->moveListLen-1] = 0;

	return movestr;
}

BOOL startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);

    if (lenpre > lenstr)
    	return FALSE;

    return strncmp(pre, str, lenpre) == 0 ? TRUE : FALSE;
}

int countBookOccurrences(Game * game) {
    FILE * fp = fopen("book.txt", "r");

    if (fp == NULL)
        return 0;

    char * moveList = movelist2str(game);
    char *line = (char *) malloc(sizeof(char) * MAX_BOOK_ENTRY_LEN);
    int charPos = 0, occurrences = 0;

    while (TRUE) {
		char ch = getc(fp);
		line[charPos++] = ch;

		if ( ch == '\n' || ch == EOF ) {
			line[charPos-1] = '\0';

			if (startsWith(line, moveList) && strlen(line) > strlen(moveList)) {
				occurrences++;
			}

			if ( ch == EOF )
				break;

			charPos = 0;
		}
    }

    fclose(fp);
    free(line);
    free(moveList);

    return occurrences;
}

Move getBookMove(Game * game) {
	Move move = 0;
	int moveNum = rand() % countBookOccurrences(game);

	FILE * fp = fopen("book.txt", "r");

	if (fp == NULL)
		return 0;

	char * moveList = movelist2str(game);
	char *line = (char *) malloc(sizeof(char) * MAX_BOOK_ENTRY_LEN);
	int charPos = 0, occurrences = 0;

	while (TRUE) {
		char ch = getc(fp);
		line[charPos++] = ch;

		if ( ch == '\n' ) {
			line[charPos] = '\0';

			if (startsWith(line, moveList)) {
				if ( occurrences == moveNum ) {
					int ind = game->moveListLen*5;
					move = parseMove(&line[ind]);
					break;
				}
				occurrences++;
			}

			charPos = 0;
		}
	}

	fclose(fp);
	free(line);
	free(moveList);

	return move;
}

char getFile(int position) {
	int file = position%8;
	return FILES[file];
}

char getRank(int position) {
	int rank = (int) (position/8);
	return RANKS[rank];
}

Move generateMove(int leavingSquare, int arrivingSquare) {
	int leaving = (leavingSquare << 8);
	int arriving = arrivingSquare;
	return (Move) (leaving + arriving);
}

int getFrom(Move move) {
	return (move >> 8) & 0xFF;
}

int getTo(Move move) {
	return move & 0xFF;
}

int char2piece(char pieceCode) {
	switch(pieceCode) {
	case 'P':
		return WHITE|PAWN;
	case 'N':
		return WHITE|KNIGHT;
	case 'B':
		return WHITE|BISHOP;
	case 'R':
		return WHITE|ROOK;
	case 'Q':
		return WHITE|QUEEN;
	case 'K':
		return WHITE|KING;

	case 'p':
		return BLACK|PAWN;
	case 'n':
		return BLACK|KNIGHT;
	case 'b':
		return BLACK|BISHOP;
	case 'r':
		return BLACK|ROOK;
	case 'q':
		return BLACK|QUEEN;
	case 'k':
		return BLACK|KING;
	}
	return 0;
}

char piece2char(int piece) {
	switch(piece) {
	case WHITE|PAWN:
		return 'P';
	case WHITE|KNIGHT:
		return 'N';
	case WHITE|BISHOP:
		return 'B';
	case WHITE|ROOK:
		return 'R';
	case WHITE|QUEEN:
		return 'Q';
	case WHITE|KING:
		return 'K';
	case BLACK|PAWN:
		return 'p';
	case BLACK|KNIGHT:
		return 'n';
	case BLACK|BISHOP:
		return 'b';
	case BLACK|ROOK:
		return 'r';
	case BLACK|QUEEN:
		return 'q';
	case BLACK|KING:
		return 'k';
	case EMPTY:
		return '.';
	}
	return 0;
}

void printBitboard(Bitboard bitboard) {
	int rank, file;

	printf("\n");
	for (rank=0; rank<8; rank++) {
		printf("%d", 8-rank);
		for (file=0; file<8; file++) {
			if ( bitboard>>(file + (7-rank)*8) & 1 ) {
				printf(" #");
			} else {
				printf(" .");
			}
		}
		printf("\n");
	}
	printf("  a b c d e f g h\n");
	fflush(stdout);
}

void printBoard(int board[]) {
	int rank, file;

	printf("\n");
	for (rank=0; rank<8; rank++) {
		printf("%d", 8-rank);
		for (file=0; file<8; file++) {
			int position = file + (7-rank)*8;
			printf(" %c", piece2char(board[position]));
		}
		printf("\n");
	}
	printf("  a b c d e f g h\n");
	fflush(stdout);
}

void printGame(Game * game) {
	printf("Game -> %p (%u)", game, sizeof(*game));
	printBoard(game->board);
	printf("board -> %p (%u)\n", game->board, sizeof(game->board));
	printf("toMove = %d -> %p (%u)\n", game->toMove, &game->toMove, sizeof(game->toMove));
	printf("ep = %d -> %p (%u)\n", game->epSquare, &game->epSquare, sizeof(game->epSquare));
	printf("castle rights = %d -> %p (%u)\n", game->castlingRights, &game->castlingRights, sizeof(game->castlingRights));
	printf("half clock = %d -> %p (%u)\n", game->halfmoveClock, &game->halfmoveClock, sizeof(game->halfmoveClock));
	printf("full num = %d -> %p (%u)\n", game->fullmoveNumber, &game->fullmoveNumber, sizeof(game->fullmoveNumber));
	printf("moveListLen = %d -> %p (%u)\n", game->moveListLen, &game->moveListLen, sizeof(game->moveListLen));
	printf("moveList -> %p (%u)\n", game->moveList, sizeof(game->moveList));
	printf("positionHistory -> %p (%u)\n", game->positionHistory, sizeof(game->positionHistory));
	fflush(stdout);
}

Bitboard not(Bitboard bb) {
	return ~bb & ALL_SQUARES;
}

char opposingColor(char color) {
	switch(color) {
	case WHITE:
		return BLACK;
	case BLACK:
		return WHITE;
	}
	return 0;
}

int countBits(Bitboard bb) {
	int i, bitCount = 0;
	for (i=0; i<NUM_SQUARES; i++) {
		if (index2bb(i) & bb)
			bitCount++;
	}
	return bitCount;
}

// ====== BOARD FILTERS ======

Bitboard getColoredPieces(int board[], char color) {
	int i;
	Bitboard colored_squares = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if (board[i] != EMPTY && (board[i]&COLOR_MASK) == color)
			colored_squares |= index2bb(i);

	return colored_squares;
}

Bitboard getEmptySquares(int board[]) {
	int i;
	Bitboard empty_squares = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if (board[i] == EMPTY)
			empty_squares |= index2bb(i);

	return empty_squares;
}

Bitboard getOccupiedSquares(int board[]) {
	return not(getEmptySquares(board));
}

Bitboard getPieces(int board[], int pieceType) {
	int i;
	Bitboard pieces = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if ((board[i]&PIECE_MASK) == pieceType)
			pieces |= index2bb(i);

	return pieces;
}

Bitboard fileFilter(Bitboard positions) {
	Bitboard filter = 0;
	int i;

	for (i=0; i<8; i++)
		if (positions&FILES_BB[i])
			filter |= FILES_BB[i];
	return filter;
}

Bitboard rankFilter(Bitboard positions) {
	Bitboard filter = 0;
	int i;

	for (i=0; i<8; i++)
		if (positions&RANKS_BB[i])
			filter |= RANKS_BB[i];
	return filter;
}

char countPieces(Bitboard bitboard) {
	int i, count=0;
	for (i=0; i<NUM_SQUARES; i++) {
		if (index2bb(i)&bitboard)
			count += 1;
	}
    return count;
}

// ======= DIRECTIONS ========

Bitboard east(Bitboard bb) {
    return (bb << 1) & not(FILE_A);
}

Bitboard west(Bitboard bb) {
    return (bb >> 1) & not(FILE_H);
}

Bitboard north(Bitboard bb) {
    return (bb << 8) & not(RANK_1);
}

Bitboard south(Bitboard bb) {
    return (bb >> 8) & not(RANK_8);
}

Bitboard NE(Bitboard bb) {
    return north(east(bb));
}

Bitboard NW(Bitboard bb) {
    return north(west(bb));
}

Bitboard SE(Bitboard bb) {
    return south(east(bb));
}

Bitboard SW(Bitboard bb) {
    return south(west(bb));
}

Bitboard WNW(Bitboard moving_piece) {
    return moving_piece << 6 & not(FILE_G | FILE_H | RANK_1);
}

Bitboard ENE(Bitboard moving_piece) {
    return moving_piece << 10 & not(FILE_A | FILE_B | RANK_1);
}

Bitboard NNW(Bitboard moving_piece) {
    return moving_piece << 15 & not(FILE_H | RANK_1 | RANK_2);
}

Bitboard NNE(Bitboard moving_piece) {
    return moving_piece << 17 & not(FILE_A | RANK_1 | RANK_2);
}

Bitboard ESE(Bitboard moving_piece) {
    return moving_piece >> 6 & not(FILE_A | FILE_B | RANK_8);
}

Bitboard WSW(Bitboard moving_piece) {
    return moving_piece >> 10 & not(FILE_G | FILE_H | RANK_8);
}

Bitboard SSE(Bitboard moving_piece) {
    return moving_piece >> 15 & not(FILE_A | RANK_7 | RANK_8);
}

Bitboard SSW(Bitboard moving_piece) {
    return moving_piece >> 17 & not(FILE_H | RANK_7 | RANK_8);
}

// ========== PAWN ===========

Bitboard getPawns(int board[]) { return getPieces(board, PAWN); }

Bitboard pawnSimplePushes(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
		return north(moving_piece) & getEmptySquares(board);
	case BLACK:
		return south(moving_piece) & getEmptySquares(board);
	}
	return 0;
}

Bitboard pawnDoublePushes(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
		return north(pawnSimplePushes(moving_piece, board, color)) & (getEmptySquares(board) & RANK_4);
	case BLACK:
		return south(pawnSimplePushes(moving_piece, board, color)) & (getEmptySquares(board) & RANK_5);
	}
	return 0;
}

Bitboard pawnPushes(Bitboard moving_piece, int board[], char color) {
	return pawnSimplePushes(moving_piece, board, color) | pawnDoublePushes(moving_piece, board, color);
}

Bitboard pawnEastAttacks(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
        return NE(moving_piece);
	case BLACK:
        return SE(moving_piece);
	}
	return 0;
}

Bitboard pawnWestAttacks(Bitboard moving_piece, int board[], char color) {
	switch(color) {
	case WHITE:
        return NW(moving_piece);
	case BLACK:
        return SW(moving_piece);
	}
	return 0;
}

Bitboard pawnAttacks(Bitboard moving_piece, int board[], char color) {
	return pawnEastAttacks(moving_piece, board, color) | pawnWestAttacks(moving_piece, board, color);
}

Bitboard pawnSimpleCaptures(Bitboard moving_piece, int board[], char color) {
	return pawnAttacks(moving_piece, board, color) & getColoredPieces(board, opposingColor(color));
}

Bitboard pawnEpCaptures(Bitboard moving_piece, Game * game, char color) {
	if (game->epSquare == -1)
		return 0;

	Bitboard valid_ep_square = 0;

	switch(color) {
	case WHITE:
		valid_ep_square = index2bb(game->epSquare) & RANK_6;
		break;
	case BLACK:
		valid_ep_square = index2bb(game->epSquare) & RANK_3;
		break;
	}

	return pawnAttacks(moving_piece, game->board, color) & valid_ep_square;
}

Bitboard pawnCaptures(Bitboard moving_piece, Game * game, char color) {
    return pawnSimpleCaptures(moving_piece, game->board, color) | pawnEpCaptures(moving_piece, game, color);
}

Bitboard pawnMoves(Bitboard moving_piece, Game * game, char color) {
	return pawnPushes(moving_piece, game->board, color) | pawnCaptures(moving_piece, game, color);
}

BOOL isDoublePush(int leaving, int arriving) {
	if ( (index2bb(leaving)&RANK_2) && (index2bb(arriving)&RANK_4) )
		return TRUE;
	if ( (index2bb(leaving)&RANK_7) && (index2bb(arriving)&RANK_5) )
		return TRUE;
	return FALSE;
}

char getEpSquare(int leaving) {
	if (index2bb(leaving)&RANK_2)
		return leaving+8;
	if (index2bb(leaving)&RANK_7)
		return leaving-8;
	return -1;
}

BOOL isDoubledPawn(Bitboard position, int board[]) {
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	if (countPieces( getPawns(board)&getColoredPieces(board, pieceColor)&fileFilter(position) ) > 1)
		return TRUE;
	return FALSE;
}

BOOL isIsolatedPawn(Bitboard position, int board[]) {
	Bitboard sideFiles = fileFilter(east(position) | west(position));
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	if (countPieces( getPawns(board)&getColoredPieces(board, pieceColor)&sideFiles ) == 0)
		return TRUE;
	return FALSE;
}

BOOL isBackwardsPawn(Bitboard position, int board[]) {
	Bitboard squaresFilter = east(position) | west(position);
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	if ( pieceColor == BLACK ) {
		squaresFilter |= northRay(squaresFilter);
	} else {
		squaresFilter |= southRay(squaresFilter);
	}

	if (countPieces( getPawns(board)&getColoredPieces(board, pieceColor)&squaresFilter ) == 0)
		return TRUE;
	return FALSE;
}

BOOL isPassedPawn(Bitboard position, int board[]) {
	Bitboard squaresFilter = 0;
	char pieceColor = board[bb2index(position)]&COLOR_MASK;

	if ( pieceColor == BLACK ) {
		squaresFilter |= southRay(east(position)) | southRay(west(position)) | southRay(position);
	} else {
		squaresFilter |= northRay(east(position)) | northRay(west(position)) | northRay(position);
	}

	if (countPieces( getPawns(board)&getColoredPieces(board, opposingColor(pieceColor))&squaresFilter ) == 0)
		return TRUE;
	return FALSE;
}

BOOL isOpenFile(Bitboard position, int board[]) {
	if (countPieces( getPawns(board)&fileFilter(position) ) == 0)
		return TRUE;
	return FALSE;
}

BOOL isSemiOpenFile(Bitboard position, int board[]) {
	if (countPieces( getPawns(board)&fileFilter(position) ) == 1)
		return TRUE;
	return FALSE;
}

// ========== KNIGHT =========

Bitboard getKnights(int board[]) { return getPieces(board, KNIGHT); }

Bitboard knightAttacks(Bitboard moving_piece) {
    return NNE(moving_piece) | ENE(moving_piece) |
           NNW(moving_piece) | WNW(moving_piece) |
           SSE(moving_piece) | ESE(moving_piece) |
           SSW(moving_piece) | WSW(moving_piece);
}

Bitboard knightMoves(Bitboard moving_piece, int board[], char color) {
    return knightAttacks(moving_piece) & not(getColoredPieces(board, color));
}

// ========== KING ===========

Bitboard getKing(int board[], char color) {
	return getPieces(board, KING) & getColoredPieces(board, color);
}

Bitboard kingAttacks(Bitboard moving_piece) {
	Bitboard kingAtks = moving_piece | east(moving_piece) | west(moving_piece);
	kingAtks |= north(kingAtks) | south(kingAtks);
    return kingAtks & not(moving_piece);
}

Bitboard kingMoves(Bitboard moving_piece, int board[], char color) {
    return kingAttacks(moving_piece) & not(getColoredPieces(board, color));
}

BOOL canCastleKingside(Game * game, char color) {
	switch(color) {

	case WHITE:
		if ( (game->castlingRights&CASTLE_KINGSIDE_WHITE) &&
			 (game->board[str2index("f1")] == EMPTY) &&
			 (game->board[str2index("g1")] == EMPTY) &&
			 (!isAttacked(str2bb("e1"), game->board, opposingColor(color))) &&
			 (!isAttacked(str2bb("f1"), game->board, opposingColor(color))) &&
			 (!isAttacked(str2bb("g1"), game->board, opposingColor(color))) )
			return TRUE;
		else
			return FALSE;

	case BLACK:
		if ( (game->castlingRights&CASTLE_KINGSIDE_BLACK) &&
			 (game->board[str2index("f8")] == EMPTY) &&
			 (game->board[str2index("g8")] == EMPTY) &&
			 (!isAttacked(str2bb("e8"), game->board, opposingColor(color))) &&
			 (!isAttacked(str2bb("f8"), game->board, opposingColor(color))) &&
			 (!isAttacked(str2bb("g8"), game->board, opposingColor(color))) )
			return TRUE;
		else
			return FALSE;

	}
	return FALSE;
}

BOOL canCastleQueenside(Game * game, char color) {
	switch(color) {

	case WHITE:
		if ( (game->castlingRights&CASTLE_QUEENSIDE_WHITE) &&
			 (game->board[str2index("b1")] == EMPTY) &&
			 (game->board[str2index("c1")] == EMPTY) &&
			 (game->board[str2index("d1")] == EMPTY) &&
			 (!isAttacked(str2bb("c1"), game->board, opposingColor(color))) &&
			 (!isAttacked(str2bb("d1"), game->board, opposingColor(color))) &&
			 (!isAttacked(str2bb("e1"), game->board, opposingColor(color))) )
			return TRUE;
		else
			return FALSE;

	case BLACK:
		if ( (game->castlingRights&CASTLE_QUEENSIDE_BLACK) &&
				 (game->board[str2index("b8")] == EMPTY) &&
				 (game->board[str2index("c8")] == EMPTY) &&
				 (game->board[str2index("d8")] == EMPTY) &&
				 (!isAttacked(str2bb("c8"), game->board, opposingColor(color))) &&
				 (!isAttacked(str2bb("d8"), game->board, opposingColor(color))) &&
				 (!isAttacked(str2bb("e8"), game->board, opposingColor(color))) )
			return TRUE;
		else
			return FALSE;

	}
	return FALSE;
}

char removeCastlingRights(char original_rights, char removed_rights) {
    return (char) (original_rights & ~(removed_rights));
}

// ========== BISHOP =========

Bitboard getBishops(int board[]) { return getPieces(board, BISHOP); }

Bitboard NE_ray(Bitboard bb) {
	int i;
	Bitboard ray = NE(bb);

	for (i=0; i<6; i++) {
		ray |= NE(ray);
	}

	return ray & ALL_SQUARES;
}

Bitboard SE_ray(Bitboard bb) {
	int i;
	Bitboard ray = SE(bb);

	for (i=0; i<6; i++) {
		ray |= SE(ray);
	}

	return ray & ALL_SQUARES;
}

Bitboard NW_ray(Bitboard bb) {
	int i;
	Bitboard ray = NW(bb);

	for (i=0; i<6; i++) {
		ray |= NW(ray);
	}

	return ray & ALL_SQUARES;
}

Bitboard SW_ray(Bitboard bb) {
	int i;
	Bitboard ray = SW(bb);

	for (i=0; i<6; i++) {
		ray |= SW(ray);
	}

	return ray & ALL_SQUARES;
}

Bitboard NE_attack(Bitboard single_piece, int board[], char color) {
	Bitboard blocker = lsb(NE_ray(single_piece) & getOccupiedSquares(board));
	if (blocker) {
		return NE_ray(single_piece) ^ NE_ray(blocker);
	} else {
		return NE_ray(single_piece);
	}
}

Bitboard NW_attack(Bitboard single_piece, int board[], char color) {
	Bitboard blocker = lsb(NW_ray(single_piece) & getOccupiedSquares(board));
	if (blocker) {
		return NW_ray(single_piece) ^ NW_ray(blocker);
	} else {
		return NW_ray(single_piece);
	}
}

Bitboard SE_attack(Bitboard single_piece, int board[], char color) {
	Bitboard blocker = msb(SE_ray(single_piece) & getOccupiedSquares(board));
	if (blocker) {
		return SE_ray(single_piece) ^ SE_ray(blocker);
	} else {
		return SE_ray(single_piece);
	}
}

Bitboard SW_attack(Bitboard single_piece, int board[], char color) {
	Bitboard blocker = msb(SW_ray(single_piece) & getOccupiedSquares(board));
	if (blocker) {
		return SW_ray(single_piece) ^ SW_ray(blocker);
	} else {
		return SW_ray(single_piece);
	}
}

Bitboard diagonalAttacks(Bitboard single_piece, int board[], char color) {
    return NE_attack(single_piece, board, color) | SW_attack(single_piece, board, color);
}

Bitboard antiDiagonalAttacks(Bitboard single_piece, int board[], char color) {
    return NW_attack(single_piece, board, color) | SE_attack(single_piece, board, color);
}

Bitboard bishopAttacks(Bitboard moving_pieces, int board[], char color) {
    return diagonalAttacks(moving_pieces, board, color) | antiDiagonalAttacks(moving_pieces, board, color);
}

Bitboard bishopMoves(Bitboard moving_piece, int board[], char color) {
    return bishopAttacks(moving_piece, board, color) & not(getColoredPieces(board, color));
}

// ========== ROOK ===========

Bitboard getRooks(int board[]) { return getPieces(board, ROOK); }

Bitboard northRay(Bitboard moving_pieces) {
    Bitboard ray_atks = north(moving_pieces);

    int i;
    for (i=0; i<6; i++) {
        ray_atks |= north(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

Bitboard southRay(Bitboard moving_pieces) {
    Bitboard ray_atks = south(moving_pieces);

    int i;
    for (i=0; i<6; i++) {
        ray_atks |= south(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

Bitboard eastRay(Bitboard moving_pieces) {
    Bitboard ray_atks = east(moving_pieces);

    int i;
    for (i=0; i<6; i++) {
        ray_atks |= east(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

Bitboard westRay(Bitboard moving_pieces) {
    Bitboard ray_atks = west(moving_pieces);

    int i;
    for (i=0; i<6; i++) {
        ray_atks |= west(ray_atks);
    }

    return ray_atks & ALL_SQUARES;
}

Bitboard northAttack(Bitboard single_piece, int board[], char color) {
    Bitboard blocker = lsb(northRay(single_piece) & getOccupiedSquares(board));

    if (blocker)
        return northRay(single_piece) ^ northRay(blocker);
    else
        return northRay(single_piece);
}

Bitboard southAttack(Bitboard single_piece, int board[], char color) {
    Bitboard blocker = msb(southRay(single_piece) & getOccupiedSquares(board));

    if (blocker)
        return southRay(single_piece) ^ southRay(blocker);
    else
        return southRay(single_piece);
}

Bitboard fileAttacks(Bitboard single_piece, int board[], char color) {
    return northAttack(single_piece, board, color) | southAttack(single_piece, board, color);
}

Bitboard eastAttack(Bitboard single_piece, int board[], char color) {
    Bitboard blocker = lsb(eastRay(single_piece) & getOccupiedSquares(board));

    if (blocker)
        return eastRay(single_piece) ^ eastRay(blocker);
    else
        return eastRay(single_piece);
}

Bitboard westAttack(Bitboard single_piece, int board[], char color) {
    Bitboard blocker = msb(westRay(single_piece) & getOccupiedSquares(board));

    if (blocker)
        return westRay(single_piece) ^ westRay(blocker);
    else
        return westRay(single_piece);
}

Bitboard rankAttacks(Bitboard single_piece, int board[], char color) {
    return eastAttack(single_piece, board, color) | westAttack(single_piece, board, color);
}

Bitboard rookAttacks(Bitboard moving_piece, int board[], char color) {
    return fileAttacks(moving_piece, board, color) | rankAttacks(moving_piece, board, color);
}

Bitboard rookMoves(Bitboard moving_piece, int board[], char color) {
    return rookAttacks(moving_piece, board, color) & not(getColoredPieces(board, color));
}

// ========== QUEEN ==========

Bitboard getQueens(int board[]) { return getPieces(board, QUEEN); }

Bitboard queenAttacks(Bitboard moving_piece, int board[], char color) {
    return rookAttacks(moving_piece, board, color) | bishopAttacks(moving_piece, board, color);
}

Bitboard queenMoves(Bitboard moving_piece, int board[], char color) {
    return rookMoves(moving_piece, board, color) | bishopMoves(moving_piece, board, color);
}

// ======== MAKE MOVE ========

void movePiece(int board[], Move move) {
	board[getTo(move)] = board[getFrom(move)];
	board[getFrom(move)] = EMPTY;
}

void makeMove(Game * dstGame, Game * game, Move move) {
	memcpy(dstGame, game, sizeof(Game));
	int leavingSquare = getFrom(move);
	int arrivingSquare = getTo(move);
	int piece = game->board[leavingSquare];

	// ===== MOVE PIECE =====
	movePiece(dstGame->board, move);

	// ===== TO MOVE =====
	dstGame->toMove = opposingColor(game->toMove);

	// ===== MOVE COUNTS =====
	dstGame->halfmoveClock += 1;
	dstGame->moveListLen += 1;
	if (game->toMove == BLACK) {
		dstGame->fullmoveNumber += 1;
	}

	if (game->board[arrivingSquare] != EMPTY) {
		dstGame->halfmoveClock = 0;
	}

	// ===== PAWNS =====
	dstGame->epSquare = -1;
	if ( (piece&PIECE_MASK) == PAWN ) {
		dstGame->halfmoveClock = 0;

		if (arrivingSquare == game->epSquare) {
		    if (index2bb(game->epSquare)&RANK_3) {
				dstGame->board[(int)(game->epSquare+8)] = EMPTY;
		    }

		    if (index2bb(game->epSquare)&RANK_6) {
		    	dstGame->board[(int)(game->epSquare-8)] = EMPTY;
		    }
		}

		if (isDoublePush(leavingSquare, arrivingSquare)) {
			dstGame->epSquare = getEpSquare(leavingSquare);
		}

		if (index2bb(arrivingSquare)&(RANK_1|RANK_8)) {
			dstGame->board[arrivingSquare] = game->toMove|QUEEN;
		}

	}

	// ===== CASTLING =====
	if (leavingSquare == str2index("a1")) {
		dstGame->castlingRights = removeCastlingRights(dstGame->castlingRights, CASTLE_QUEENSIDE_WHITE);
	}
	else if (leavingSquare == str2index("h1")) {
		dstGame->castlingRights = removeCastlingRights(dstGame->castlingRights, CASTLE_KINGSIDE_WHITE);
	}
	else if (leavingSquare == str2index("a8")) {
		dstGame->castlingRights = removeCastlingRights(dstGame->castlingRights, CASTLE_QUEENSIDE_BLACK);
	}
	else if (leavingSquare == str2index("h8")) {
		dstGame->castlingRights = removeCastlingRights(dstGame->castlingRights, CASTLE_KINGSIDE_BLACK);
	}

	if ( piece == (WHITE|KING) ) {
		dstGame->castlingRights = removeCastlingRights(dstGame->castlingRights, (CASTLE_KINGSIDE_WHITE|CASTLE_QUEENSIDE_WHITE));
		if (leavingSquare == str2index("e1")) {

			if (arrivingSquare == str2index("g1"))
				movePiece(dstGame->board, generateMove(str2index("h1"), str2index("f1")));

            if (arrivingSquare == str2index("c1"))
				movePiece(dstGame->board, generateMove(str2index("a1"), str2index("d1")));
		}
	} else if ( piece == (BLACK|KING) ) {
		dstGame->castlingRights = removeCastlingRights(dstGame->castlingRights, CASTLE_KINGSIDE_BLACK|CASTLE_QUEENSIDE_BLACK);
		if (leavingSquare == str2index("e8")) {

			if (arrivingSquare == str2index("g8"))
				movePiece(dstGame->board, generateMove(str2index("h8"), str2index("f8")));

            if (arrivingSquare == str2index("c8"))
				movePiece(dstGame->board, generateMove(str2index("a8"), str2index("d8")));
		}
	}

	// ===== MOVE LIST =====
	dstGame->moveList[dstGame->moveListLen-1] = move;

	// ===== POSITION HISTORY =====
	toFen(dstGame->positionHistory[dstGame->moveListLen], dstGame);

}

void unmakeMove(Game * dstGame, Game * game) {
	if (game->moveListLen >= 2) {
		memcpy(dstGame, game, sizeof(Game));
		loadFen(dstGame, game->positionHistory[game->moveListLen-2]);
		dstGame->moveListLen -= 2;
		dstGame->moveList[game->moveListLen-1] = 0;
		dstGame->moveList[game->moveListLen-2] = 0;
		memset(dstGame->positionHistory[game->moveListLen], 0, MAX_FEN_LEN*sizeof(char));
		memset(dstGame->positionHistory[game->moveListLen-1], 0, MAX_FEN_LEN*sizeof(char));
	} else {
		memcpy(dstGame, game, sizeof(Game));
		loadFen(dstGame, game->positionHistory[0]);
		dstGame->moveListLen = 0;
		memset(dstGame->moveList, 0, MAX_PLYS_PER_GAME*sizeof(int));
		memset(&dstGame->positionHistory[1], 0, (MAX_PLYS_PER_GAME-1)*MAX_FEN_LEN*sizeof(char));
	}
}

// ======== MOVE GEN =========

Bitboard getMoves(Bitboard movingPiece, Game * game, char color) {
	int piece = game->board[bb2index(movingPiece)] & PIECE_MASK;

	switch (piece) {
	case PAWN:
		return pawnMoves(movingPiece, game, color);
	case KNIGHT:
		return knightMoves(movingPiece, game->board, color);
	case BISHOP:
		return bishopMoves(movingPiece, game->board, color);
	case ROOK:
		return rookMoves(movingPiece, game->board, color);
	case QUEEN:
		return queenMoves(movingPiece, game->board, color);
	case KING:
		return kingMoves(movingPiece, game->board, color);
	}
	return 0;
}

int pseudoLegalMoves(Move * moves, Game * game, char color) {
	int leavingSquare, arrivingSquare, moveCount = 0;

	for (leavingSquare=0; leavingSquare<NUM_SQUARES; leavingSquare++) {
		int piece = game->board[leavingSquare];

		if (piece != EMPTY && (piece&COLOR_MASK) == color) {
			Bitboard targets = getMoves(index2bb(leavingSquare), game, color);

			for (arrivingSquare=0; arrivingSquare<NUM_SQUARES; arrivingSquare++) {
				if (isSet(targets, arrivingSquare)) {
					moves[moveCount++] = generateMove(leavingSquare, arrivingSquare);
				}
			}

			if ( (piece&PIECE_MASK) == KING ) {
				if (canCastleKingside(game, color)) {
					moves[moveCount++] = generateMove(leavingSquare, leavingSquare+2);
				}
				if (canCastleQueenside(game, color)) {
					moves[moveCount++] = generateMove(leavingSquare, leavingSquare-2);
				}
			}
		}
	}

	return moveCount;
}

Bitboard getAttacks(Bitboard movingPiece, int board[], char color) {
	int piece = board[bb2index(movingPiece)] & PIECE_MASK;

	switch (piece) {
	case PAWN:
		return pawnAttacks(movingPiece, board, color);
	case KNIGHT:
		return knightAttacks(movingPiece);
	case BISHOP:
		return bishopAttacks(movingPiece, board, color);
	case ROOK:
		return rookAttacks(movingPiece, board, color);
	case QUEEN:
		return queenAttacks(movingPiece, board, color);
	case KING:
		return kingAttacks(movingPiece);
	}
	return 0;
}

int countAttacks(Bitboard target, int board[], char color) {
	int i, attackCount = 0;

	for (i=0; i<NUM_SQUARES; i++)
		if (board[i] != EMPTY && (board[i]&COLOR_MASK) == color)
			if ( getAttacks(index2bb(i), board, color) & target )
				attackCount += 1;

	return attackCount;
}

BOOL isAttacked(Bitboard target, int board[], char color) {
	if (countAttacks(target, board, color) > 0)
		return TRUE;
	else
		return FALSE;
}

BOOL isCheck(int board[], char color) {
	return isAttacked(getKing(board, color), board, opposingColor(color));
}

BOOL isLegalMove(Game * game, Move move) {
	Game newGame;
	makeMove(&newGame, game, move);
	if ( isCheck(newGame.board, game->toMove) )
		return FALSE;
	return TRUE;
}

int legalMoves(Move * legalMoves, Game * game, char color) {
	int i, legalCount = 0;

	Move pseudoMoves[MAX_BRANCHING_FACTOR];
	int pseudoCount = pseudoLegalMoves(pseudoMoves, game, color);

	for (i=0; i<pseudoCount; i++) {
		if (isLegalMove(game, pseudoMoves[i])) {
			legalMoves[legalCount++] = pseudoMoves[i];
		}
	}

	return legalCount;
}

int legalMovesCount(Game * game, char color) {
	int i, legalCount = 0;

	Move pseudoMoves[MAX_BRANCHING_FACTOR];
	int pseudoCount = pseudoLegalMoves(pseudoMoves, game, color);

	for (i=0; i<pseudoCount; i++) {
		if (isLegalMove(game, pseudoMoves[i])) {
			legalCount++;
		}
	}

	return legalCount;
}

// ====== GAME CONTROL =======

BOOL isCheckmate(Game * game) {
	if (isCheck(game->board, game->toMove) && legalMovesCount(game, game->toMove) == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL isStalemate(Game * game) {
	if (!isCheck(game->board, game->toMove) && legalMovesCount(game, game->toMove) == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL hasInsufficientMaterial(int board[]) {
	int pieceCount = countBits(getOccupiedSquares(board));

	if ( pieceCount <= 3 ) {
		if ( pieceCount == 2 || getKnights(board) != 0 || getBishops(board) != 0 )
			return TRUE;
	}

	return FALSE;
}

BOOL isEndgame(int board[]) {
	if (countBits(getOccupiedSquares(board)) <= ENDGAME_PIECE_COUNT)
		return TRUE;
	return FALSE;
}

BOOL isOver75MovesRule(Game * game) {
	if (game->halfmoveClock >= 150)
		return TRUE;
	else
		return FALSE;
}

BOOL hasGameEnded(Game * game) {
	if ( isCheckmate(game) ||
		 isStalemate(game) ||
		 hasInsufficientMaterial(game->board) ||
		 isOver75MovesRule(game) )
		return TRUE;
	else
		return FALSE;
}

void printOutcome(Game * game) {
	if (isCheckmate(game) && game->toMove == BLACK)
		printf("WHITE wins!\n");
	if (isCheckmate(game) && game->toMove == WHITE)
		printf("BLACK wins!\n");
	if (isStalemate(game))
		printf("Draw by stalemate!\n");
	if (hasInsufficientMaterial(game->board))
		printf("Draw by insufficient material!\n");
	if ( isOver75MovesRule(game) )
		printf("Draw by 75 move rule!\n");
	fflush(stdout);
}

// ========== EVAL ===========

int winScore(char color) {
	if (color == WHITE)
		return 10*PIECE_VALUES[KING];
	if (color == BLACK)
		return -10*PIECE_VALUES[KING];
	return 0;
}

int materialSum(int board[], char color) {
	int i, material = 0;

	for (i=0; i<NUM_SQUARES; i++) {
		if (board[i] != EMPTY && (board[i]&COLOR_MASK) == color) {
			material += PIECE_VALUES[board[i]&PIECE_MASK];
		}
	}

	return material;
}

int materialBalance(int board[]) {
	return materialSum(board, WHITE) - materialSum(board, BLACK);
}

int positionalBonus(int board[], char color) {
	int bonus = 0;

	int i;
	for (i=0; i<NUM_SQUARES; i++) {
		int piece = board[i];

		if (piece != EMPTY && (piece&COLOR_MASK) == color) {
			int pieceType = piece&PIECE_MASK;

			switch(pieceType) {
			case PAWN:
				if (color == WHITE) {
					bonus += PAWN_BONUS[i];
				} else {
					bonus += PAWN_BONUS[FLIP[i]];
				}

				if (isDoubledPawn(index2bb(i), board)) {
					bonus -= DOUBLED_PAWN_PENALTY/2;
				}
				if (isPassedPawn(index2bb(i), board)) {
					bonus += PASSED_PAWN_BONUS;
				}

				if (isIsolatedPawn(index2bb(i), board)) {
					bonus -= ISOLATED_PAWN_PENALTY;
				} else if (isBackwardsPawn(index2bb(i), board)) {
					bonus -= BACKWARDS_PAWN_PENALTY;
				}

				break;

			case KNIGHT:
				if (color == WHITE) {
					bonus += KNIGHT_BONUS[i];
				} else {
					bonus += KNIGHT_BONUS[FLIP[i]];
				}
				break;

			case BISHOP:
				if (color == WHITE) {
					bonus += BISHOP_BONUS[i];
				} else {
					bonus += BISHOP_BONUS[FLIP[i]];
				}
				break;

			case ROOK:
                if (isOpenFile(index2bb(i), board)) {
                    bonus += ROOK_OPEN_FILE_BONUS;
                } else if (isSemiOpenFile(index2bb(i), board)) {
                    bonus += ROOK_SEMI_OPEN_FILE_BONUS;
                }

				if (color == WHITE) {
					if (index2bb(i) & RANK_7) {
						bonus += ROOK_ON_SEVENTH_BONUS;
					}
				} else {
					if (index2bb(i) & RANK_2) {
						bonus += ROOK_ON_SEVENTH_BONUS;
					}
				}
				break;

			case KING:
				if (isEndgame(board)) {
					if (color == WHITE) {
						bonus += KING_ENDGAME_BONUS[i];
					} else {
						bonus += KING_ENDGAME_BONUS[FLIP[i]];
					}
				} else {
					if (color == WHITE) {
						bonus += KING_BONUS[i];
					} else {
						bonus += KING_BONUS[FLIP[i]];
					}
				}
			}
		}
	}

	return bonus;
}

int positionalBalance(int board[]) {
	return positionalBonus(board, WHITE) - positionalBonus(board, BLACK);
}

int evaluateEndNode(Game * game) {
	if (isCheckmate(game)) {
		return winScore(opposingColor(game->toMove));
	}
	if (isStalemate(game) || hasInsufficientMaterial(game->board) || isOver75MovesRule(game)) {
		return 0;
	}
	return 0;
}

int evaluateGame(Game * game) {
	if (hasGameEnded(game))
		return evaluateEndNode(game);
	else
		return materialBalance(game->board) + positionalBalance(game->board);
}

// ========= SEARCH ==========

Node simpleEvaluation(Game * game) {
	int bestScore = winScore(opposingColor(game->toMove));
	Move bestMove = 0;

	Move moves[MAX_BRANCHING_FACTOR];
	int moveCount = legalMoves(moves, game, game->toMove);

	Game newGame;
	int i;
	for (i=0; i<moveCount; i++) {
		makeMove(&newGame, game, moves[i]);
		int score = evaluateGame(&newGame);

		if (score == winScore(game->toMove)) {
			return (Node) { .move = moves[i], .score = score };
		}

		if ( (game->toMove == WHITE && score > bestScore) ||
			 (game->toMove == BLACK && score < bestScore) ) {
			bestScore = score;
			bestMove = moves[i];
		}
	}

	return (Node) { .move = bestMove, .score = bestScore };
}

Node alpha_beta(Game * game, char depth, int alpha, int beta, BOOL verbose) {
	if (hasGameEnded(game))
		return (Node) { .score = evaluateEndNode(game) };

	Node simpleNode = simpleEvaluation(game);

	if (depth == 1 || simpleNode.score == winScore(game->toMove))
		return simpleNode;

	Move bestMove = 0;

	Move moves[MAX_BRANCHING_FACTOR];
	int moveCount = legalMoves(moves, game, game->toMove);

	Game newGame;
	int i;
	for (i=0; i<moveCount; i++) {
		makeMove(&newGame, game, moves[i]);

		if (verbose) {
			int l = getFrom(moves[i]);
			int a = getTo(moves[i]);
			printf("(%d/%d) evaluating move: %c%c to %c%c = ", i+1, moveCount, getFile(l), getRank(l), getFile(a), getRank(a));
			fflush(stdout);
		}

		int score = alpha_beta(&newGame, depth-1, alpha, beta, FALSE).score;

		if (verbose) {
			printf("%.2f\n", score/100.0);
			fflush(stdout);
		}

		if (score == winScore(game->toMove)) {
			return (Node) { .move = moves[i], .score = score };
		}

		if (game->toMove == WHITE) {
			if (score > alpha) {
				alpha = score;
				bestMove = moves[i];
				if (alpha > beta) {
					break;
				}
			}
		} else if (game->toMove == BLACK) {
			if (score < beta) {
				beta = score;
				bestMove = moves[i];
				if (alpha > beta) {
					break;
				}
			}
		}
	}

	return (Node) { .move = bestMove, .score = game->toMove==WHITE?alpha:beta };
}

Move getRandomMove(Game * game) {
	Move moves[MAX_BRANCHING_FACTOR];
	int totalMoves = legalMoves(moves, game, game->toMove);
	int chosenMove = rand() % totalMoves;
	return moves[chosenMove];
}

Move getAIMove(Game * game, int depth) {
	printf("--- AI ---\n");
	fflush(stdout);

	if ( fromInitial(game) && countBookOccurrences(game) > 0 ) {
		printf("There are %d available book continuations.\n", countBookOccurrences(game));
		fflush(stdout);
		Move bookMove = getBookMove(game);
		printf("CHOSEN book move: %c%c to %c%c\n", getFile(getFrom(bookMove)), getRank(getFrom(bookMove)), getFile(getTo(bookMove)), getRank(getTo(bookMove)));
		fflush(stdout);
		return bookMove;
	}

	time_t startTime, endTime;
	printf("Searching with base depth = %d\n", depth);
	fflush(stdout);

	startTime = time(NULL);

//	Move move = getRandomMove(game);
//	Move move = simpleEvaluation(game).move;
//	Move move = minimax(game, AI_DEPTH).move;
	Node node = alpha_beta(game, depth, -INT32_MAX, INT32_MAX, TRUE);

	endTime = time(NULL);

	int l = getFrom(node.move);
	int a = getTo(node.move);
	printf("CHOSEN move: %c%c to %c%c in %d seconds [%.2f,%.2f]\n", getFile(l), getRank(l), getFile(a), getRank(a), (int) (endTime-startTime), evaluateGame(game)/100.0, node.score/100.0);
	fflush(stdout);

	return node.move;
}

Move parseMove(char * move) {
	int pos1 = str2index(&move[0]);
	int pos2 = str2index(&move[2]);
	return generateMove(pos1, pos2);
}

Move getPlayerMove() {
	char input[100];
	gets( input );
	return parseMove(input);
}

Move suggestMove(char fen[], int depth) {
	Game game;
	loadFen(&game, fen);
	return getAIMove(&game, depth);
}

// ===== PLAY LOOP (TEXT) ====

void playTextWhite(int depth) {
	printf("Playing as WHITE!\n");
	fflush(stdout);

	Game game, newGame;
	getInitialGame(&game);

	while(TRUE) {
		printBoard(game.board);
        if (hasGameEnded(&game))
        	break;

        makeMove(&newGame, &game, getPlayerMove(game));
        memcpy(&game, &newGame, sizeof(Game));

        printBoard(game.board);
        if (hasGameEnded(&game))
			break;

        makeMove(&newGame, &game, getAIMove(&game, depth));
        memcpy(&game, &newGame, sizeof(Game));
	}
	printOutcome(&game);
}

void playTextBlack(int depth) {
	printf("Playing as BLACK!\n");
	fflush(stdout);

	Game game, newGame;
	getInitialGame(&game);

	while(TRUE) {
        printBoard(game.board);
        if (hasGameEnded(&game))
        	break;

        makeMove(&newGame, &game, getAIMove(&game, depth));
		memcpy(&game, &newGame, sizeof(Game));

        printBoard(game.board);
        if (hasGameEnded(&game))
			break;

        makeMove(&newGame, &game, getPlayerMove(game));
		memcpy(&game, &newGame, sizeof(Game));
	}
	printOutcome(&game);
}

void playTextAs(char color, int depth) {
	if (color == WHITE)
		playTextWhite(depth);
	if (color == BLACK)
		playTextBlack(depth);
}

void playTextRandomColor(int depth) {
	char colors[] = {WHITE, BLACK};
	char color = colors[rand()%2];
	playTextAs(color, depth);
}

// ===========================

 /*
int main(int argc, char *argv[]) {
	srand(time(NULL));

	playTextRandomColor(DEFAULT_AI_DEPTH);

	return EXIT_SUCCESS;
}
// */
