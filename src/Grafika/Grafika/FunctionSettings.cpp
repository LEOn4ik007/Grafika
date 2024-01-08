#include "FunctionSettings.h"

#include "ui_FunctionSettings.h"

FunctionSettings::FunctionSettings(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique <Ui::FunctionSettings>())
{
    ui->setupUi(this);
}

FunctionSettings::~FunctionSettings() = default;
