#pragma once

#include <memory>
#include <cstdint>
#include <string>


enum class Operator : std::uint8_t
{
	Undefined = 0,
	Addition = 1,
	Substraction = 2,
	Multiplication = 3,
	Division = 4
};

unsigned int GetOperatorPriority(Operator iOperator);


using NodePtr = std::unique_ptr<Node>;

/// Базовывй класс узла дерева
class Node
{
protected:
	Node() {}
	virtual ~Node() {}

public:
	Node(const Node&) = delete;
	void operator=(const Node&) = delete;

	static NodePtr Create(double iValue);
	static NodePtr Create(Operator iOperator, NodePtr&& iFirst, NodePtr&& iSecond);

	virtual bool GetValue(double& oValue) const = 0;
};


/// Узел с константой
class ConstantNode : public Node
{
public:
	explicit ConstantNode(double iValue) : _value(iValue) {}

	virtual bool GetValue(double& oValue) const override;

private:
	double _value;
};


/// Узел с операцией
class OperationNode : public Node
{
public:
	OperationNode(Operator iOperator, NodePtr&& iFirst, NodePtr&& iSecond);

	virtual bool GetValue(double& oValue) const override;
	
private:
	NodePtr _first;
	NodePtr _second;
	Operator _operator;
};



/// Синтаксическое дерево формулы
class FormulaSolver
{
public:
	FormulaSolver();
    ~FormulaSolver() {}

    enum class Result : std::uint8_t
    {
        Success = 0,
        UnknownError = 1,
        BadParenthesis = 2,
        UnknownOperation = 3,
        EmptyInputString = 4,
    };

    Result ParseFormula(const std::string& iFormulaString);

    bool Solve(double& oResult) const;

private:
    bool IsOperator(const char& iCharacter) const;

    bool IsNumeric(const char& iCharacter) const;

    bool GetNumber(const std::string& iString, double& oNumber) const;

    Operator GetOperator(const char& iCharacter) const;

    NodePtr ParseBlock(const std::string& iString) const;
    NodePtr ParseOperation(NodePtr&& iLeft, const std::string& iString, std::string::const_iterator& iOperatorIter) const;

    std::string::const_iterator FindNextOperator(const std::string& iString, std::string::const_iterator iStart) const;

    bool FindNextBlock(const std::string& iString, std::string::iterator iStart, std::string& oBlock) const;

    bool GetLeftOperand(const std::string& iString, std::string::const_iterator iFrom, double& oNumber) const;
    bool GetRightOperand(const std::string& iString, std::string::const_iterator iFrom, double& oNumber) const;

private:
	NodePtr _top;
};

