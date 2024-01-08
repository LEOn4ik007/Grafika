#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>

namespace Ui { class MainWindowClass; };

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void CreateFunctionSettingsDialog();

private:
    std::unique_ptr<Ui::MainWindowClass> ui;
};
