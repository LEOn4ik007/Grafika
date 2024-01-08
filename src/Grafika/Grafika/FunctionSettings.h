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

public:
    FunctionSettings(QWidget * parent = nullptr);
    ~FunctionSettings() override;

public:
    QString GetTitle() const;

private:
    std::unique_ptr<Ui::FunctionSettings> ui;
    static int nextNumber;
};

