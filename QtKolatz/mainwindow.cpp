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
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stopFlag(false)
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

    // Initialize the calculation watcher
    calcWatcher = new QFutureWatcher<CollatzResult>(this);
    connect(calcWatcher, &QFutureWatcher<CollatzResult>::finished, this, [this]() {
        CollatzResult result = calcWatcher->result();
        if (stopFlag.load()) {
            outputTextEdit->append("Обчислення перервано користувачем.");
        } else {
            outputTextEdit->append(QString("Найдовший ланцюг у діапазоні: %1\nДовжина ланцюга: %2\nЧас обчислень: %3 мс")
                                       .arg(result.bestNumber)
                                       .arg(result.bestLength)
                                       .arg(result.timeMs));
        }
        resetUI();
    });

    // Connect signals and slots
    connect(exitButton,  &QPushButton::clicked, this, &MainWindow::close);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopButton,  &QPushButton::clicked, this, &MainWindow::onStopClicked);
}

MainWindow::~MainWindow()
{
    // If a calculation is running, signal it to stop
    stopFlag.store(true);
    calcWatcher->cancel();
    calcWatcher->waitForFinished();
}

void MainWindow::onStartClicked()
{
    // Disable Start, enable Stop
    startButton->setEnabled(false);
    stopButton->setEnabled(true);

    outputTextEdit->clear();
    outputTextEdit->append("Розрахунки запущено...");

    // Reset the stop flag
    stopFlag.store(false);

    // Get parameters from the UI
    quint64 limit = limitSpinBox->value();
    int numThreads = threadSlider->value();

    // Launch the Collatz calculation asynchronously using the CollatzCalculator module
    QFuture<CollatzResult> future = QtConcurrent::run(&CollatzCalculator::calculate, limit, numThreads, std::ref(stopFlag));
    calcWatcher->setFuture(future);
}

void MainWindow::onStopClicked()
{
    // Signal cancellation of the calculation
    stopFlag.store(true);
    outputTextEdit->append("Зупинка обчислень...");
}

void MainWindow::resetUI()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}
