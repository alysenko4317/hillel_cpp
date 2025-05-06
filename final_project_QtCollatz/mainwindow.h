#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <atomic>
#include <QFutureWatcher>
#include "collatzcalculator.h"  // Collatz calculation module

class QPushButton;
class QSlider;
class QSpinBox;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onTestClicked();

private:
    // UI elements
    QPushButton *exitButton;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *testButton;
    QSlider     *threadSlider;
    QSpinBox    *limitSpinBox;
    QTextEdit   *outputTextEdit;

    std::atomic_bool stopFlag;
    QFutureWatcher<CollatzResult> *calcWatcher;

    quint64 currentLimit;
    int currentNumThreads;

    void resetUI();
};

#endif // MAINWINDOW_H
