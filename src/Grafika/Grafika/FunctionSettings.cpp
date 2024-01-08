#include "FunctionSettings.h"

#include "ui_FunctionSettings.h"

int FunctionSettings::nextNumber = 0;

FunctionSettings::FunctionSettings(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique <Ui::FunctionSettings>())
{
    ui->setupUi(this);
    ui->lineEditTitle->setText(tr("Function %1").arg(++nextNumber));
    connect(ui->lineEditTitle, &QLineEdit::textChanged, this, &FunctionSettings::titleChanged);
}

FunctionSettings::~FunctionSettings() = default;

QString FunctionSettings::GetTitle() const
{
    return ui->lineEditTitle->text();
}
