#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

class SudokuGrid {

public:
    SudokuGrid()
    {
        clearAll();
    }
    SudokuGrid(int grd[][9])
    {
        int i, j;
        clearAll();
        for(i=0; i<9; i++)
            for(j=0; j<9; j++)
                if(grd[i][j] != -1)
                    setOriginalCell(i,j,grd[i][j]);
    }

    void copySolutionFromGrid(int grd[][9]);
    void setOriginalCell(int x, int y, int value);
    void setCalculatedCell(int x, int y, int value);
    int numberOfPossibleValues(int x, int y);
    int findValue(int x, int y, int number);
    bool solutionStep(bool justOne);
    bool solve();
    int cellValue(int x, int y);
    int numberOfUnknownCells();
    bool originalValue(int x, int y);
    bool rowContains(int x, int value);
    bool colContains(int y, int value);
    bool boxContains(int x, int y, int value);
    bool rangeContains(int fromx, int tox, int fromy, int toy, int value);
    void clearCalculated();
    void clearAll();

private:
    int grid[9][9];
    bool original[9][9];
};

#endif // SUDOKUGRID_H
