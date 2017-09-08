#pragma once

#include <cstdint>
#include <string>

/// Решатель строковой формулы
class FormulaSolver 
{
public:
    /// Результат решения
    enum class Result : std::uint8_t
    {
        Success = 0,
        UnknownError = 1,
        BadParentheses = 2,
        UnknownOperator = 3,
        UnknownOperatorPriority = 4,
        EmptyInputString = 5,
        BadOperand = 6,
        BadCharacter = 7,
        BadTree = 8
    };

    static Result Solve(const std::string& iFormulaString, double& oResult);

    static void ResultToConsole(Result iResult);
};
