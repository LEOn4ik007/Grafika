#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FunctionSettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique <Ui::MainWindowClass>())
{
    ui->setupUi(this);
    connect(ui->actionExit, &QAction::triggered, [] { QApplication::exit(0); });
    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::CreateFunctionSettingsDialog);
}

MainWindow::~MainWindow()
{
    for (auto connection : connections)
        disconnect(connection);
}

void MainWindow::CreateFunctionSettingsDialog()
{
    auto * functionSettings = new FunctionSettings(this);
    functionSettings->show();
    auto * action = ui->menuFunctions->addAction(functionSettings->GetTitle(), [=] { functionSettings->show(); });
    connect(functionSettings, &FunctionSettings::titleChanged, action, &QAction::setText);
    auto connection = connect(functionSettings, &QObject::destroyed, this, [&, action] { ui->menuFunctions->removeAction(action); });
    connections.push_back(connection);
}
