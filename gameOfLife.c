//-----------------------------------------
// NAME: Huayi Chen
//-----------------------------------------


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




int main() {
    char c;
    do {
        createTable();
        printGeneration(calcGeneNum());

        c = getchar();
        if (c != EOF)
            printf("\n%c", c);

    }
    while  (c != EOF);

    printf("\n\nEnd of processing.\n");
    return 0;
}


// Create a board and initialize the Cell information
void createTable(){
    char ch;
    char input[LINE_LENGTH];
    int size;

    fgets(input, LINE_LENGTH, stdin);
    size=strlen(input);
    if (input[size-1]=='\n')
        input [size-1]=0;
    fflush(stdin);    //remove buffer

    scanf("%d %d", &boardRows, &boardCols);
    getchar();    // remove the input line '\n'

    assert(boardRows>0);
    assert(boardCols>0);
    assert(boardRows<CELL_SIZE-1);
    assert(boardCols<CELL_SIZE-1);

    if(boardRows>0 && boardCols>0 && boardRows<CELL_SIZE-1 && boardCols<CELL_SIZE-1){
        createWall(0);

        // create the content in the inner area
        for (int r = 1; r < boardRows+1; r++) {
            for (int c = 1; c < boardCols+1; c++) {
                ch = getchar();         // get next character in standard input

                // only 2 kinds of characters we can process
                assert(ch=='X' || ch==' ');
                if (ch == 'X') {
                    setToAlive(r, c, 0);
                }
                else if (ch == ' '){
                    setToDead(r, c, 0);
                }
            }
            getchar();
        }

    }

    checkGenerations();

    // print generation 0
    printf("%s\n", input);
    printf("Generation %d:\n", 0);
    for (int r=0 ; r<boardRows+2; r++){
        for (int c=0 ; c<boardCols+2; c++) {
            printf("%c", allGeneration[r][c][0]);
        }
        printf("\n");
    }

}


// Create boarder in num'th generation
void createWall(int num){
    // set boarder
    allGeneration[0][0][num] = allGeneration[boardRows + 1][0][num] = allGeneration[0][boardCols +1][num] = allGeneration[boardRows +1][boardCols + 1][num] = CORNER;
    for (int r = 1; r < boardRows + 1; r++) {
        allGeneration[r][0][num] = allGeneration[r][boardCols + 1][num] = LEFT_RIGHT_WALL;
    }
    for (int c = 1; c < boardCols + 1; c++) {
        allGeneration[0][c][num] = allGeneration[boardRows + 1][c][num] = UP_DOWN_WALL;
    }
}


// Print all last 10 generations until the end
void printGeneration(int end){
    int begin=1;

    if (end-10>0){
        begin = end-10;
#ifndef NDEBUG
        begin =1;
#endif
    }

    checkGenerations();

    for (int i = begin; i < end; i++) {
        printf("Generation %d:\n", i);
        createWall(i);

        for (int r = 0; r < boardRows+2; r++) {
            for (int c = 0; c < boardCols+2; c++) {
                printf("%c", allGeneration[r][c][i]);
            }
            printf("\n");
        }
    }
}


// set cell by given position and its status
Cell setCell(int row, int col, Boolean isAlive){
    Cell newCell={0, 0, false};
    assert(row>0);
    assert(row<boardRows+1);
    assert(col>0);
    assert(col<boardCols+1);
    if(row>0 && row<boardRows+1 && col>0 && col<boardCols+1) {
        newCell.row = row;
        newCell.col = col;
        newCell.living = isAlive;
    }
    checkCell(newCell);
    return newCell;
}


// make cell become Alive by given index and its generation number
void setToAlive(int row, int col, int numGeneration){
    allGeneration[row][col][numGeneration]=ALIVE;
    status[row][col] = setCell(row, col, true);
}


// make cell become Dead by given index and its generation number
void setToDead(int row, int col, int numGeneration){
    allGeneration[row][col][numGeneration]=DEAD;
    status[row][col] = setCell(row, col, false);
}


// count the alive neighbors by given index of Cell
int countAliveNeighbor(Cell theCell){
    int aliveNeighbors=0;
    Boolean neighborStatus;
    checkCell(theCell);

    for (int r=theCell.row-1; r<=theCell.row+1; r++) {
        for (int c = theCell.col-1; c <= theCell.col+1; c++) {
            neighborStatus = status[r][c].living;

            if ((r != theCell.row || c != theCell.col) && neighborStatus == true) {     // not count current Cell itself
                aliveNeighbors++;
            }
        }
    }
    return aliveNeighbors;
}


// calculate the next state of given Cell, and return true if next state is alive, false for dead
Boolean calcNextState(Cell curr){
    Boolean nextState;
    checkCell(curr);
    int livingNeighbors = countAliveNeighbor(curr);

    // Game of life rules
    if (curr.living)
        nextState = (livingNeighbors==2 || livingNeighbors==3);
    else
        nextState = (livingNeighbors==3);

    return nextState;
}


// return true if the next generation does not find the cycle in past generations
Boolean updateState(){
    Boolean continueUpdate = true;
    // temporary 2D array to store next generation status
    char nextGeneration[CELL_SIZE][CELL_SIZE];
    int number=0;

    // calculate all next states and store them in temporary 2D array
    for (int r = 1; r < boardRows+1; r++) {
        for (int c = 1; c < boardCols+1; c++) {
            if (calcNextState(status[r][c]))
                nextGeneration[r][c]=ALIVE;   // Alive
            else
                nextGeneration[r][c]=DEAD;   // Dead
        }
    }

    // Use correctness to match the states of next generation and all past generations, if finds cycle, does not update
    while  (number < generationNum && continueUpdate) {
        int sameStateNum=0;
        int r, c;

        for (r = 1; r < boardRows+1; r++) {
            for (c = 1; c < boardCols+1; c++) {
                // In the number'th generation, count the same state
                if (nextGeneration[r][c] == allGeneration[r][c][number]) {
                    sameStateNum++;
                }
            }
        }

        // Use temporary data to update status of each Cell in its feature
        for (r = 1; r < boardRows + 1; r++) {
            for (c = 1; c < boardCols + 1; c++) {
                assert(nextGeneration[r][c] == ALIVE || nextGeneration[r][c] == DEAD);
                if (nextGeneration[r][c] == ALIVE) {
                    setToAlive(r, c, generationNum);
                }
                if (nextGeneration[r][c] == DEAD) {
                    setToDead(r, c, generationNum);
                }
            }
        }

        // If the number of the same state are equal to total cell number in board, then find the first cycle begin
        if (sameStateNum==(r-1)*(c-1)){
            cycleStartNum=number;
            continueUpdate=false;
        }

        number++;
    }
    checkGenerations();

    return continueUpdate;
}


// Calculate the last generation number for ending print
int calcGeneNum(){
    int count=0;

    // Until the counts of generation find a cycle or reach the maximum
    while (count <= MAX_GENERATION) {
        generationNum++;
        count++;
        if (!updateState()) {
            generationNum=0;
            count++;
            break;
        }
    }

    // If the generation find the cycle, print information
    if (cycleStartNum != -1) {
        printf("Found a cycle between generation %d and generation %d\n", cycleStartNum, count-1);
        cycleStartNum = -1;
    }

    return count;
}


void checkCell(Cell theCell){
    assert(theCell.row>=1);
    assert(theCell.row<boardRows+1);
    assert(theCell.col>=1);
    assert(theCell.col<boardCols+1);
}


void checkGenerations(){
    assert(boardRows>0);
    assert(boardRows<CELL_SIZE-1);
    assert(boardCols>0);
    assert(boardCols<CELL_SIZE-1);

#ifndef NDEBUG

    for (int number = 0; number < generationNum; number++) {
        for (int r = 1; r < boardRows + 1; r++) {
            for (int c = 1; c < boardCols + 1; c++) {
                assert(allGeneration[r][c][number] == ALIVE || allGeneration[r][c][number] == DEAD);
            }
        }
    }
#endif
}
