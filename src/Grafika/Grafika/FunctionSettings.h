#pragma once

#include <memory>

#include <QDialog>

namespace Ui { class FunctionSettings; };

class FunctionSettings :
    public QDialog
{
    Q_OBJECT

signals:
    void titleChanged(const QString & title) const;
    void viewChanged() const;

public:
    FunctionSettings(QWidget * parent = nullptr);
    ~FunctionSettings() override;

public:
    QString GetTitle() const;
    QPolygonF GetPoints() const;
    const QColor & GetColor() const;
    double GetWidth() const;
    Qt::PenStyle GetPenStyle() const;

private:
    double F(double x) const;
    void FillStyleCombobox();
    void SetColor(const QColor & color);
    void ShowColorDialog();

private:
    static int nextNumber;
    std::unique_ptr<Ui::FunctionSettings> ui;
    QColor curveColor;
};
