#include "mainwindow.h"
#include "sudokuwidget.h"

#include <QtGui>

MainWindow::MainWindow()
{
    sw = new SudokuWidget;

    sw->updateFont("Arial");

    QMenu *file = new QMenu(tr("File"));
    QMenu *construction = new QMenu(tr("Construction"));
    QMenu *solution = new QMenu(tr("Solution"));
    language = new QMenu(tr("Numerals"));

    menuBar()->addMenu(file);
    menuBar()->addMenu(construction);
    menuBar()->addMenu(solution);
    menuBar()->addMenu(language);

    file->addAction(tr("Save..."),sw,SLOT(save()),QKeySequence::Save);
    file->addAction(tr("Load..."),sw,SLOT(load()),QKeySequence::Open);
    file->addSeparator();
    file->addAction(tr("Save SVG without answers..."),sw,SLOT(saveEmptyPuzzle()));
    file->addAction(tr("Save SVG with answers..."),sw,SLOT(saveSolvedPuzzle()));

    solution->addAction(tr("Step"),sw,SLOT(oneSolutionStep()),QKeySequence("S"));
    solution->addAction(tr("Solve"),sw,SLOT(solvePuzzle()),QKeySequence("A"));
    solution->addAction(tr("Clear Solution"),sw,SLOT(clearCalculated()),QKeySequence("C"));
    solution->addSeparator();
    solution->addAction(tr("Solve with report"),sw,SLOT(solvePuzzleWithReport()),QKeySequence("R"));

    construction->addAction(tr("Build Completed Puzzle"),sw,SLOT(populateGridRandomly()),QKeySequence("P"));
    construction->addAction(tr("Clear All"),sw,SLOT(clearAll()),QKeySequence("X"));

    // to do : store the starting character in the
    languageGroup = new QActionGroup(this);
    languageGroup->setExclusive(true);

    makeLanguageAction(tr("Western"),0x0030);
    makeLanguageAction(tr("Arabic"),0x0660);
    makeLanguageAction(tr("Persian"),0x06F0);
    makeLanguageAction(tr("Devanagari"),0x0966);
    makeLanguageAction(tr("Bengali"),0x09E6);
    makeLanguageAction(tr("Nko"),0x07C0);
    makeLanguageAction(tr("Gurmukhi"),0x0A66);
    makeLanguageAction(tr("Gujarati"),0x0AE6);
    makeLanguageAction(tr("Oriya"),0x0B66);
    makeLanguageAction(tr("Tamil"),0x0BE6);
    makeLanguageAction(tr("Telugu"),0x0C66);
    makeLanguageAction(tr("Kannada"),0x0CE6);
    makeLanguageAction(tr("Malayalam"),0x0D66);
    makeLanguageAction(tr("Thai"),0x0E50);
    makeLanguageAction(tr("Lao"),0x0ED0);
    makeLanguageAction(tr("Tibetan"),0x0F20);
    makeLanguageAction(tr("Myanmar"),0x1040);
    makeLanguageAction(tr("Myanmar Shan"),0x1090);
    makeLanguageAction(tr("Khmer"),0x17E0);
    makeLanguageAction(tr("Mongolian"),0x1810);
    makeLanguageAction(tr("Limbu"),0x1946);
    makeLanguageAction(tr("New Tai Lue"),0x19D0);
    makeLanguageAction(tr("Tai Tham Hora"),0x1A80);
    makeLanguageAction(tr("Tai Tham Tham"),0x1A90);
    makeLanguageAction(tr("Balinese"),0x1B50);
    makeLanguageAction(tr("Sundanese"),0x1BB0);
    makeLanguageAction(tr("Lepcha"),0x1C40);
    makeLanguageAction(tr("Ol Chiki"),0x1C50);
    makeLanguageAction(tr("Vai"),0xA620);
    makeLanguageAction(tr("Kayah Li"),0xA900);
    makeLanguageAction(tr("Javanese"),0xA9D0);
    makeLanguageAction(tr("Cham"),0xAA50);
    makeLanguageAction(tr("Meetei Mayek"),0xABF0);
//    makeLanguageAction(tr("Osmanya"),0x104A0);

    languageGroup->actions().first()->setChecked(true);

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

void MainWindow::makeLanguageAction(const QString& name, quint32 begin)
{
    QAction *action = new QAction(name,language);
    action->setData(begin);
    action->setCheckable(true);
    languageGroup->addAction(action);
    language->addAction(action);
}
