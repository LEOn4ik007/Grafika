#include "ui_MainWindow.h"
#include "MainWindow.h"

#include <QMessageBox>
#include <QPen>

#include <QwtPlot>
#include <QwtLegend>
#include <QwtPlotGrid>
#include <QwtPlotMagnifier>
#include <QwtPlotPanner>
#include <QwtPlotCurve>

#include "FunctionSettings.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique <Ui::MainWindowClass>())
{
    ui->setupUi(this);
    connect(ui->actionExit, &QAction::triggered, [] { QApplication::exit(0); });
    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::CreateFunctionSettingsDialog);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::ShowAboutDialog);
    SetupPlot();
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
    
    QwtPlotCurve * curve = new QwtPlotCurve();
    curve->setTitle(functionSettings->GetTitle());
    curve->setPen(Qt::blue, 6);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    auto points = functionSettings->GetPoints();
    curve->setSamples(points);
    curve->attach(plot);
}

void MainWindow::ShowAboutDialog()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Icon::Information);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setWindowTitle(tr("About Grafika"));
    msgBox.setText(tr("%1<br/><a href=\"%2\">%2</a>").arg("Functions graphics plotter").arg("https://github.com/LEOn4ik007/Grafika"));
    msgBox.exec();
}

void MainWindow::SetupPlot()
{
    plot = new QwtPlot(this);
    setCentralWidget(plot);

    plot->setCanvasBackground(Qt::white);
    plot->setAxisTitle(QwtPlot::yLeft, "Y");
    plot->setAxisTitle(QwtPlot::xBottom, "X");
    plot->insertLegend(new QwtLegend());

    QwtPlotGrid * grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::gray, 1));
    grid->attach(plot);

    QwtPlotMagnifier * magnifier = new QwtPlotMagnifier(plot->canvas());
    magnifier->setMouseButton(Qt::MiddleButton);

    QwtPlotPanner * d_panner = new QwtPlotPanner(plot->canvas());

    d_panner->setMouseButton(Qt::RightButton);
}
