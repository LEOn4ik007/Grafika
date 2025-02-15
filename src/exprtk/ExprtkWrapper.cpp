#include "ExprtkWrapper.h"
#include <exprtk.hpp>


ExprtkWrapper::ExprtkWrapper(std::string function, std::string name, double&x)
{
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable(name, x);
    symbol_table.add_constants();

    m_expression = std::make_unique<exprtk::expression<double>>();
    m_expression->register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    if (!parser.compile(function, *m_expression))
        m_expression.reset();
}

ExprtkWrapper::~ExprtkWrapper()
{
}

double ExprtkWrapper::GetValue() const
{
    return m_expression->value();
}

bool ExprtkWrapper::IsValid() const
{
    return !!m_expression;
}