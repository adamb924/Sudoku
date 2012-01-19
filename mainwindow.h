#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SudokuWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

private slots:
    void selectFont();
private:
    SudokuWidget *sw;
};

#endif // MAINWINDOW_H
