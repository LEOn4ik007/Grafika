#pragma once

#include <memory>
#include <vector>

#include <QtWidgets/QMainWindow>

namespace Ui { class MainWindowClass; };
class QwtPlot;
class QwtPlotCurve;
class FunctionSettings;

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void CreateFunctionSettingsDialog();
    void ShowAboutDialog();
    void SetupPlot();
    QwtPlotCurve * CreateCurve(const FunctionSettings * functionSettings);

private:
    std::unique_ptr<Ui::MainWindowClass> ui;
    std::vector<QMetaObject::Connection> connections;
    QwtPlot * plot{ nullptr };
};
