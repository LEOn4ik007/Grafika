#pragma once
#include <string>
#include <memory>

#ifdef exprtk_EXPORTS
#define EXPORTEXPRTK_WRAPPER __declspec( dllexport )
#else
#define EXPORTEXPRTK_WRAPPER __declspec( dllimport )
#endif

namespace exprtk { template<typename T> class expression; }

class EXPORTEXPRTK_WRAPPER ExprtkWrapper
{
public:
	ExprtkWrapper(std::string function, std::string name, double &x);
	~ExprtkWrapper();

	double GetValue() const;
	bool IsValid() const;

private:
	std::unique_ptr<exprtk::expression<double>> m_expression;
};
