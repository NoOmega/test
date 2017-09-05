#include "FormulaSolver.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


class Error : public std::runtime_error
{
public:
    Error(const std::string& s) : std::runtime_error(s) {}
};

unsigned int GetOperatorPriority(Operator iOperator)
{
    switch (iOperator)
    {
        case Operator::Addition:
        case Operator::Substraction: return 0;
        case Operator::Multiplication:
        case Operator::Division: return 1;
        case Operator::Undefined: throw Error("GetOperatorPriority(Operator::Undefined)");
    }
}


// NODE

NodePtr Node::Create(double iValue)
{
    return NodePtr(new ConstantNode(iValue));
}

NodePtr Node::Create(Operator iOperator, NodePtr&& iFirst, NodePtr&& iSecond)
{
    return NodePtr(new OperationNode(iOperator, std::move(iFirst), std::move(iSecond)));
}

// CONSTATNT NODE

bool ConstantNode::GetValue(double& oValue) const
{
    oValue = _value;
    return true;
}

// OPERATION NODE

OperationNode::OperationNode(Operator iOperator, NodePtr&& iFirst, NodePtr&& iSecond)
{
    _first = std::move(iFirst);
    _second = std::move(iSecond);
    _operator = iOperator;
}

bool OperationNode::GetValue(double& oValue) const
{
    if (_first == nullptr || _second == nullptr)
        return false;

    double a = 0., b = 0.;
    if (!_first->GetValue(a) || !_second->GetValue(b))
        return false;

    switch (_operator)
    {
        case Operator::Addition:
        {
            oValue = a + b;
            return true;
        }
        case Operator::Substraction:
        {
            oValue = a - b;
            return true;
        }
        case Operator::Multiplication:
        {
            oValue = a * b;
            return true;
        }
        case Operator::Division:
        {
            if (b == 0.)
                return false;

            oValue = a / b;
            return true;
        }
    }

    return false;
}

// FORMULA SOLVER

FormulaSolver::FormulaSolver() :
    _top(nullptr)
{
}

FormulaSolver::Result FormulaSolver::ParseFormula(const std::string& iFormulaString)
{
    _top = nullptr;

    if (iFormulaString.empty())
        return Result::EmptyInputString;

    
}

bool FormulaSolver::Solve(double& oResult) const
{
    if (_top == nullptr)
        return false;

    return _top->GetValue(oResult);
}

bool FormulaSolver::IsOperator(const char& iCharacter) const
{
    return iCharacter == '+' ||
           iCharacter == '-' ||
           iCharacter == '/' ||
           iCharacter == '*';
}

bool FormulaSolver::IsNumeric(const char& iCharacter) const
{
    return iCharacter == '0' ||
           iCharacter == '1' ||
           iCharacter == '2' ||
           iCharacter == '3' ||
           iCharacter == '4' ||
           iCharacter == '5' ||
           iCharacter == '6' ||
           iCharacter == '7' ||
           iCharacter == '8' ||
           iCharacter == '9' ||
           iCharacter == '.' ||
           iCharacter == ',';
}

bool FormulaSolver::GetNumber(const std::string& iString, double& oNumber) const
{
    std::istringstream stream(iString);
    double result = 0.;
    if (stream >> result)
    {
        oNumber = result;
        return true;
    }

    return false;
}

Operator FormulaSolver::GetOperator(const char& iCharacter) const
{
    switch (iCharacter)
    {
        case '+': return Operator::Addition;
        case '-': return Operator::Substraction;
        case '*': return Operator::Multiplication;
        case '/': return Operator::Division;
    }

    return Operator::Undefined;
}

NodePtr FormulaSolver::ParseBlock(const std::string& iString) const
{
    if (iString.empty())
        return nullptr;

    double firstNum = 0.;
    auto startIter = iString.begin();
    if (!GetRightOperand(iString, startIter, firstNum))
        return nullptr;

    auto result = Node::Create(firstNum);

    auto currentOperatorIter = FindNextOperator(iString, startIter);
    while (currentOperatorIter != startIter)
    {
        result = ParseOperation(std::move(result), iString, currentOperatorIter);

        startIter = currentOperatorIter;
        currentOperatorIter = FindNextOperator(iString, startIter);
    }

    return result;
}

NodePtr FormulaSolver::ParseOperation(NodePtr&& iLeft, const std::string& iString, std::string::const_iterator& iOperatorIter) const
{
    double num = 0.;
    auto startIter = iString.begin();
    if (!GetRightOperand(iString, iOperatorIter, num))
        return nullptr;

    auto currentOperator = GetOperator(*iOperatorIter);

    auto right = Node::Create(num);

    auto nextOperatorIter = FindNextOperator(iString, iOperatorIter);
    if (nextOperatorIter != iOperatorIter)
    {
        auto nextOperator = GetOperator(*nextOperatorIter);

        if (GetOperatorPriority(currentOperator) < GetOperatorPriority(nextOperator))
        {
            // У следующего оператора больше приоритет - создаём сначала его
            right = ParseOperation(std::move(right), iString, nextOperatorIter);
            if (right == nullptr)
                return nullptr;

            iOperatorIter = nextOperatorIter;
        }
    }

    return Node::Create(currentOperator, std::move(iLeft), std::move(right));
}

std::string::const_iterator FormulaSolver::FindNextOperator(const std::string& iString, std::string::const_iterator iStart) const
{
    for (auto iter = iStart + 1; iter != iString.end(); ++iter)
    {
        if (IsOperator(*iter))
            return iter;
    }

    return iStart;
}

bool FormulaSolver::FindNextBlock(const std::string& iString, std::string::iterator iStart, std::string& oBlock) const
{

}

bool FormulaSolver::GetLeftOperand(const std::string& iString, std::string::const_iterator iOperator, double& oNumber) const
{

}

bool FormulaSolver::GetRightOperand(const std::string& iString, std::string::const_iterator iOperator, double& oNumber) const
{
    auto iter = iOperator + 1;
    std::string str;

    for (auto iter = iOperator + 1; iter != iString.end(); ++iter)
    {
        if (!IsNumeric(*iter))
        {
            if (str.empty())
                continue;
            else
                break;
        }

        str.push_back(*iter);
    }

    if (str.empty())
        return false;

    return GetNumber(str, oNumber);
}
