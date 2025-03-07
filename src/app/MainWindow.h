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
class QwtPlotMagnifier;

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
    void CreateCurves(FunctionSettings * functionSettings);
    void OnCurvesViewChanged(const FunctionSettings * functionSettings);
    void OnCheckXInterval();
    void OnFunctionSyntaxClicked();
    void OnEnglishClicked();
    void OnRussianClicked();
    void SetLang(const QString & lang);
    void SetupLang();
    std::vector<QwtPlotCurve*>& GetCurves(const FunctionSettings*);
    void DropCurves(std::vector<QwtPlotCurve*>&);

private:
    QSettings & settings;
    std::unique_ptr<Ui::MainWindowClass> ui;
    std::vector<QMetaObject::Connection> connections;
    Plot * plot{ nullptr };
    QTimer * intervalTimer;
    double xMin, xMax;
    QLabel * cursorCoordinates;
    FunctionSyntaxDialog * functionSyntaxDialog{nullptr};
    QwtPlotMagnifier * qwtPlotMagnifier;
    std::unordered_map<const FunctionSettings*, std::vector<QwtPlotCurve *>> settingsCurves;
};
