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
    auto * curve = CreateCurve(functionSettings);
    functionSettings->show();
    auto * action = ui->menuFunctions->addAction(functionSettings->GetTitle(), [=] { functionSettings->show(); });
    connect(functionSettings, &FunctionSettings::titleChanged, action, &QAction::setText);
    connect(functionSettings, &FunctionSettings::viewChanged, [=] {OnCurveViewChanged(functionSettings, curve); });
    auto connection = connect(functionSettings, &QObject::destroyed, this, [&, action, curve] { ui->menuFunctions->removeAction(action); delete curve; });
    connections.push_back(connection);
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

    QwtPlotPanner * panner = new QwtPlotPanner(plot->canvas());
    panner->setMouseButton(Qt::RightButton);
}

QwtPlotCurve * MainWindow::CreateCurve(const FunctionSettings * functionSettings)
{
    auto * curve = new QwtPlotCurve();
    curve->setTitle(functionSettings->GetTitle());
    OnCurveViewChanged(functionSettings, curve);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    auto points = functionSettings->GetPoints();
    curve->setSamples(points);
    curve->attach(plot);

    return curve;
}

void MainWindow::OnCurveViewChanged(const FunctionSettings * functionSettings, QwtPlotCurve * curve) const
{
    curve->setPen(functionSettings->GetColor(),functionSettings->GetWidth(), functionSettings->GetPenStyle());
    plot->replot();
}
