//
// Created by Chy on 2022/12/3.
//

#ifndef GAME_OF_LIFE_GAMEOFLIFE_H
#define GAME_OF_LIFE_GAMEOFLIFE_H


#include <stdio.h>
#include <string.h>
#include <assert.h>
// Get Case introduction
#define LINE_LENGTH 80
// since the given Cell size  is 60x60 maximum, then we create 62x62 for holding boarder
#define CELL_SIZE 62
#define MAX_GENERATION 250

const char ALIVE = '*';
const char DEAD = '.';
const char CORNER = '+';
const char UP_DOWN_WALL = '-';
const char LEFT_RIGHT_WALL = '|';

typedef enum BOOL { false, true } Boolean;
typedef struct CELL Cell;
struct CELL{
    int row;
    int col;
    Boolean living;
};

int boardRows;
int boardCols;
// All generations information store by each generation number
char allGeneration[CELL_SIZE][CELL_SIZE][MAX_GENERATION];
// Show a cell with its information(position, living status)
Cell status[CELL_SIZE][CELL_SIZE];
int generationNum=0;
// Use to find the cycle begin number
int cycleStartNum=-1;

void createTable();
void createWall(int);
Boolean updateState();
int calcGeneNum();
int countAliveNeighbor(Cell);
Boolean calcNextState(Cell);
Cell setCell(int, int, Boolean);
void printGeneration(int);
void setToAlive(int, int, int);
void setToDead(int, int, int);
void checkCell(Cell);
void checkGenerations();


#endif //GAME_OF_LIFE_GAMEOFLIFE_H
