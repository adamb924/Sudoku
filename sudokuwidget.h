#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QFont>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QWidget>
#include <QList>

#include <QtGlobal>

class QMouseEvent;
class QPaintEvent;

class SudokuWidget : public QWidget
{
    Q_OBJECT
public:
    SudokuWidget(QWidget *parent = 0);
    SudokuWidget(int grd[][9]);
    QSize sizeHint() const;
    QFont font();

public slots:
    void updateFont(const QString &font);
    void oneSolutionStep();
    void solvePuzzle();
    void clearCalculated();
    void save();
    void load();
    void saveSvg(bool withAnswers);
    void saveEmptyPuzzle();
    void saveSolvedPuzzle();

    void populateGridRandomly();

    void changeLanguage(QAction *action);
    void clearAll();

    void randomPopulationStep();

    void solvePuzzleWithReport();

private:
//    QList<int> aPossibleMoveHistory;
    QVector<double> aSteps;
    QVector<double> aPossibleMoveHistory;

    int numberOfPossibleValues(int x, int y) const;
    int findValue(int x, int y, int number=1) const;
    int cellValue(int x, int y) const;
    int numberOfUnknownCells() const;
    bool originalValue(int x, int y) const;
    bool rowContains(int x, int value) const;
    bool colContains(int y, int value) const;
    bool boxContains(int x, int y, int value) const;
    bool rangeContains(int fromx, int tox, int fromy, int toy, int value) const;
    void nthUnknownCell(int &i, int &j, int which = 1) const;

    void copySolutionFromGrid(int grd[][9]);
    void setOriginalCell(int x, int y, int value);
    void setCalculatedCell(int x, int y, int value);
    bool solutionStep(bool justOne, bool stochastic);
    bool solve(bool stepwise, bool stochastic);
    void setNonEmptyToOriginal();
    int numberOfPossibleMoves() const;

    int randomUpTo(int ceiling) const;




    int grid[9][9];
    bool original[9][9];

signals:
    void characterSelected(const QString &character);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent ( QMouseEvent * event );
    void keyReleaseEvent( QKeyEvent * event );

private:
    qreal dCellSize;
    int nFontSize;

    quint32 cLanguageOffset;
    QFont displayFont;
    int selx, sely;
    int margin, gridwidth, gridheight, cellwidth;
    QColor cBackground, cCellBackground, cMinorGrid, cMajorGrid, cGivenEntries, cCalculatedEntries;
    int wMajorGrid, wMinorGrid;

};

#endif
