#pragma once

#include <QDialog>

namespace Ui
{
class FunctionSyntaxDialog;
}

class FunctionSyntaxDialog final
	: public QDialog
{
public:
	FunctionSyntaxDialog(QWidget* parent = nullptr);
	~FunctionSyntaxDialog() override;

private:
	std::unique_ptr<Ui::FunctionSyntaxDialog> ui_;
};
