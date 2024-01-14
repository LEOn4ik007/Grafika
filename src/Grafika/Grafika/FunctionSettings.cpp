#include "FunctionSettings.h"

#include "ui_FunctionSettings.h"

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
{
    ui->setupUi(this);
    ui->lineEditTitle->setText(tr("Function %1").arg(++nextNumber));
    connect(ui->lineEditTitle, &QLineEdit::textChanged, this, &FunctionSettings::titleChanged);
    connect(ui->buttonRemove, &QAbstractButton::clicked, this, &QObject::deleteLater);
    connect(ui->doubleSpinBoxXMax, &QDoubleSpinBox::valueChanged, ui->doubleSpinBoxXMin, &QDoubleSpinBox::setMaximum);
    connect(ui->doubleSpinBoxXMin, &QDoubleSpinBox::valueChanged, ui->doubleSpinBoxXMax, &QDoubleSpinBox::setMinimum);
    SetupAutoMode(ui->checkBoxXMaxAuto, ui-> doubleSpinBoxXMax);
    SetupAutoMode(ui->checkBoxXMinAuto, ui->doubleSpinBoxXMin);
    SetupAutoMode(ui->checkBoxDeltaXAuto, ui->doubleSpinBoxDeltaX);
}

FunctionSettings::~FunctionSettings() = default;

QString FunctionSettings::GetTitle() const
{
    return ui->lineEditTitle->text();
}
