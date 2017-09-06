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

int GetOperatorPriority(Operator iOperator)
{
    switch (iOperator)
    {
        case Operator::Addition:
        case Operator::Substraction: return 0;
        case Operator::Multiplication:
        case Operator::Division: return 1;
    }

    return -1;
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

bool FormulaSolver::GetOperator(const char& iCharacter, Operator& oOperator) const
{
    switch (iCharacter)
    {
        case '+': oOperator = Operator::Addition; return true;
        case '-': oOperator = Operator::Substraction; return true;
        case '*': oOperator = Operator::Multiplication; return true;
        case '/': oOperator = Operator::Division; return true;
    }

    return false;
}

FormulaSolver::Result FormulaSolver::RecursiveParseBlock(const std::string& iString, NodePtr& oBlockNode) const
{
    if (iString.empty())
        return Result::EmptyInputString;

    double firstNum = 0.;
    auto startIter = iString.begin();

    if (IsOperator(*startIter))
        return Result::BadOperand;

    if (IsOpeningParanthesisBeforeNextOperator(iString, startIter))
        /// RecursiveParseBlock here

    if (!GetRightOperand(iString, startIter, firstNum))
        return Result::BadOperand;

    oBlockNode = Node::Create(firstNum);

    auto currentOperatorIter = FindNextOperator(iString, startIter);
    while (currentOperatorIter != startIter)
    {
        auto res = RecursiveParseOperation(std::move(oBlockNode), iString, currentOperatorIter, oBlockNode);
        if (res != Result::Success)
            return res;

        startIter = currentOperatorIter;
        currentOperatorIter = FindNextOperator(iString, startIter);
    }

    return Result::Success;
}

FormulaSolver::Result FormulaSolver::RecursiveParseOperation(NodePtr&& iLeft, const std::string& iString, std::string::const_iterator& iOperatorIter, NodePtr& oOperationNode) const
{
    double num = 0.;
    auto startIter = iString.begin();
    if (!GetRightOperand(iString, iOperatorIter, num))
        return Result::BadOperand;

    Operator currentOperator;
    if (!GetOperator(*iOperatorIter, currentOperator))
        return Result::UnknownOperator;

    auto right = Node::Create(num);

    auto nextOperatorIter = FindNextOperator(iString, iOperatorIter);
    if (nextOperatorIter != iOperatorIter)
    {
        Operator nextOperator;
        if (!GetOperator(*nextOperatorIter, nextOperator))
            return Result::UnknownOperator;

        // Проверка приоритетов текущей и следующей операций
        auto curPriority = GetOperatorPriority(currentOperator);
        auto nextPriority = GetOperatorPriority(nextOperator);
        if (curPriority < 0 || nextPriority < 0)
            return Result::UnknownOperatorPriority;

        if (curPriority < nextPriority)
        {
            // У следующего оператора больше приоритет - создаём сначала его
            auto res = RecursiveParseOperation(std::move(right), iString, nextOperatorIter, right);
            if (res != Result::Success)
                return res;

            iOperatorIter = nextOperatorIter;
        }
    }

    oOperationNode = Node::Create(currentOperator, std::move(iLeft), std::move(right));
    return Result::Success;
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

bool FormulaSolver::IsOpeningParanthesisBeforeNextOperator(const std::string& iString, std::string::const_iterator iStart) const
{
    for (auto iter = iStart; iter != iString.end(); ++iter)
    {
        if (*iter == '(')
            return true;
        else if (IsOperator(*iter))
            return false;
    }

    return false;
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
            if (*iter == ' ')
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
