#pragma once

#include <memory>

#include <QDialog>

namespace Ui { class FunctionSettings; };


class FunctionSettings :
    public QDialog
{
public:
    FunctionSettings(QWidget * parent = nullptr);
    ~FunctionSettings() override;

private:
    std::unique_ptr<Ui::FunctionSettings> ui;
};

