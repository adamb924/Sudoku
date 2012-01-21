#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SudokuWidget;

class QActionGroup;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

private slots:
    void selectFont();
private:
    SudokuWidget *sw;

    void makeLanguageAction(const QString& name, quint32 begin);

    QActionGroup *languageGroup;
    QMenu *language;
};

#endif // MAINWINDOW_H
