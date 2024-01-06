#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>


namespace Ui { class MainWindowClass; };


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindowClass> ui;

};
