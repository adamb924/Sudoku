#include "sudokugrid.h"


void SudokuGrid::setOriginalCell(int x, int y, int value)
{
    grid[x][y] = value;
    original[x][y] = true;
}

void SudokuGrid::setCalculatedCell(int x, int y, int value)
{
    if(!original[x][y])
    {
        grid[x][y] = value;
        original[x][y] = false;
    }
}

int SudokuGrid::numberOfPossibleValues(int x, int y)
{
    int i, count=0;
    for(i=1; i<=9; i++)
        if(!rowContains(x,i) && !colContains(y,i) && !boxContains(x,y,i))
            count++;
    return count;
}

int SudokuGrid::findValue(int x, int y, int number=1)
{
    int i, ct=0;
    for(i=1; i<=9; i++)
    {
        if(!rowContains(x,i) && !colContains(y,i) && !boxContains(x,y,i))
        {
            ct++;
            if(ct==number)
                return i;
        }
    }
    return -1;
}

bool SudokuGrid::rowContains(int x, int value)
{
    int j;
    for(j=0; j<9; j++)
        if(grid[x][j]==value)
            return true;
    return false;
}

bool SudokuGrid::colContains(int y, int value)
{
    int i;
    for(i=0; i<9; i++)
        if(grid[i][y]==value)
            return true;
    return false;
}

bool SudokuGrid::boxContains(int x, int y, int value)
{
    int fromx, tox, fromy, toy;
    if(x<6)
    {
        if(x<3)
        {
            fromx = 0;
            tox = 2;
        }
        else
        {
            fromx = 3;
            tox = 5;
        }
    }
    else
    {
        fromx = 6;
        tox = 8;
    }

    if(y<6)
    {
        if(y<3)
        {
            fromy = 0;
            toy = 2;
        }
        else
        {
            fromy = 3;
            toy = 5;
        }
    }
    else
    {
        fromy = 6;
        toy = 8;
    }

    return rangeContains(fromx,tox,fromy,toy,value);
}

bool SudokuGrid::rangeContains(int fromx, int tox, int fromy, int toy, int value)
{
    int i, j;
    for(i=fromx; i<=tox; i++)
        for(j=fromy; j<=toy; j++)
            if(grid[i][j]==value)
                return true;
    return false;
}

bool SudokuGrid::solutionStep(bool justOne=false)
{
    int i, j, k, candidate, nposs, minpossibilities=81, mini=-1, minj=-1;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(cellValue(i,j)==-1)
            {
                nposs = numberOfPossibleValues(i,j);

                if(nposs < minpossibilities)
                {
                    minpossibilities = nposs;
                    mini = i;
                    minj = j;
                }
                if(nposs==1)
                {
                    setCalculatedCell(i,j,findValue(i,j));
                    if(justOne) { return true; }
                }
                if(nposs==0)
                {
                    return false;
                }
            }
        }
    }
    if(minpossibilities>1) // must be hard
    {
        for(k=1; k<=minpossibilities; k++) // cycle through all possibilities
        {
            candidate = findValue(mini,minj,k);
            SudokuGrid *hyp = new SudokuGrid(grid);
            hyp->setOriginalCell(mini,minj,candidate);

            if(hyp->solve())
            {
                copySolutionFromGrid(hyp->grid);
                return true;
            }
            delete hyp;
        }
    }
    return true;
}

void SudokuGrid::copySolutionFromGrid(int grd[][9])
{
    int i, j;
    for(i=0; i<9; i++)
        for(j=0; j<9; j++)
            if(!originalValue(i,j))
                setCalculatedCell(i,j,grd[i][j]);
}

bool SudokuGrid::solve()
{
    int last=81, current;
    do
    {
        if(!solutionStep())
            return false;
        current = numberOfUnknownCells();
        if(current==last)
            return false;
        last = current;
    }
    while(current>0);
    if(numberOfUnknownCells()==0)
        return true;
    else
        return false;
}

int SudokuGrid::cellValue(int x, int y)
{
    return grid[x][y];
}

bool SudokuGrid::originalValue(int x, int y)
{
    return original[x][y];
}

int SudokuGrid::numberOfUnknownCells()
{
    int i, j, count = 0;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(grid[i][j]==-1)
            {
                count++;
            }
        }
    }
    return count;
}

void SudokuGrid::clearCalculated()
{
    int i, j;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(!original[i][j])
            {
                grid[i][j] = -1;
            }
        }
    }

}

void SudokuGrid::clearAll()
{
    int i, j;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            grid[i][j] = -1;
            original[i][j] = false;
        }
    }
}
