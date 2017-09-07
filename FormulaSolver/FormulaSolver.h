#pragma once

#include <memory>
#include <cstdint>
#include <string>


enum class Operator : std::uint8_t
{
    Addition = 0,
    Substraction = 1,
    Multiplication = 2,
    Division = 3
};

int GetOperatorPriority(Operator iOperator);


//using NodePtr = std::unique_ptr<class Node>;
typedef std::unique_ptr<class Node> NodePtr;

/// Базовывй класс узла дерева
class Node
{
protected:
    Node() {}
public:
    virtual ~Node() {}

private:
    Node(const Node&);
    void operator=(const Node&);

public:
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
        BadParentheses = 2,
        UnknownOperator = 3,
        UnknownOperatorPriority = 4,
        EmptyInputString = 5,
        BadOperand = 6,
        BadCharacter = 7
    };

    Result ParseFormula(const std::string& iFormulaString);

    bool Solve(double& oResult) const;

    static void ResultToConsole(Result iResult);

private:
    bool IsOperator(const char& iCharacter) const;

    bool IsNumeric(const char& iCharacter) const;

    bool GetNumber(const std::string& iString, double& oNumber) const;

    bool GetOperator(const char& iCharacter, Operator& oOperator) const;

    Result RecursiveParseBlock(const std::string& iString, std::string::const_iterator& iBlockStartIter, NodePtr& oBlockNode) const;
    Result RecursiveParseOperation(NodePtr&& iLeft, const std::string& iString, std::string::const_iterator& iOperatorIter, NodePtr& oOperationNode) const;

    std::string::const_iterator FindNextOperator(const std::string& iString, std::string::const_iterator iStart) const;

    bool IsOpeningParanthesisBeforeNextOperator(const std::string& iString, std::string::const_iterator iStart) const;
    bool IsClosingParanthesisBeforeNextOperator(const std::string& iString, std::string::const_iterator iStart) const;

    bool GetLeftOperand(const std::string& iString, std::string::const_iterator iFrom, double& oNumber) const;
    bool GetRightOperand(const std::string& iString, std::string::const_iterator iFrom, double& oNumber) const;

    void MoveIterNextAfterBlock(const std::string& iString, std::string::const_iterator& ioIter) const;
    void MoveIterIntoNextBlock(const std::string& iString, std::string::const_iterator& ioIter) const;

private:
    NodePtr _top;
};
