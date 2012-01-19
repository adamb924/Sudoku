#include "mainwindow.h"
#include "sudokuwidget.h"

#include <QtGui>

MainWindow::MainWindow()
{
    sw = new SudokuWidget;

    sw->updateFont("Arial");

    QMenu *file = new QMenu(tr("File"));
    QMenu *solution = new QMenu(tr("Solution"));
    QMenu *language = new QMenu(tr("Numerals"));

    menuBar()->addMenu(file);
    menuBar()->addMenu(solution);
    menuBar()->addMenu(language);

    file->addAction(tr("Save..."),sw,SLOT(save()));
    file->addAction(tr("Load..."),sw,SLOT(load()));
    file->addSeparator();
    file->addAction(tr("Save SVG without answers..."),sw,SLOT(saveEmptyPuzzle()));
    file->addAction(tr("Save SVG with answers..."),sw,SLOT(saveSolvedPuzzle()));

    solution->addAction(tr("Step"),sw,SLOT(oneSolutionStep()));
    solution->addAction(tr("Solve"),sw,SLOT(solve()));
    solution->addAction(tr("Clear Solution"),sw,SLOT(clearCalculated()));
    solution->addAction(tr("Clear All"),sw,SLOT(clearAll()));

    QActionGroup *languageGroup = new QActionGroup(this);
    languageGroup->setExclusive(true);
    QAction *western = new QAction(tr("Western"),language);
    western->setData(SudokuWidget::Western);
    QAction *arabic = new QAction(tr("Arabic"),language);
    arabic->setData(SudokuWidget::Arabic);
    QAction *persian = new QAction(tr("Persian"),language);
    persian->setData(SudokuWidget::Persian);
    QAction *devanagari = new QAction(tr("Devanagari"),language);
    devanagari->setData(SudokuWidget::Devanagari);
    QAction *bengali = new QAction(tr("Bengali"),language);
    bengali->setData(SudokuWidget::Bengali);

    languageGroup->addAction(western);
    languageGroup->addAction(arabic);
    languageGroup->addAction(persian);
    languageGroup->addAction(devanagari);
    languageGroup->addAction(bengali);

    western->setChecked(true);

    language->addAction(western);
    language->addAction(arabic);
    language->addAction(persian);
    language->addAction(devanagari);
    language->addAction(bengali);
    language->addSeparator();
    language->addAction(tr("Choose font..."),this,SLOT(selectFont()));

    connect(languageGroup,SIGNAL(triggered(QAction*)),sw,SLOT(changeLanguage(QAction*)));

    setCentralWidget(sw);
}

MainWindow::~MainWindow()
{
}

void MainWindow::selectFont()
{
    QFont font;
    bool ok = false;
    font = QFontDialog::getFont(&ok,sw->font(),this);
    if(ok)
        sw->updateFont(font.family());
}
