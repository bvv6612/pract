#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QDebug>
#include "calculator.h"
#include "trafficanalysis.h"
Calculator *myCalculator;
TrafficAnalysis *myTrafficAnalysis;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myCalculator = new Calculator();
    myTrafficAnalysis = new TrafficAnalysis();
    connect(ui->pushButton_calculator,SIGNAL(clicked()),this,SLOT(CalculatorWindow()));
    connect(ui->action_calculator,SIGNAL(triggered()),this,SLOT(CalculatorWindow()));
    connect(ui->pushButton_trafficAnalysis,SIGNAL(clicked()),this,SLOT(TrafficAnalysisWindow()));
    connect(ui->action_trafficAnalysis,SIGNAL(triggered()),this,SLOT(TrafficAnalysisWindow()));
}

void MainWindow::CalculatorWindow()
{
    myCalculator->show();
}

void MainWindow::TrafficAnalysisWindow()
{
    myTrafficAnalysis->show();
}

MainWindow::~MainWindow()
{
    delete myCalculator;
    delete myTrafficAnalysis;
    delete ui;
}

