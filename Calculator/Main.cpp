#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include "list.cpp"


class UnknownCharacterException : public std::exception {
public:
	UnknownCharacterException(const std::string& message) : std::exception(message.c_str()) {}

	const char* what() const noexcept override
	{
		return std::exception::what();
	}
};

class calculator
{
private:
	int iterator = -1;
	std::string expression;
	std::ifstream fFrom;
	std::string sFrom;
	std::string sTo;
	List < std::string > expressions;
public:

	calculator(std::string input)
	{
		expression = input;
	}

	void loadFile(std::ifstream& myFile)
	{
		if (myFile.is_open())
		{
			expressions = myFile;
		}
		else
		{
			//throw exception
		}
	}

	int checkSyntax()
	{
		List<char>zatvorky;
		for (std::string::iterator it = expression.begin(); it != expression.end(); ++it)
		{
			switch (*it)
			{
				case '(':
					zatvorky.push_back(*it);
					break;
				case ')':
					zatvorky.pop_front();
					break;
				case '*':
					break;
				case '/':
					break;
				case '+':
					break;
				case '-':
					break;
				case '=':
					break;
				case '.':
					break;
				default:
					if (!std::isdigit(*it)) { return -1; }
			}
		}

		if (zatvorky.empty())
		{
			return 0;
		}

		return -1;
	}

	void calcFile()
	{
		while( expressions.empty() == false )
		{
			expression = expressions.front();
			expressions.pop_front();
			std::cout << calculate() << std::endl;
		}
	}

	char takeNextCharacter() {
		if (expression.size() != (iterator) ) { ++iterator; }

		return(expression[iterator]);
	}

	char takeCurrentCharacter() {
		return(expression[iterator]);
	}

	double calculate() 
	{
		if (checkSyntax() != 0) 
		{
			throw UnknownCharacterException("Nezanmy znak!");
		}

		char Operator;
		char A;
		double C;
		bool flag=false;
		double tmpl, tmpr;

		std::vector<char> leftValue, rightValue;

		leftValue.reserve(6);
		rightValue.reserve(6);

		do
		{
			if (!flag)
			{
				leftValue = makeValue();
			}
			else
			{
				leftValue = flaotToChars(C);
			}

			A = takeCurrentCharacter();

			rightValue = makeValue();

			Operator = A;


			tmpl = charsToDouble(leftValue);
			tmpr = charsToDouble (rightValue);

			C = execOperation(Operator, tmpl, tmpr);

			A = takeCurrentCharacter();

			flag = true;

			if (A == ')')
			{
				return C;
			}
		} while (A != '\0' );

		return C;
	}

	double execOperation(char o, double a, double b)
	{
		switch (o) {
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			return a / b;
		default:
			return -1;
		}
	}

	double charsToDouble(std::vector<char> v) { return std::stod(std::string(v.begin(), v.end())); }

	std::vector<char> flaotToChars(double n) {
		std::vector<char> tmp;
		tmp.reserve(6);
		std::string stmp = std::to_string(n);

		for (char c : stmp)
		{
			tmp.emplace_back(c);
		}

		return tmp;
	}

	std::vector<char> makeValue()
	{
		std::vector<char> Value;
		Value.reserve(6);
		double C;
		char A = takeNextCharacter();
		if (A == '(')
		{
			C = calculate();
			A = takeNextCharacter();
			Value = flaotToChars(C);
		}
		else if (isdigit(A) || isNegative(A))
		{
			do
			{
				Value.push_back(A);
				A = takeNextCharacter();
			} while (isdigit(A) || A == '.');
		}
		return Value;
	}

	void print(double a, double b, char c,double result) {

		std::cout << a << ' ' << c << ' ' << b << " = " << result;
	}

	bool isNegative(char c)
	{
		if (c == '-')
		{
			if (iterator == 0 || expression[iterator - 1] == '(' 
				|| expression[iterator - 1] == '*' || expression[iterator - 1] == '/')
			{
				return true;
			}
		}

		return false; 
	}
};

void print(std::string s)
{
	std::cout << s << " = ?" << std::endl;
}

int main() {

	
	std::string rovnica1 = "-12x25+13.23+(-2.36*-3)+1";
	std::string rovnica2 = "(-12.25+13.23)+(-2.36*-3)+1";
	std::string rovnica3 = "-16.45+13.73+((-2.36*-3)+1=2.1)";

	print(rovnica1);
	calculator calc = rovnica1;

	try
	{
		double result = calc.calculate();
	}
	catch (UnknownCharacterException& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	std::cout << calc.calculate() << std::endl;

	//std::ifstream myFile("rovnice.txt");
	//calc.loadFile(myFile);


	return 0;
}
