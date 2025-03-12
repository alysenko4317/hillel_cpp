#include "mainwindow.h"

#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create a central widget and the main layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Row with "Start", "Stop", "Exit" ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    startButton = new QPushButton("Start", this);
    stopButton  = new QPushButton("Stop",  this);
    exitButton  = new QPushButton("Exit",  this);

    // By default, "Stop" is disabled
    stopButton->setEnabled(false);

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(exitButton);

    // --- Slider for threads count ---
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    QLabel *sliderLabel = new QLabel("Кількість потоків:", this);
    threadSlider = new QSlider(Qt::Horizontal, this);
    threadSlider->setMinimum(1);
    int maxThreads = QThread::idealThreadCount();
    threadSlider->setMaximum(maxThreads);
    threadSlider->setValue(maxThreads); // default to maximum
    sliderLayout->addWidget(sliderLabel);
    sliderLayout->addWidget(threadSlider);

    // --- QSpinBox for upper limit ---
    QHBoxLayout *spinBoxLayout = new QHBoxLayout();
    QLabel *spinBoxLabel = new QLabel("Граничне число:", this);
    limitSpinBox = new QSpinBox(this);
    limitSpinBox->setMinimum(1);
    limitSpinBox->setMaximum(1000000);
    limitSpinBox->setValue(1000000);
    spinBoxLayout->addWidget(spinBoxLabel);
    spinBoxLayout->addWidget(limitSpinBox);

    // --- Read-only text output ---
    outputTextEdit = new QTextEdit(this);
    outputTextEdit->setReadOnly(true);

    // Add everything to the main layout
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(sliderLayout);
    mainLayout->addLayout(spinBoxLayout);
    mainLayout->addWidget(outputTextEdit);

    // Connect signals and slots
    connect(exitButton,  &QPushButton::clicked, this, &MainWindow::close);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopButton,  &QPushButton::clicked, this, &MainWindow::onStopClicked);
}

MainWindow::~MainWindow()
{
    // TODO: If you had any manual allocations, clean them here
}

void MainWindow::onStartClicked()
{
    // Disable Start, enable Stop
    startButton->setEnabled(false);
    stopButton->setEnabled(true);

    outputTextEdit->append("Розрахунки запущено...");

    // TODO: Here you would start your Collatz calculation in multiple threads
}

void MainWindow::onStopClicked()
{
    // Re-enable Start, disable Stop
    startButton->setEnabled(true);
    stopButton->setEnabled(false);

    outputTextEdit->append("Розрахунки зупинено.");

    // TODO: If your threads are running, signal them to stop
}

void MainWindow::resetUI()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}
