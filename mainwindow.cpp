#include "mainwindow.h"
#include "sudokuwidget.h"

#include <QHBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    sw = new SudokuWidget;

    QMenuBar *menu = new QMenuBar;
    QAction *step = new QAction("Step",menu);
    QAction *solve = new QAction("Solve",menu);
    QAction *clearcalculated = new QAction("Clear Solution",menu);
    QAction *clearall = new QAction("Clear All",menu);
    QAction *save = new QAction("Save",menu);
    QAction *load = new QAction("Load",menu);
    menu->addAction(step);
    menu->addAction(solve);
    menu->addAction(clearcalculated);
    menu->addAction(clearall);
    menu->addAction(save);
    menu->addAction(load);
    setMenuBar(menu);

    QHBoxLayout *horizontal = new QHBoxLayout;
    horizontal->addWidget(sw);

    widget->setLayout(horizontal);

//    connect(step,SIGNAL(triggered()),sw,SLOT(solutionStep(bool)));
    connect(step,SIGNAL(triggered()),this,SLOT(step()));
    connect(solve,SIGNAL(triggered()),sw,SLOT(solve()));
    connect(clearcalculated,SIGNAL(triggered()),sw,SLOT(clearCalculated()));
    connect(clearall,SIGNAL(triggered()),sw,SLOT(clearAll()));
    connect(save,SIGNAL(triggered()),sw,SLOT(save()));
    connect(load,SIGNAL(triggered()),sw,SLOT(load()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
}

void MainWindow::step()
{
    sw->solutionStep(true);
}
