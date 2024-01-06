#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique <Ui::MainWindowClass>())
{
    ui->setupUi(this);
    connect(ui->actionExit, &QAction::triggered, [] { QApplication::exit(0); });
}

MainWindow::~MainWindow() = default;
