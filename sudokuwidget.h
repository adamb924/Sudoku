#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QFont>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QWidget>

#include <QtGlobal>

#include "sudokugrid.h"

class QMouseEvent;
class QPaintEvent;

class SudokuWidget : public QWidget
{
    Q_OBJECT
public:
    enum Language { Western, Arabic, Persian, Devanagari, Bengali };
//                 enum Language { Western, Arabic, Persian, Nko, Devanagari, Bengali, Gurmukhi, Gujarati, Oriya, Tamil, Telugu, Kannada, Malayalam, Thai, Lao, Tibetan, Myanmar, MyanmarShan, Ethopic, Khmer, Mongolian, Limbu, NewTaiLue, TaiThamHora

    SudokuWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    QFont font();

public slots:
    void updateFont(const QString &font);
    void solutionStep(bool justOne);
    void oneSolutionStep();
    void solve();
    void clearCalculated();
    void clearAll();
    void save();
    void load();
    void saveSvg(bool withAnswers);
    void saveEmptyPuzzle();
    void saveSolvedPuzzle();

    void changeLanguage(QAction *action);


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
    SudokuGrid sg;
    QFont displayFont;
    int selx, sely;
    int margin, gridwidth, gridheight, cellwidth;
    QColor cBackground, cCellBackground, cMinorGrid, cMajorGrid, cGivenEntries, cCalculatedEntries;
    int wMajorGrid, wMinorGrid;

};

#endif
