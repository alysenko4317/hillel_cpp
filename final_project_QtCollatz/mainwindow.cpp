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
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stopFlag(false)
{
    // Create a central widget and the main layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Row with "Start", "Stop", "Test", "Exit" ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    startButton = new QPushButton("Start", this);
    stopButton  = new QPushButton("Stop",  this);
    testButton  = new QPushButton("Test",  this);
    exitButton  = new QPushButton("Exit",  this);

    // By default, "Stop" is disabled
    stopButton->setEnabled(false);

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(testButton);
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
    // Set minimum to 1,000,000 so that the user can only enter numbers >= 1,000,000.
    limitSpinBox->setMinimum(1000000);
    // Set a high maximum to allow manual entry of larger numbers.
    limitSpinBox->setMaximum(1000000000);
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
        try {
            CollatzResult result = calcWatcher->result();
            outputTextEdit->append("----- Результати обчислень -----");
            outputTextEdit->append(QString("Верхня межа: %1").arg(currentLimit));
            outputTextEdit->append(QString("Використано потоків: %1").arg(currentNumThreads));
            if (stopFlag.load()) {
                outputTextEdit->append("Обчислення перервано користувачем.");
            } else {
                outputTextEdit->append(QString("Найдовший ланцюг у діапазоні: %1")
                                           .arg(result.bestNumber));
                outputTextEdit->append(QString("Довжина ланцюга: %1").arg(result.bestLength));
                outputTextEdit->append(QString("Час обчислень: %1 мс").arg(result.timeMs));
            }
            outputTextEdit->append("----- Кінець обчислень -----");
        }
        catch (const QUnhandledException &ex) {
            try {
                // Watchout for: QUnhandledException Example how to handle it (to unwrap your exception):
                // https://stackoverflow.com/questions/76063090/how-to-propagate-exceptions-to-the-main-thread-from-a-qtconcurrentrun-with-pro
                std::rethrow_exception(ex.exception());
            } catch (const std::exception &e) {
                outputTextEdit->append(QString("Error: %1").arg(e.what()));
            } catch (...) {
                outputTextEdit->append(QString("Unknown exception caught in QUnhandledException"));
            }
        }
        catch (const std::exception &e) {
            outputTextEdit->append(QString("Error: %1").arg(e.what()));
        }
        resetUI();
    });

    // Connect signals and slots
    connect(exitButton,  &QPushButton::clicked, this, &MainWindow::close);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(stopButton,  &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(testButton,  &QPushButton::clicked, this, &MainWindow::onTestClicked);
}

MainWindow::~MainWindow()
{
    // If a calculation is running, signal it to stop
    stopFlag.store(true);
    if(calcWatcher->isRunning()) {
        calcWatcher->cancel();
        calcWatcher->waitForFinished();
    }
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

    // Get parameters from the UI and store them for later output
    currentLimit = limitSpinBox->value();
    currentNumThreads = threadSlider->value();

    // Launch the Collatz calculation asynchronously using the CollatzCalculator module
    QFuture<CollatzResult> future = QtConcurrent::run(&CollatzCalculator::calculate, currentLimit, currentNumThreads, std::ref(stopFlag));
    calcWatcher->setFuture(future);
}

void MainWindow::onStopClicked()
{
    // Signal cancellation of the calculation
    stopFlag.store(true);
    outputTextEdit->append("Зупинка обчислень...");
}

void MainWindow::onTestClicked()
{
    outputTextEdit->clear();
    outputTextEdit->append("----- Test Sequence for 13 -----");
    try {
        // Call the test function from CollatzCalculator
        CollatzTestResult testRes = CollatzCalculator::getTestSequence(13);
        // Output the full sequence and its length.
        outputTextEdit->append(QString("Sequence: %1").arg(testRes.sequence));
        outputTextEdit->append(QString("Sequence length: %1").arg(testRes.length));
    } catch (const std::exception &e) {
        outputTextEdit->append(QString("Test Error: %1").arg(e.what()));
    }
    outputTextEdit->append("----- End of Test -----");
}

void MainWindow::resetUI()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}
