#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QFont>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;

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
	{
	    for(j=0; j<9; j++)
	    {
		if(grd[i][j] != -1)
		{
		    setOriginalCell(i,j,grd[i][j]);
		}
	    }
	}
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

class SudokuWidget : public QWidget
{
    Q_OBJECT

public:
    SudokuWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    QFont font();

public slots:
    void updateFont(const QFont &font);
    void solutionStep(bool justOne);
    void solve();
    void clearCalculated();
    void clearAll();
    void save();
    void load();

signals:
    void characterSelected(const QString &character);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent ( QMouseEvent * event );
    void keyReleaseEvent( QKeyEvent * event );

private:
    SudokuGrid sg;
    QFont displayFont;
    int selx, sely;
    int margin, gridwidth, gridheight, cellwidth;
    QColor cBackground, cCellBackground, cMinorGrid, cMajorGrid, cGivenEntries, cCalculatedEntries;
    int wMajorGrid, wMinorGrid;

};

#endif
