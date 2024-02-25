#include "ui_MainWindow.h"
#include "MainWindow.h"

#include <QMessageBox>
#include <QPen>
#include <QTimer>
#include <QLabel>
#include<QMouseEvent>
#include <QPainter>

#include <QwtPlot>
#include <QwtLegend>
#include <QwtPlotGrid>
#include <QwtPlotMagnifier>
#include <QwtPlotPanner>
#include <QwtPlotCurve>
#include <QwtPlotPicker>
#include <QwtPickerDragPointMachine>

#include "FunctionSettings.h"

class Plot : public QwtPlot
{
public:
    explicit Plot(QWidget * parent)
        : QwtPlot(parent)
    {
    }

private:
    void drawCanvas(QPainter * painter) override
    {
        const auto zeroPos = QPointF{ transform(QwtPlot::xBottom, 0.0), transform(QwtPlot::yLeft, 0.0) };
        painter->save();
        painter->setPen(QPen(Qt::black, 3));
        painter->drawLine(QPointF{ 0.0, zeroPos.y() }, QPointF{ static_cast<qreal>(canvas()->width()), zeroPos.y()});
        painter->drawLine(QPointF{ zeroPos.x(), 0.0 }, QPointF{ zeroPos.x(), static_cast<qreal>(canvas()->height())});
        painter->restore();

        QwtPlot::drawCanvas(painter);
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique <Ui::MainWindowClass>())
    , intervalTimer(new QTimer(this))
    , cursorCoordinates(new QLabel(this))
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

    ui->statusBar->addPermanentWidget(cursorCoordinates);
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

    if (event->type() == QEvent::MouseMove)
    {
        const auto pos = static_cast<QMouseEvent *>(event)->pos();
        cursorCoordinates->setText(QString("%1, %2").arg(plot->invTransform(QwtPlot::xBottom, pos.x())).arg(plot->invTransform(QwtPlot::yLeft, pos.y())));
    }

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
    plot = new Plot(this);
    setCentralWidget(plot);

    plot->setCanvasBackground(Qt::white);
    plot->setAxisTitle(QwtPlot::yLeft, "Y");
    plot->setAxisTitle(QwtPlot::xBottom, "X");
    plot->insertLegend(new QwtLegend());

    auto * grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::black, 1));
    grid->setMinorPen(QPen(Qt::lightGray, 1, Qt::DotLine));
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->attach(plot);

    auto * magnifier = new QwtPlotMagnifier(plot->canvas());
    magnifier->setMouseButton(Qt::MiddleButton);

    auto * panner = new QwtPlotPanner(plot->canvas());
    panner->setMouseButton(Qt::RightButton);

    auto * picker =
        new QwtPlotPicker(
            QwtPlot::xBottom, QwtPlot::yLeft,   
            QwtPlotPicker::CrossRubberBand,
            QwtPicker::ActiveOnly,
            plot->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());
 
    plot->canvas()->installEventFilter(this);
    plot->canvas()->setMouseTracking(true);

    plot->setAxisScale(QwtPlot::xBottom, -10, 10);
    plot->setAxisScale(QwtPlot::yLeft, -10, 10);
}

QwtPlotCurve * MainWindow::CreateCurve(FunctionSettings * functionSettings)
{
    auto * curve = new QwtPlotCurve();
    auto setPoints = [=] {
        auto points = functionSettings->GetPoints();
        curve->setSamples(points); 
    };

    curve->setTitle(functionSettings->GetTitle());
    connect(functionSettings, &FunctionSettings::titleChanged, [=] {curve->setTitle(functionSettings->GetTitle()); });
    connect(functionSettings, &FunctionSettings::settingsChanged, setPoints);
    OnCurveViewChanged(functionSettings, curve);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

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
