#pragma once

#include <memory>

#include <QDialog>

namespace Ui { class FunctionSettings; }

class QAction;
class QTimer;
class ExprtkWrapper;

class FunctionSettings :
    public QDialog
{
    Q_OBJECT

signals:
    void titleChanged(const QString & title) const;
    void viewChanged() const;
    void settingsChanged() const;

public:
    FunctionSettings(QWidget * parent = nullptr);
    ~FunctionSettings() override;

public:
    QString GetTitle() const;
    QPolygonF GetPoints();
    const QColor & GetColor() const;
    double GetWidth() const;
    Qt::PenStyle GetPenStyle() const;

public slots:
    void SetXMin(double value);
    void SetXMax(double value);
    void SetCanvasWidth(int value);

private:
    void FillStyleCombobox();
    void SetColor(const QColor & color);
    void ShowColorDialog();
    void Parse();

private:
    static int nextNumber;
    std::unique_ptr<Ui::FunctionSettings> ui;
    QColor curveColor;
    double xMin{ 0.0 }, xMax{ 1000.0 };
    int canvasWidth{ 100 };
    double x;
    QTimer * changedTimer;
    std::unique_ptr<ExprtkWrapper> expression;
    QAction * invalidExpression{ nullptr };
};
