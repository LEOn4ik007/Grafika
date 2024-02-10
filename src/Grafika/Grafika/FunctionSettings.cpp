#include "FunctionSettings.h"

#include "ui_FunctionSettings.h"

#include <QColorDialog>
#include <QTimer>

#include <exprtk.hpp>

namespace
{
    void SetupAutoMode(QCheckBox * checkBox, QWidget * widget)
    {
        QObject::connect(checkBox, &QCheckBox::stateChanged, widget, [=](int state) {widget->setEnabled(state == Qt::Unchecked); });
    }
}

int FunctionSettings::nextNumber = 0;

FunctionSettings::FunctionSettings(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique <Ui::FunctionSettings>())
    , changedTimer(new QTimer(this))
{
    ui->setupUi(this);
    ui->lineEditTitle->setText(tr("Function %1").arg(++nextNumber));

    changedTimer->setSingleShot(true);
    changedTimer->setInterval(std::chrono::milliseconds(50));
    connect(changedTimer, &QTimer::timeout, this, &FunctionSettings::settingsChanged);

    connect(ui->lineEditTitle, &QLineEdit::textChanged, this, &FunctionSettings::titleChanged);
    connect(ui->buttonRemove, &QAbstractButton::clicked, this, &QObject::deleteLater);
    connect(ui->doubleSpinBoxXMax, &QDoubleSpinBox::valueChanged, ui->doubleSpinBoxXMin, &QDoubleSpinBox::setMaximum);
    connect(ui->doubleSpinBoxXMin, &QDoubleSpinBox::valueChanged, ui->doubleSpinBoxXMax, &QDoubleSpinBox::setMinimum);
    connect(ui->pushButtonColor, &QAbstractButton::clicked, this, &FunctionSettings::ShowColorDialog);
    connect(ui->comboBoxStyle, &QComboBox::currentIndexChanged, this, &FunctionSettings::viewChanged);
    connect(ui->spinBoxWidth, &QSpinBox::valueChanged, this, &FunctionSettings::viewChanged);
    connect(ui->doubleSpinBoxXMin, &QDoubleSpinBox::valueChanged, changedTimer, qOverload<>(&QTimer::start));
    connect(ui->doubleSpinBoxXMax, &QDoubleSpinBox::valueChanged, changedTimer, qOverload<>(&QTimer::start));
    connect(ui->doubleSpinBoxDeltaX, &QDoubleSpinBox::valueChanged, changedTimer, qOverload<>(&QTimer::start));
    connect(ui->lineEditFunctionString, &QLineEdit::textChanged, this, &FunctionSettings::Parse);
    
    SetupAutoMode(ui->checkBoxXMaxAuto, ui-> doubleSpinBoxXMax);
    SetupAutoMode(ui->checkBoxXMinAuto, ui->doubleSpinBoxXMin);
    SetupAutoMode(ui->checkBoxDeltaXAuto, ui->doubleSpinBoxDeltaX);
    FillStyleCombobox();
    SetColor(Qt::black);
}

FunctionSettings::~FunctionSettings() = default;

QString FunctionSettings::GetTitle() const
{
    return ui->lineEditTitle->text();
}

QPolygonF FunctionSettings::GetPoints()
{
    if (!expression)
        return {};

    QPolygonF points;
    for (x = ui->doubleSpinBoxXMin->value(); x <= ui->doubleSpinBoxXMax->value(); x += ui->doubleSpinBoxDeltaX->value())
        if (auto y = expression->value(); !isnan(y))
            points << QPointF(x, y);

    return points;
}

const QColor & FunctionSettings::GetColor() const
{
    return curveColor;
}

double FunctionSettings::GetWidth() const
{
    return ui->spinBoxWidth->value();
}

Qt::PenStyle FunctionSettings::GetPenStyle() const
{
    return static_cast<Qt::PenStyle>(ui->comboBoxStyle->currentData().toInt());
}

void FunctionSettings::SetXMin(double value)
{
    xMin = value;
    if (ui->checkBoxXMinAuto->isChecked())
        ui->doubleSpinBoxXMin->setValue(value);
    SetCanvasWidth(canvasWidth);
}

void FunctionSettings::SetXMax(double value)
{
    xMax = value;
    if (ui->checkBoxXMaxAuto->isChecked())
        ui->doubleSpinBoxXMax->setValue(value);
    SetCanvasWidth(canvasWidth);
}

void FunctionSettings::SetCanvasWidth(int value)
{
    assert(value > 0);
    canvasWidth = value;
    if (ui->checkBoxDeltaXAuto->isChecked())
        ui->doubleSpinBoxDeltaX->setValue((xMax - xMin) / canvasWidth);
}

void FunctionSettings::FillStyleCombobox()
{
    ui->comboBoxStyle->addItem(tr("Solid line"), static_cast<int>(Qt::SolidLine));
    ui->comboBoxStyle->addItem(tr("Dash line"), static_cast<int>(Qt::DashLine));
    ui->comboBoxStyle->addItem(tr("Dot line"), static_cast<int>(Qt::DotLine));
    ui->comboBoxStyle->addItem(tr("Dash dot line"), static_cast<int>(Qt::DashDotLine));
    ui->comboBoxStyle->addItem(tr("Dash dot dot line"), static_cast<int>(Qt::DashDotDotLine));
    ui->comboBoxStyle->setCurrentIndex(0);
}

void FunctionSettings::SetColor(const QColor & color)
{
    ui->pushButtonColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    curveColor = color;
    emit viewChanged();
}

void FunctionSettings::ShowColorDialog()
{
    QColorDialog dialog(this);
    dialog.setCurrentColor(curveColor);
    if (dialog.exec() == QDialog::Accepted)
        SetColor(dialog.currentColor());
}

void FunctionSettings::Parse()
{
    const auto expression_string = ui->lineEditFunctionString->text().toStdString();

    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", x);
    symbol_table.add_constants();

    expression = std::make_unique<exprtk::expression<double>>();
    expression->register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    if (parser.compile(expression_string, *expression))
        changedTimer->start();
    else
        expression.reset();

    //for (x = T(-5); x <= T(+5); x += T(0.001))
    //{
    //    const T y = expression.value();
    //    printf("%19.15f\t%19.15f\n", x, y);
    //}
}
