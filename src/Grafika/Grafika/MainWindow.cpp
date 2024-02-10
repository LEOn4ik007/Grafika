#include "ui_MainWindow.h"
#include "MainWindow.h"

#include <QMessageBox>
#include <QPen>
#include <QTimer>

#include <QwtPlot>
#include <QwtLegend>
#include <QwtPlotGrid>
#include <QwtPlotMagnifier>
#include <QwtPlotPanner>
#include <QwtPlotCurve>
#include <QwtScaleDiv>

#include "FunctionSettings.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique <Ui::MainWindowClass>())
    , intervalTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->actionExit, &QAction::triggered, [] { QApplication::exit(0); });
    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::CreateFunctionSettingsDialog);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::ShowAboutDialog);
    SetupPlot();

    intervalTimer->setSingleShot(false);
    intervalTimer->setInterval(std::chrono::milliseconds(200));
    connect(intervalTimer, &QTimer::timeout, this, &MainWindow::OnCheckXInterval);
    intervalTimer->start();
}

MainWindow::~MainWindow()
{
    for (auto connection : connections)
        disconnect(connection);
}

bool MainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Resize)
        emit canvasWidthChanged(plot->canvas()->width());

    return QObject::eventFilter(obj, event);
}

void MainWindow::CreateFunctionSettingsDialog()
{
    auto * functionSettings = new FunctionSettings(this);
    auto * curve = CreateCurve(functionSettings);
    functionSettings->show();
    auto * action = ui->menuFunctions->addAction(functionSettings->GetTitle(), [=] { functionSettings->show(); });
    connect(functionSettings, &FunctionSettings::titleChanged, action, &QAction::setText);
    connect(functionSettings, &FunctionSettings::viewChanged, [=] {OnCurveViewChanged(functionSettings, curve); });
    connect(this, &MainWindow::xMinChanged, functionSettings, &FunctionSettings::SetXMin);
    connect(this, &MainWindow::xMaxChanged, functionSettings, &FunctionSettings::SetXMax);
    connect(this, &MainWindow::canvasWidthChanged, functionSettings, &FunctionSettings::SetCanvasWidth);
    auto connection = connect(functionSettings, &QObject::destroyed, this, [&, action, curve] { ui->menuFunctions->removeAction(action); delete curve; });
    connections.push_back(connection);
    functionSettings->SetCanvasWidth(plot->canvas()->width());
    functionSettings->SetXMin(xMin);
    functionSettings->SetXMax(xMax);
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

    plot->canvas()->installEventFilter(this);
}

QwtPlotCurve * MainWindow::CreateCurve(const FunctionSettings * functionSettings)
{
    auto * curve = new QwtPlotCurve();
    curve->setTitle(functionSettings->GetTitle());
    connect(functionSettings, &FunctionSettings::titleChanged, [=] {curve->setTitle(functionSettings->GetTitle()); });
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

void MainWindow::OnCheckXInterval()
{
    const auto interval = plot->axisInterval(QwtPlot::xBottom);
    if (xMin != interval.minValue())
    {
        xMin = interval.minValue();
        emit xMinChanged(xMin);
    }
    if (xMax != interval.maxValue())
    {
        xMax = interval.maxValue();
        emit xMaxChanged(xMax);
    }
}