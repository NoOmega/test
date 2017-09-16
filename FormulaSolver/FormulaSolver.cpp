#include "FormulaSolver.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>


/// Операторы
enum class Operator : std::uint8_t
{
    Addition = 0,
    Substraction = 1,
    Multiplication = 2,
    Division = 3
};

/// Вспомогательные функции парсинга
namespace ParserUtils
{
    static int GetOperatorPriority(Operator iOperator)
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

    static bool IsOperator(const char& iCharacter)
    {
        return iCharacter == '+' ||
            iCharacter == '-' ||
            iCharacter == '/' ||
            iCharacter == '*';
    }

    static bool IsNumeric(const char& iCharacter)
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

    static bool GetNumber(const std::string& iString, double& oNumber)
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

    static std::string GetString(double iNumber)
    {
        std::ostringstream strs;
        strs << iNumber;
        return strs.str();
    }

    static bool GetOperator(const char& iCharacter, Operator& oOperator)
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

    static bool GetString(Operator iOperator, std::string& oString)
    {
        switch (iOperator)
        {
            case Operator::Addition: oString = "+"; return true;
            case Operator::Substraction: oString = "-"; return true;
            case Operator::Multiplication: oString = "*"; return true;
            case Operator::Division: oString = "/"; return true;
        }

        return false;
    }

    static std::string::const_iterator FindNextOperator(const std::string& iString, std::string::const_iterator iStart)
    {
        size_t innerBlockCount = 0;
        bool foundOpen = false;
        if (*iStart == '(')
        {
            innerBlockCount++;
            foundOpen = true;
        }

        for (auto iter = iStart + 1; iter != iString.end(); ++iter)
        {
            if (*iter == '(')
            {
                innerBlockCount++;
                foundOpen = true;
            }
            else if (*iter == ')')
            {
                if (foundOpen == false)
                    return iStart;

                innerBlockCount--;
            }

            if (innerBlockCount == 0 && IsOperator(*iter))
                return iter;
        }

        return iStart;
    }

    static bool IsOpeningBracketBeforeNextOperator(const std::string& iString, std::string::const_iterator iStart, bool isOpeningBracket = true)
    {
        auto bracket = isOpeningBracket ? '(' : ')';
        auto opposite = isOpeningBracket ? ')' : '(';
        for (auto iter = iStart; iter != iString.end(); ++iter)
        {
            if (*iter == bracket)
                return true;
            else if (IsOperator(*iter) || *iter == opposite)
                return false;
        }

        return false;
    }

    static bool IsClosingBracketBeforeNextOperator(const std::string& iString, std::string::const_iterator iStart)
    {
        return IsOpeningBracketBeforeNextOperator(iString, iStart, false);
    }

    static bool GetRightOperand(const std::string& iString, std::string::const_iterator iOperator, double& oNumber)
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

    static void MoveIterNextAfterBlock(const std::string& iString, std::string::const_iterator& ioIter)
    {
        while (*ioIter != ')' && ioIter + 1 != iString.end())
        {
            ++ioIter;
        }
        ++ioIter;
    }

    static void MoveIterIntoNextBlock(const std::string& iString, std::string::const_iterator& ioIter)
    {
        while (*ioIter != '(' && ioIter + 1 != iString.end())
        {
            ++ioIter;
        }
        ++ioIter;
    }
}

/// Лексическое дерево
namespace Tree
{
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

        virtual FormulaSolver::Result GetValue(double& oValue) const = 0;

        virtual void PrintToConsole(std::vector<std::string>& ioLevels, size_t iLevel, std::vector<std::vector<size_t>>& ioVConnections) const = 0;
    };

    /// Узел с константой
    class ConstantNode : public Node
    {
    public:
        explicit ConstantNode(double iValue) : _value(iValue) {}

        virtual FormulaSolver::Result GetValue(double& oValue) const override;

        virtual void PrintToConsole(std::vector<std::string>& ioLevels, size_t iLevel, std::vector<std::vector<size_t>>& ioVConnections) const override;

    private:
        double _value;
    };

    /// Узел с операцией
    class OperationNode : public Node
    {
    public:
        OperationNode(Operator iOperator, NodePtr&& iLeft, NodePtr&& iRight);

        virtual FormulaSolver::Result GetValue(double& oValue) const override;

        virtual void PrintToConsole(std::vector<std::string>& ioLevels, size_t iLevel, std::vector<std::vector<size_t>>& ioVConnections) const override;

    private:
        NodePtr _left;
        NodePtr _right;
        Operator _operator;
    };


    // NODE

    NodePtr Node::Create(double iValue)
    {
        return NodePtr(new ConstantNode(iValue));
    }

    NodePtr Node::Create(Operator iOperator, NodePtr&& iLeft, NodePtr&& iRight)
    {
        return NodePtr(new OperationNode(iOperator, std::move(iLeft), std::move(iRight)));
    }

    // CONSTATNT NODE

    FormulaSolver::Result ConstantNode::GetValue(double& oValue) const
    {
        oValue = _value;
        return FormulaSolver::Result::Success;
    }

    void ConstantNode::PrintToConsole(std::vector<std::string>& ioLevels, size_t iLevel, std::vector<std::vector<size_t>>& ioVConnections) const
    {
        ioLevels[iLevel] += "(" + ParserUtils::GetString(_value) + ")";
    }

    // OPERATION NODE

    const char vConnect = '|';
    const std::string hConnect = "--";
    const std::string emptyConsoleLine = "                                                                               ";

    OperationNode::OperationNode(Operator iOperator, NodePtr&& iLeft, NodePtr&& iRight)
    {
        _left = std::move(iLeft);
        _right = std::move(iRight);
        _operator = iOperator;
    }

    FormulaSolver::Result OperationNode::GetValue(double& oValue) const
    {
        if (_left == nullptr || _right == nullptr)
            return FormulaSolver::Result::InternalError;

        double a = 0., b = 0.;
        auto res1 = _left->GetValue(a);
        if (res1 != FormulaSolver::Result::Success)
            return res1;
        auto res2 = _right->GetValue(b);
        if (res2 != FormulaSolver::Result::Success)
            return res2;

        switch (_operator)
        {
            case Operator::Addition:
            {
                oValue = a + b;
                return FormulaSolver::Result::Success;
            }
            case Operator::Substraction:
            {
                oValue = a - b;
                return FormulaSolver::Result::Success;
            }
            case Operator::Multiplication:
            {
                oValue = a * b;
                return FormulaSolver::Result::Success;
            }
            case Operator::Division:
            {
                if (b == 0.)
                    return FormulaSolver::Result::DivideByZero;

                oValue = a / b;
                return FormulaSolver::Result::Success;
            }
        }

        return FormulaSolver::Result::InternalError;
    }

    void OperationNode::PrintToConsole(std::vector<std::string>& ioLevels, size_t iLevel, std::vector<std::vector<size_t>>& ioVConnections) const
    {
        auto startIndex = ioLevels[iLevel].size();

        // Выводим оператор
        std::string operatorStr;
        if (!ParserUtils::GetString(_operator, operatorStr))
            return;

        ioLevels[iLevel] += "(" + operatorStr + ")";

        // Правый операнд
        ioLevels[iLevel] += hConnect;
        _right->PrintToConsole(ioLevels, iLevel, ioVConnections);

        // Вертикальная связь
        for (auto i = iLevel; i < ioLevels.size(); ++i)
        {
            ioVConnections[i].push_back(startIndex + 1);
        }

        // Добавляем новый уровень
        ioLevels.push_back("");
        ioVConnections.push_back(std::vector<size_t>());

        // Добавляем пробелы слева
        for (decltype(startIndex) i = 0; i < startIndex; ++i)
        {
            ioLevels.back() += " ";
        }

        // Левый операнд
        _left->PrintToConsole(ioLevels, ioLevels.size() - 1, ioVConnections);
    }

    /// TREE

    void PrintToConsole(const NodePtr& iTopNode)
    {
        if (iTopNode == nullptr)
        {
            std::cout << std::endl << "Empty tree.";
            return;
        }

        std::vector<std::string> levels;
        levels.push_back("");
        std::vector<std::vector<size_t>> vConnections;
        vConnections.push_back(std::vector<size_t>());

        iTopNode->PrintToConsole(levels, 0, vConnections);

        std::vector<std::string> subLevels;
        for (const auto& vConnLevel : vConnections)
        {
            subLevels.push_back(emptyConsoleLine);
            for (const auto& vConnIndex : vConnLevel)
            {
                subLevels.back()[vConnIndex] = vConnect;
                if (levels.at(subLevels.size() - 1).at(vConnIndex) == ' ')
                    levels.at(subLevels.size() - 1).at(vConnIndex) = vConnect;
            }
        }

        // Output
        std::cout << std::endl;
        for (size_t i = 0; i < levels.size(); ++i)
        {
            std::cout << levels.at(i) << std::endl << subLevels.at(i) << std::endl;
        }
    }
}

// PARSER

class Parser
{
    Parser();

public:
    static FormulaSolver::Result RecursiveParseBlock(const std::string& iString, std::string::const_iterator& iBlockStartIter, Tree::NodePtr& oBlockNode)
    {
        if (iBlockStartIter == iString.end())
            return FormulaSolver::Result::Success;

        // Обработка отрицательного числа в начале блока
        double firstNum = 0.;
        bool firstNumIsNegative = false;
        if (ParserUtils::IsOperator(*iBlockStartIter))
        {
            if (*iBlockStartIter == '-')
            {
                firstNumIsNegative = true;
                ++iBlockStartIter;
            }
            else
                return FormulaSolver::Result::BadOperand;
        }

        if (ParserUtils::IsOpeningBracketBeforeNextOperator(iString, iBlockStartIter))
        {
            // Если блок начинается с вложенного блока - парсим вложенный
            auto blockStart = iBlockStartIter;
            auto res = RecursiveParseBlock(iString, ++iBlockStartIter, oBlockNode);
            if (res != FormulaSolver::Result::Success)
                return res;

            // Обработка минуса перед скобкой
            if (firstNumIsNegative)
                oBlockNode = Tree::Node::Create(Operator::Multiplication, std::move(oBlockNode), Tree::Node::Create(-1));

            // Если блок состоит только из вложенного - возвращаем результат
            if (iBlockStartIter == iString.end())
                return res;

            iBlockStartIter = blockStart;
        }
        else
        {
            // Берём первое число от начала блока
            if (!ParserUtils::GetRightOperand(iString, iBlockStartIter, firstNum))
                return FormulaSolver::Result::BadOperand;

            if (firstNumIsNegative)
                firstNum = -firstNum;

            oBlockNode = Tree::Node::Create(firstNum);

            // Обработка блоков без операций (-3), (2), ()
            if (ParserUtils::IsClosingBracketBeforeNextOperator(iString, iBlockStartIter))
            {
                ParserUtils::MoveIterNextAfterBlock(iString, iBlockStartIter);
                return FormulaSolver::Result::Success;
            }
        }

        // Пока в блоке есть операторы - парсим их цепочками по приоритету
        do
        {
            auto res = RecursiveParseOperatorChain(std::move(oBlockNode), iString, iBlockStartIter, oBlockNode);
            if (res != FormulaSolver::Result::Success)
                return res;

        } while (iBlockStartIter != ParserUtils::FindNextOperator(iString, iBlockStartIter));

        return FormulaSolver::Result::Success;
    }

    static FormulaSolver::Result RecursiveParseOperatorChain(Tree::NodePtr&& iLeftOperand, const std::string& iString, std::string::const_iterator& iStartIter, Tree::NodePtr& oChainNode)
    {
        oChainNode = std::move(iLeftOperand);
        bool endOfChain = false;

        auto currentOperatorIter = ParserUtils::FindNextOperator(iString, iStartIter);
        while (currentOperatorIter != iStartIter)
        {
            Operator currentOperator;
            if (!ParserUtils::GetOperator(*currentOperatorIter, currentOperator))
                return FormulaSolver::Result::UnknownOperator;

            Tree::NodePtr right = nullptr;
            if (ParserUtils::IsOpeningBracketBeforeNextOperator(iString, currentOperatorIter + 1))
            {
                // Если правый операнд - блок, парсим его
                auto blockIter = currentOperatorIter + 1;
                ParserUtils::MoveIterIntoNextBlock(iString, blockIter);

                auto res = RecursiveParseBlock(iString, blockIter, right);
                if (res != FormulaSolver::Result::Success)
                    return res;
            }
            else
            {
                // Берём правый операнд и создаём узел
                double num = 0.;
                if (!ParserUtils::GetRightOperand(iString, currentOperatorIter, num))
                    return FormulaSolver::Result::BadOperand;

                right = Tree::Node::Create(num);
            }

            if (!ParserUtils::IsClosingBracketBeforeNextOperator(iString, currentOperatorIter + 1))
            {
                // Находим следующий оператор
                auto nextOperatorIter = ParserUtils::FindNextOperator(iString, currentOperatorIter);
                if (nextOperatorIter != currentOperatorIter)
                {
                    Operator nextOperator;
                    if (!ParserUtils::GetOperator(*nextOperatorIter, nextOperator))
                        return FormulaSolver::Result::UnknownOperator;

                    // Проверка приоритетов текущего и следующего оператора
                    auto currentPriority = ParserUtils::GetOperatorPriority(currentOperator);
                    auto nextPriority = ParserUtils::GetOperatorPriority(nextOperator);
                    if (currentPriority < 0 || nextPriority < 0)
                        return FormulaSolver::Result::UnknownOperatorPriority;

                    if (currentPriority < nextPriority)
                    {
                        // У следующего оператора больше приоритет - парсим следующую цепочку операторов
                        auto res = RecursiveParseOperatorChain(std::move(right), iString, currentOperatorIter, right);
                        if (res != FormulaSolver::Result::Success)
                            return res;
                    }
                    else if (currentPriority > nextPriority)
                    {
                        endOfChain = true;
                        iStartIter = currentOperatorIter;
                    }
                }
            }

            // Создаём операцию
            oChainNode = Tree::Node::Create(currentOperator, std::move(oChainNode), std::move(right));

            if (endOfChain)
                return FormulaSolver::Result::Success;

            iStartIter = currentOperatorIter;
            currentOperatorIter = ParserUtils::FindNextOperator(iString, iStartIter);
        }

        return FormulaSolver::Result::Success;
    }
};

// FORMULA SOLVER

FormulaSolver::Result FormulaSolver::Solve(const std::string& iFormulaString, double& oResult, bool iShowTree)
{
    // Убираем пробелы и проверяем скобки
    auto formula = iFormulaString;
    auto bracketStack = 0;
    for (auto it = formula.begin(); it != formula.end();)
    {
        if (bracketStack < 0)
            return Result::BadParentheses;

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
            else if (!ParserUtils::IsOperator(*it) && !ParserUtils::IsNumeric(*it))
                return Result::BadCharacter;
            it++;
        }
    }

    if (iFormulaString.empty())
        return Result::EmptyInputString;

    if (bracketStack != 0)
        return Result::BadParentheses;

    // Парсим формулу и строим дерево
    Tree::NodePtr top = nullptr;
    auto res = Parser::RecursiveParseBlock(formula, formula.begin(), top);
    if (res != Result::Success)
        return res;

    if (top == nullptr)
        return Result::InternalError;

    // Вывод дерева
    if (iShowTree)
        Tree::PrintToConsole(top);

    return top->GetValue(oResult);
}

void FormulaSolver::ResultToConsole(FormulaSolver::Result iResult)
{
    std::cout << std::endl;
    switch (iResult)
    {
        case Result::UnknownError: std::cout << "Unknown error"; break;
        case Result::BadOperand: std::cout << "There is no operand for some operator"; break;
        case Result::BadParentheses: std::cout << "Error in brackets"; break;
        case Result::EmptyInputString: std::cout << "Empty formula"; break;
        case Result::UnknownOperator: std::cout << "There is unknown operator"; break;
        case Result::UnknownOperatorPriority: std::cout << "You forgot to set priority for new operator in \"GetOperatorPriority()\""; break;
        case Result::BadCharacter: std::cout << "Bad character"; break;
        case Result::InternalError: std::cout << "Internal solver error, revision required"; break;
        case Result::DivideByZero: std::cout << "Cannot divide by zero"; break;
    }
}
