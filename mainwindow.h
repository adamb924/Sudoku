#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SudokuWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    SudokuWidget *sw;

protected:
    void changeEvent(QEvent *e);
private slots:
    void step();
};

#endif // MAINWINDOW_H
