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

    // Убираем пробелы
    auto formula = iFormulaString;
    auto bracketStack = 0;
    for (auto it = formula.begin(); it != formula.end();)
    {
        if (*it == ' ')
        {
            it = formula.erase(it);
        }
        else
        {
            if (*it == '(')
                bracketStack++;
            else if (*it == ')')
                bracketStack--;
            else if (!IsOperator(*it) && !IsNumeric(*it))
                return Result::BadCharacter;
            it++;
        }
    }

    if (iFormulaString.empty())
        return Result::EmptyInputString;

    if (bracketStack != 0)
        return Result::BadParentheses;

    return RecursiveParseBlock(formula, formula.begin(), _top);
}

bool FormulaSolver::Solve(double& oResult) const
{
    if (_top == nullptr)
        return false;

    return _top->GetValue(oResult);
}

void FormulaSolver::ResultToConsole(FormulaSolver::Result iResult)
{
    std::cout << std::endl;
    switch (iResult)
    {
        case FormulaSolver::Result::UnknownError: std::cout << "Unknown error"; break;
        case FormulaSolver::Result::BadOperand: std::cout << "There is no operand for some operator"; break;
        case FormulaSolver::Result::BadParentheses: std::cout << "Error in brackets"; break;
        case FormulaSolver::Result::EmptyInputString: std::cout << "Empty formula"; break;
        case FormulaSolver::Result::UnknownOperator: std::cout << "There is unknown operator"; break;
        case FormulaSolver::Result::UnknownOperatorPriority: std::cout << "The creature that added new operator forgot to set priority in \"GetOperatorPriority()\""; break;
        case FormulaSolver::Result::BadCharacter: std::cout << "Bad character"; break;
    }
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

FormulaSolver::Result FormulaSolver::RecursiveParseBlock(const std::string& iString, std::string::const_iterator& iBlockStartIter, NodePtr& oBlockNode) const
{
    if (iBlockStartIter == iString.end())
        return Result::Success;

    double firstNum = 0.;
    bool firstNumIsNegative = false;
    if (IsOperator(*iBlockStartIter))
    {
        if (*iBlockStartIter == '-')
            firstNumIsNegative = true;
        else
            return Result::BadOperand;
    }

    if (IsOpeningParanthesisBeforeNextOperator(iString, iBlockStartIter))
    {
        // Если блок начинается с вложенного блока - парсим вложенный
        auto res = RecursiveParseBlock(iString, ++iBlockStartIter, oBlockNode);
        if (res != Result::Success)
            return res;

        // Если блок состоит только из вложенного - возвращаем результат
        if (iBlockStartIter == iString.end())
            return res;
    }
    else
    {
        // Берём первое число от начала блока
        if (!GetRightOperand(iString, iBlockStartIter, firstNum))
            return Result::BadOperand;

        if (firstNumIsNegative)
            firstNum = -firstNum;

        oBlockNode = Node::Create(firstNum);

        // (-3), (2), ()
        if (IsClosingParanthesisBeforeNextOperator(iString, iBlockStartIter))
        {
            MoveIterNextAfterBlock(iString, iBlockStartIter);
            return Result::Success;
        }
    }

    auto currentOperatorIter = FindNextOperator(iString, iBlockStartIter);
    while (currentOperatorIter != iBlockStartIter)
    {
        auto res = RecursiveParseOperation(std::move(oBlockNode), iString, currentOperatorIter, oBlockNode);
        if (res != Result::Success)
            return res;

        if (IsClosingParanthesisBeforeNextOperator(iString, currentOperatorIter))
        {
            MoveIterNextAfterBlock(iString, currentOperatorIter);
            iBlockStartIter = currentOperatorIter - 1;
            return Result::Success;
        }

        iBlockStartIter = currentOperatorIter;
        currentOperatorIter = FindNextOperator(iString, iBlockStartIter);
    }

    return Result::Success;
}

FormulaSolver::Result FormulaSolver::RecursiveParseOperation(NodePtr&& iLeft, const std::string& iString, std::string::const_iterator& iOperatorIter, NodePtr& oOperationNode) const
{
    Operator currentOperator;
    if (!GetOperator(*iOperatorIter, currentOperator))
        return Result::UnknownOperator;

    NodePtr right = nullptr;
    if (IsOpeningParanthesisBeforeNextOperator(iString, iOperatorIter + 1))
    {
        auto blockIter = iOperatorIter + 1;
        MoveIterIntoNextBlock(iString, blockIter);

        auto res = RecursiveParseBlock(iString, blockIter, right);
        if (res != Result::Success)
            return res;
    }
    else
    {
        double num = 0.;
        if (!GetRightOperand(iString, iOperatorIter, num))
            return Result::BadOperand;

        right = Node::Create(num);
    }

    if (IsClosingParanthesisBeforeNextOperator(iString, iOperatorIter + 1))
    {
        ++iOperatorIter;
    }
    else
    {
        auto nextOperatorIter = FindNextOperator(iString, iOperatorIter);
        if (nextOperatorIter != iOperatorIter)
        {
            Operator nextOperator;
            if (!GetOperator(*nextOperatorIter, nextOperator))
                return Result::UnknownOperator;

            // Проверка приоритетов текущей и следующей операций
            auto currPriority = GetOperatorPriority(currentOperator);
            auto nextPriority = GetOperatorPriority(nextOperator);
            if (currPriority < 0 || nextPriority < 0)
                return Result::UnknownOperatorPriority;

            if (currPriority < nextPriority)
            {
                // У следующего оператора больше приоритет - создаём сначала его
                auto res = RecursiveParseOperation(std::move(right), iString, nextOperatorIter, right);
                if (res != Result::Success)
                    return res;

                iOperatorIter = nextOperatorIter;
            }
        }
    }

    oOperationNode = Node::Create(currentOperator, std::move(iLeft), std::move(right));
    return Result::Success;
}

std::string::const_iterator FormulaSolver::FindNextOperator(const std::string& iString, std::string::const_iterator iStart) const
{
    size_t innerBlockCount = 0;
    for (auto iter = iStart + 1; iter != iString.end(); ++iter)
    {
        if (*iter == '(')
            innerBlockCount++;
        else if (*iter == ')')
            innerBlockCount--;

        if (innerBlockCount == 0 && IsOperator(*iter))
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

bool FormulaSolver::IsClosingParanthesisBeforeNextOperator(const std::string& iString, std::string::const_iterator iStart) const
{
    for (auto iter = iStart; iter != iString.end(); ++iter)
    {
        if (*iter == ')')
            return true;
        else if (IsOperator(*iter))
            return false;
    }

    return false;
}

bool FormulaSolver::GetLeftOperand(const std::string& iString, std::string::const_iterator iOperator, double& oNumber) const
{
    return false;
}

bool FormulaSolver::GetRightOperand(const std::string& iString, std::string::const_iterator iOperator, double& oNumber) const
{
    std::string str;
    auto iter = iOperator;
    if (IsOperator(*iOperator))
        ++iter;
    for (; iter != iString.end(); ++iter)
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

void FormulaSolver::MoveIterNextAfterBlock(const std::string& iString, std::string::const_iterator& ioIter) const
{
    while (*ioIter != ')' && ioIter + 1 != iString.end())
    {
        ++ioIter;
    }
    ++ioIter;
}

void FormulaSolver::MoveIterIntoNextBlock(const std::string& iString, std::string::const_iterator& ioIter) const
{
    while (*ioIter != '(' && ioIter + 1 != iString.end())
    {
        ++ioIter;
    }
    ++ioIter;
}
