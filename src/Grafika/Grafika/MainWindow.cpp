#include "MainWindow.h"
#include <QMessageBox>
#include "ui_MainWindow.h"
#include "FunctionSettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique <Ui::MainWindowClass>())
{
    ui->setupUi(this);
    connect(ui->actionExit, &QAction::triggered, [] { QApplication::exit(0); });
    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::CreateFunctionSettingsDialog);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::ShowAboutDialog);
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

void MainWindow::ShowAboutDialog()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Icon::Information);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setWindowTitle(tr("About Grafika"));
    msgBox.setText(tr("%1<br/><a href=\"%2\">%2</a>").arg("Functions grafics plotter").arg("https://github.com/LEOn4ik007/Grafika"));
    msgBox.exec();
}
