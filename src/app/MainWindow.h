#pragma once

#include <memory>
#include <vector>

#include <QtWidgets/QMainWindow>

namespace Ui { class MainWindowClass; };
class QTimer;
class QLabel;
class QSettings;
class Plot;
class QwtPlotCurve;
class FunctionSettings;
class FunctionSyntaxDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void xMinChanged(double value) const;
    void xMaxChanged(double value) const;
    void canvasWidthChanged(int value) const;

public:
    MainWindow(QSettings & settings, QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    bool eventFilter(QObject * obj, QEvent * event) override;

private:
    void CreateFunctionSettingsDialog();
    void ShowAboutDialog();
    void SetupPlot();
    QwtPlotCurve * CreateCurve(FunctionSettings * functionSettings);
    void OnCurveViewChanged(const FunctionSettings * functionSettings, QwtPlotCurve * curve) const;
    void OnCheckXInterval();
    void OnFunctionSyntaxClicked();
    void OnEnglishClicked();
    void OnRussianClicked();
    void SetLang(const QString & lang);
    void SetupLang();

private:
    QSettings & settings;
    std::unique_ptr<Ui::MainWindowClass> ui;
    std::vector<QMetaObject::Connection> connections;
    Plot * plot{ nullptr };
    QTimer * intervalTimer;
    double xMin, xMax;
    QLabel * cursorCoordinates;
    FunctionSyntaxDialog * functionSyntaxDialog{nullptr};
};
