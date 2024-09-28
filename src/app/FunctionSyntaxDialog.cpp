#include "FunctionSyntaxDialog.h"

#include <QCoreApplication>

#include "ui_FunctionSyntaxDialog.h"

namespace
{

constexpr auto CONTEXT = "FUNCTION_SYNTAX";

constexpr auto TABLE_HEAD_TEMPLATE = QT_TRANSLATE_NOOP(
	"FUNCTION_SYNTAX",
	R"(<p><h2>%1</h2><table border="1"><tr><th>%2</th><th>DEFINITION</th><th>EXAMPLE</th></tr>%3</table></p>)");

constexpr auto OPERATOR = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "OPERATOR");
constexpr auto FUNCTION = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "FUNCTION");

using OperationDescription = std::tuple<const char*, const char*, const char*>;

constexpr auto ARITHMETIC_HEAD = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Arithmetic &amp; Assignment Operators");
constexpr OperationDescription ARITHMETIC[]
{
	{"+" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Addition between x and y."), "x + y"},
	{"-" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Subtraction between x and y."), "x - y"},
	{"*" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Multiplication between x and y."), "x * y"},
	{"/" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Division between x and y."), "x / y"},
	{"%" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Modulus of x with respect to y."), "x % y"},
	{"^" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "x to the power of y."), "x ^ y"},
	{":=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Assign the value of x to y. Where y is either a variable or vector type."), "y := x"},
	{"+=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Increment x by the value of the expression on the right hand side. Where x is either a variable or vector type."), "x += abs(y - z)"},
	{"-=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Decrement x by the value of the expression on the right hand side. Where x is either a variable or vector type."), "x[i] -= abs(y + z)"},
	{"*=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Assign the multiplication of x by the value of the  expression on the righthand side to x. Where x is either a variable or vector type."), "x *= abs(y / z)"},
	{"/=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Assign the division of x by the value of the expression on the right-hand side to x. Where x is either a variable or vector type."), "x[i + j] /= abs(y * z)"},
	{"%=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Assign x modulo the value of the expression on the right hand side to x. Where x is either a variable or vector type."), "x[2] %= y ^ 2"},
};
constexpr auto EQUALITIES_HEAD = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Equalities &amp; Inequalities");
constexpr OperationDescription EQUALITIES[]
{
	{"== or ="       , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True only if x is strictly equal to y."), "x == y"},
	{"&lt;&gt; or !=", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True only if x does not equal y."), "x &lt;&gt; y or x != y"},
	{"&lt;"          , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True only if x is less than y."), "x &lt; y"},
	{"&lt;="         , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True only if x is less than or equal to y."), "x &lt;= y"},
	{"&gt;"          , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True only if x is greater than y."), "x &gt; y"},
	{"&gt;="         , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True only if x greater than or equal to y."), "x &gt;= y"},
};
constexpr auto BOOLEAN_OPERATIONS_HEAD = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Boolean Operations");
constexpr OperationDescription BOOLEAN_OPERATIONS[]
{
	{"true" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "True state or any value other than zero (typically 1)."), ""},
	{"false", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "False state, value of exactly zero."), ""},
	{"and"  , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical AND, True only if x and y are both true."), "x and y"},
	{"mand" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Multi-input logical AND, True only if all inputs are true. Left to right short-circuiting of expressions."), "mand(x &gt; y, z &lt; w, u or v, w and x)"},
	{"mor"  , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Multi-input logical OR, True if at least one of the inputs are true. Left to right short-circuiting of expressions."), "mor(x &gt; y, z &lt; w, u or v, w and x)"},
	{"nand" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical NAND, True only if either x or y is false."), "x nand y"},
	{"nor"  , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical NOR, True only if the result of x or y is false"), "x nor y"},
	{"not"  , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical NOT, Negate the logical sense of the input."), "not(x and y) == x nand y"},
	{"or"   , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical OR, True if either x or y is true."), "x or y"},
	{"xor"  , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical XOR, True only if the logical states of x and y differ."), "x xor y"},
	{"xnor" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Logical XNOR, True iff the biconditional of x and y is satisfied."), "x xnor y"},
	{"&amp;", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Similar to AND but with left to right expression short circuiting optimisation."), "(x &amp; y) == (y and x)"},
	{"|"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Similar to OR but with left to right expression short circuiting optimisation."), "(x | y) == (y or x)"},
};
constexpr auto GENERAL_PURPOSE_FUNCTIONS_HEAD = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "General Purpose Functions");
constexpr OperationDescription GENERAL_PURPOSE_FUNCTIONS[]
{
	{"abs"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Absolute value of x."), "abs(x)"},
	{"avg"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Average of all the inputs."), "avg(x,y,z,w,u,v) == (x + y + z + w + u + v) / 6"},
	{"ceil"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Smallest integer that is greater than or equal to x."), "ceil(x)"},
	{"clamp"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Clamp x in range between r0 and r1, where r0 &lt; r1."), "clamp(r0,x,r1)"},
	{"equal"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Equality test between x and y using normalised epsilon"), "equal(x, y)"},
	{"erf"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Error function of x."), "erf(x)"},
	{"erfc"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Complimentary error function of x."), "erfc(x)"},
	{"exp"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "e to the power of x."), "exp(x)"},
	{"expm1"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "e to the power of x minus 1, where x is very small."), "expm1(x)"},
	{"floor"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Largest integer that is less than or equal to x."), "floor(x)"},
	{"frac"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Fractional portion of x."), "frac(x)"},
	{"hypot"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Hypotenuse of x and y"), "hypot(x,y) = sqrt(x*x + y*y)"},
	{"iclamp"           , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Inverse-clamp x outside of the range r0 and r1. Where r0 &lt; r1. If x is within the range it will snap to the closest bound."), "iclamp(r0,x,r1"},
	{"inrange"          , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "In-range returns 'true' when x is within the range r0 and r1. Where r0 &lt; r1."), "inrange(r0,x,r1"},
	{"log"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Natural logarithm of x."), "log(x)"},
	{"log10"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Base 10 logarithm of x."), "log10(x)"},
	{"log1p"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Natural logarithm of 1 + x, where x is very small."), "log1p(x)"},
	{"log2"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Base 2 logarithm of x."), "log2(x)"},
	{"logn"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Base N logarithm of x. where n is a positive integer."), "logn(x,8)"},
	{"max"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Largest value of all the inputs."), "max(x,y,z,w,u,v)"},
	{"min"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Smallest value of all the inputs."), "min(x,y,z,w,u)"},
	{"mul"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Product of all the inputs."), "mul(x,y,z,w,u,v,t) == (x * y * z * w * u * v * t)"},
	{"ncdf"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Normal cumulative distribution function."), "ncdf(x)"},
	{"not_equal"        , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Not-equal test between x and y using normalised epsilon"), "not_equal(x, y)"},
	{"pow"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "x to the power of y."), "pow(x,y) == x ^ y"},
	{"root"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Nth-Root of x. where n is a positive integer."), "root(x,3) == x^(1/3)"},
	{"round"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Round x to the nearest integer."), "round(x)"},
	{"roundn"           , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Round x to n decimal places"), "roundn(x,3)) where n &gt; 0 and is an integer. (eg: roundn(1.2345678,4) == 1.2346"},
	{"sgn"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Sign of x, -1 where x &lt; 0, +1 where x &gt; 0, else zero."), "sgn(x)"},
	{"sqrt"             , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Square root of x, where x &gt;= 0."), "sqrt(x)"},
	{"sum"              , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Sum of all the inputs."), "sum(x,y,z,w,u,v,t) == (x + y + z + w + u + v + t)"},
	{"swap or &lt;=&gt;", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Swap the values of the variables x and y and return the current value of y."), "swap(x,y) or x &lt;=&gt; y"},
	{"trunc"            , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Integer portion of x."), "trunc(x)"},
};
constexpr auto TRIGONOMETRY_FUNCTIONS_HEAD = QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Trigonometry Functions");
constexpr OperationDescription TRIGONOMETRY_FUNCTIONS[]
{
	{"acos"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Arc cosine of x expressed in radians. Interval [-1,+1]"), "acos(x)"},
	{"acosh"   , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Inverse hyperbolic cosine of x expressed in radians."), "acosh(x)"},
	{"asin"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Arc sine of x expressed in radians. Interval [-1,+1]"), "asin(x)"},
	{"asinh"   , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Inverse hyperbolic sine of x expressed in radians."), "asinh(x)"},
	{"atan"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Arc tangent of x expressed in radians. Interval [-1,+1]"), "atan(x)"},
	{"atan2"   , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Arc tangent of (x / y) expressed in radians. [-pi,+pi]"), "atan2(x,y)"},
	{"atanh"   , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Inverse hyperbolic tangent of x expressed in radians."), "atanh(x)"},
	{"cos"     , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Cosine of x."), "cos(x)"},
	{"cosh"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Hyperbolic cosine of x."), "cosh(x)"},
	{"cot"     , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Cotangent of x."), "cot(x)"},
	{"csc"     , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Cosecant of x."), "csc(x)"},
	{"sec"     , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Secant of x."), "sec(x)"},
	{"sin"     , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Sine of x."), "sin(x)"},
	{"sinc"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Sine cardinal of x."), "sinc(x)"},
	{"sinh"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Hyperbolic sine of x."), "sinh(x)"},
	{"tan"     , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Tangent of x."), "tan(x)"},
	{"tanh"    , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Hyperbolic tangent of x."), "tanh(x)"},
	{"deg2rad" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Convert x from degrees to radians."), "deg2rad(x)"},
	{"deg2grad", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Convert x from degrees to gradians."), "deg2grad(x)"},
	{"rad2deg" , QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Convert x from radians to degrees."), "rad2deg(x)"},
	{"grad2deg", QT_TRANSLATE_NOOP("FUNCTION_SYNTAX", "Convert x from gradians to degrees."), "grad2deg(x)"},
};

auto Tr(const char* str)
{
	return QCoreApplication::translate(CONTEXT, str);
}

auto concatenate(const std::ranges::input_range auto& container)
{
	QString result;
	for (const auto& item : container)
		result.append(QString("<tr><td>%1</td><td>%2</td><td>%3</td></tr>")
		                  .arg(std::get<0>(item))
		                  .arg(Tr(std::get<1>(item)))
		                  .arg(std::get<2>(item)));
	return result;
}

auto get_table(const char* head, const char* op_type, const std::ranges::input_range auto& container)
{
	return Tr(TABLE_HEAD_TEMPLATE).arg(Tr(head)).arg(Tr(op_type)).arg(concatenate(container));
}

} // namespace

FunctionSyntaxDialog::FunctionSyntaxDialog(QWidget* parent)
	: QDialog(parent)
	, ui_(std::make_unique<Ui::FunctionSyntaxDialog>())
{
	ui_->setupUi(this);
	const auto text = get_table(ARITHMETIC_HEAD, OPERATOR, ARITHMETIC) + get_table(EQUALITIES_HEAD, OPERATOR, EQUALITIES)
	                + get_table(BOOLEAN_OPERATIONS_HEAD, OPERATOR, BOOLEAN_OPERATIONS)
	                + get_table(GENERAL_PURPOSE_FUNCTIONS_HEAD, FUNCTION, GENERAL_PURPOSE_FUNCTIONS)
	                + get_table(TRIGONOMETRY_FUNCTIONS_HEAD, FUNCTION, TRIGONOMETRY_FUNCTIONS)
					;

	ui_->text->setText(text);
}

FunctionSyntaxDialog::~FunctionSyntaxDialog() = default;
