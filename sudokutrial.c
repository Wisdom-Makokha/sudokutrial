// Program to check if my written code for the sudoku project is sound
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include<time.h>
#include<conio.h>

#define COUNTER(size, name) for (int name = 0; name < size; name++)

int main(void)
{
    void createboard(int sudokugrid[9][9], int difficulty);
    int printgrid(int sudokugrid[9][9]);

    int sudokugrid[9][9];
    int difficulty[6] = {20, 29, 38, 47, 56, 64};
    
    srand(time(NULL));
    COUNTER(9, i)
        COUNTER(9, j)
            sudokugrid[i][j] = 0;

    printgrid(sudokugrid);

    createboard(sudokugrid, difficulty[0]);
    printgrid(sudokugrid);

    return EXIT_SUCCESS;
}


//This function prints out the generated sudokugrid
int printgrid(int sudokugrid[9][9])
{
    COUNTER(9, i)
    {
        COUNTER(9, j)
            printf("%d ", sudokugrid[i][j]);

        printf("\n");
    }
    printf("\n");
}

//This function checks if the number is in the specified sudoku grid row
//it returns FALSE if the number is not found
//it returns TRUE if the number is found
bool checkinrow(int row, int sudokugrid[9][9], int number)
{
    COUNTER(9, i)
    if (number == sudokugrid[row][i])
    {
        // fprintf(stderr, "The number already exists in this row!!\n");
        return true;
    }
    return false;
}


//This function checks if the number is in the specified sudoku grid column
//it returns FALSE if the number is not found
//it returns TRUE if the number is found
bool checkincolumn(int column, int sudokugrid[9][9], int number)
{
    COUNTER(9, i)
    if (number == sudokugrid[i][column])
    {
        // fprintf(stderr, "The number already exists in this column!!\n");
        return true;
    }
    
    return false;
}

//This function checks if the number is in a square within a sudoku grid
//A square being one of the nine 3 x 3 boxes within a sudoku grid
//it returns FALSE if the number is not found
//it returns TRUE if the number is found
bool checkinsquare(int rowstart, int columnstart, int sudokugrid[9][9], int number)
{
    COUNTER(3, i)
    COUNTER(3, j)
    if (number == sudokugrid[rowstart + i][columnstart + j])
    {
        // fprintf(stderr, "The number already exists in this square!!\n");
        return true;
    }
    
    return false;
}

//This function runs the row, column and square check
//the AND condition ensures that if the number is found in either the row, column or square the function returns a false
//a true is returned if the number is not found
bool checkifsafe(int row, int column, int number, int sudokugrid[9][9])
{
    bool checkinsquare(int rowstart, int columnstart, int sudokugrid[9][9], int number);
    bool checkincolumn(int column, int sudokugrid[9][9], int number);
    bool checkinrow(int row, int sudokugrid[9][9], int number);

    if(!checkinrow(row, sudokugrid, number) &&
        !checkincolumn(column, sudokugrid, number) &&
         !checkinsquare(((row / 3) * 3), ((column / 3) * 3), sudokugrid, number))
        return true;
    else
    {
        // fprintf(stderr, "The number is not safe!!\n");
        return false;
    }
}

//this function can fill one of the nine squares of the sudoku grid
void fillsquare(int rowstart, int columnstart, int sudokugrid[9][9])
{
    //random number is used to generate random numbers to place in the point
    //checkinsquare is used to not duplicate a number
    int randomnumber(bool limit, int probabilitycheck[9], int *risk);
    bool checkifsafe(int row, int column, int number, int sudokugrid[9][9]);
    int printgrid(int sudokugrid[9][9]);

    //probabilitycheck is used to control the chance of each number appearing
    //foundcount counts the picked numbers
    int probabilitycheck[9];
    int foundcount = 0;
    int num;
    bool issafe;
    int risk = 0;
    
    COUNTER(9, i)
        probabilitycheck[i] = 1;

    //a random number is obtained from the randomnumber function to put at a point
    //the number is checked to ensure it hasn't been used before then put at the point
    //foundcount stops the do...while loop when all nine numbers are found
    COUNTER(3, i)
    {
        COUNTER(3, j)
        {
            do
            {
                num = randomnumber(true, probabilitycheck, &risk);
                issafe = checkifsafe(rowstart + i, columnstart + j, num, sudokugrid);
            } while (!issafe && foundcount != 8);
            
            foundcount++;
            sudokugrid[rowstart + i][columnstart + j] = num;
        }
    }
}


//this function returns a random number between 1 and 9
int randomnumber(bool limit, int probabilitycheck[9], int *risk)
{
    int selectnum;
    double maxnum = 0;
    int prime = 100;
    double offset[9];

    //a random number is selected and the range limited to between 0 and prime and stored in offset
    //the offset is then found as a fraction of prime and multiplied by probabilitycheck
    //probabilitycheck is used to control the selection of numbers when limit is true
    COUNTER(9, i)
    {
        offset[i] = (rand() % (prime + 1 - 0)) + 0;
        offset[i] = (offset[i] / prime) * probabilitycheck[i];
    }

    //the largest of our randomly affected offsets is picked
    COUNTER(9, i)
    {
        if (offset[i] > maxnum)
        {
            maxnum = offset[i];
            selectnum = i;
        }
    }

    if(limit)
    {
        probabilitycheck[selectnum] = 0;
        *risk++;
    }

    if(*risk == 9)
    {
        COUNTER(9, i)
            probabilitycheck[i] = 1;
        
        *risk = 0;
    }

    //if limit is set to true the selected number is given a probabilitycheck of 0
    //this ensures it cannot be selected again because of our largest number selection
    //this should also reduce the number of loops needed to fill a square
 
    return selectnum + 1;
}

//this function is to be used to remove the digits from an already filled board to generate the sudoku puzzle to solve
//the larger the count the higher the difficulty
void removedigits(int count, int sudokugrid[9][9])
{
    int randomnumber(bool limit, int probabilitycheck[9], int *risk);

    int probabilitycheck[9];
    COUNTER(9, i)
        probabilitycheck[i] = 1;

    int row, col;

    //a  random row and column is picked and checked if they are 0
    //if the point is not zero it is turned to a 0, removing the digit
    while (count != 0)
    {
        row = (randomnumber(false, probabilitycheck, NULL)) - 1;
        col = (randomnumber(false, probabilitycheck, NULL)) - 1;

        if (sudokugrid[row][col] != 0)
        {
            count--;
            sudokugrid[row][col] = 0;
        }
    }
}

int solve(int sudokugrid[9][9], int row, int column)
{
    bool checkifsafe(int row, int column, int number, int sudokugrid[9][9]);
    bool checkfilled(int sudokugrid[9][9], int row, int column);
    
    if(row == 8 && column == 8)
        return 1;

    if(column == 9)
    {
        row++;
        column = 0;
    }

    if(checkfilled(sudokugrid, row, column))
        return solve(sudokugrid, row, column + 1);

    COUNTER(9, number)
    {
        if(checkifsafe(row, column, number + 1, sudokugrid))
        {
            sudokugrid[row][column] = number + 1;

            if(solve(sudokugrid, row, column + 1) == 1)
                return 1;
        }

        sudokugrid[row][column] = 0;
    }

    return 0;
}

void solvegrid(int sudokugrid[9][9])
{
    int countmissing(int sudokugrid[9][9]);
    void reducecandidates(int sudokugrid[9][9], bool candidates[9][9][9]);
    void addnumberstogrid(int sudokugrid[9][9], bool candidates[9][9][9]);
    void printcandidates(bool candidates[9][9][9]);

    int missing = 0, tries = 0;
    bool candidates[9][9][9];

    COUNTER(9, i)
    COUNTER(9, j)
    COUNTER(9, k)
        candidates[i][j][k] = true;
    
    // printcandidates(candidates);
    do
    {
        missing = countmissing(sudokugrid);

        reducecandidates(sudokugrid, candidates);
        addnumberstogrid(sudokugrid, candidates);
        tries++;
    } while (missing != 0 && tries < 10000);

    // printcandidates(candidates);
}

int countmissing(int sudokugrid[9][9])
{
    int count = 0;
    COUNTER(9, i)
    COUNTER(9, j)
    if (sudokugrid[i][j] == 0)
        count++;

    return count;
}

void reducecandidates(int sudokugrid[9][9], bool candidates[9][9][9])
{
    void removecolumncandidates(bool candidates[9][9][9], int number, int column, int row);
    void removerowcandidates(bool candidates[9][9][9], int number, int row, int column);
    void removesquarecandidates(bool candidates[9][9][9], int number, int rowstart, int columnstart);

    COUNTER(9, i)
    COUNTER(9, j)
        if(sudokugrid[i][j] != 0)
        {
            COUNTER(9, k)
            {
                if((sudokugrid[i][j] - 1) == k)
                    continue;
                
                candidates[i][j][k] = false;
            }
        }
    
    COUNTER(9, i)
    COUNTER(9, j)
    {
        if (sudokugrid[i][j] == 0)
            continue;
        else
        {
            removerowcandidates(candidates, sudokugrid[i][j], i, j);
            removecolumncandidates(candidates, sudokugrid[i][j], j, i);
            removesquarecandidates(candidates, sudokugrid[i][j], i, j);
        }
    }
}

void removecolumncandidates(bool candidates[9][9][9], int number, int column, int row)
{
    COUNTER(9, i)
    {
        if(i == row)
            continue;
        
        candidates[i][column][number - 1] = false;
    }
}

void removerowcandidates(bool candidates[9][9][9], int number, int row, int column)
{
    COUNTER(9, i)
    {
        if(i == column)
            continue;

        candidates[row][i][number - 1] = false;
    }
}

void removesquarecandidates(bool candidates[9][9][9], int number, int row, int column)
{
    int rowstart = (row / 3) * 3;
    int columnstart = (column / 3) * 3;
    
    COUNTER(3, i)
    COUNTER(3, j)
    {
        if((rowstart + i) == row && (columnstart + i) == column)
            continue;
        
        candidates[rowstart + i][columnstart + j][number - 1] = false;
    }
}

void addnumberstogrid(int sudokugrid[9][9], bool candidates[9][9][9])
{
    int checkforsinglecandidates(bool candidates[9][9][9], int row, int column);
    bool checkifsafe(int row, int column, int number, int sudokugrid[9][9]);

    int number;

    COUNTER(9, i)
    COUNTER(9, j)
    {
        number = checkforsinglecandidates(candidates, i, j);

        if (number == 0)
            continue;
        else
        {
            if (checkifsafe(i, j, number, sudokugrid))
                sudokugrid[i][j] = number;
        }
    }
}

int checkforsinglecandidates(bool candidates[9][9][9], int row, int column)
{
    int countcandidates = 0;
    int likelycandidate = 0;

    COUNTER(9, i)
    {
        if (!candidates[row][column][i])
            continue;
        else
        {
            countcandidates++;
            likelycandidate = i;
        }
    }

    if (countcandidates == 1)
    {
        candidates[row][column][likelycandidate] = false;
        return likelycandidate + 1;
    }
    else
        return 0;
}

void createboard(int sudokugrid[9][9], int difficulty)
{
    void solvegrid(int sudokugrid[9][9]);
    void fillsquare(int rowstart, int columnstart, int sudokugrid[9][9]);
    void removedigits(int count, int sudokugrid[9][9]);
    int solve(int sudokugrid[9][9], int row, int column);

    COUNTER(3, i)
        fillsquare(i * 3, i * 3, sudokugrid);

    solve(sudokugrid, 0, 0);
    // removedigits(difficulty, sudokugrid);
}

void printcandidates(bool candidates[9][9][9])
{
    COUNTER(9, i)
    COUNTER(9, j)
    {
        COUNTER(9, k)
            if(candidates[i][j][k])
                printf("%d ", k+1);
        
        printf("\n");
    }

    printf("\n");
    printf("\n");
}


bool checkfilled(int sudokugrid[9][9], int row, int column)
{
    if(sudokugrid[row][column] > 0)
        return true;
    else
        return false;
}