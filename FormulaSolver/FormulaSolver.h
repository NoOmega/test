#pragma once

#include <cstdint>
#include <string>

/// <summary> Решатель строковой формулы </summary>
class FormulaSolver 
{
    FormulaSolver();

public:
    /// <summary> Результат решения </summary>
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
        InternalError = 8
    };

    /// <summary> Решить формулу </summary>
    /// <remarks> Парсит входящую строку, строит по ней лексическое дерево и возвращает решение </remarks>
    /// <param name="iFormulaString"> Входящая строка с формулой </param>
    /// <param name="oResult"> Сюда будет записано решение формулы </param>
    /// <param name="iShowTree"> true - выводит дерево в консоль </param>
    /// <returns> Результат работы </returns>
    static Result Solve(const std::string& iFormulaString, double& oResult, bool iShowTree = false);

    /// <summary> Вывести результат в консоль </summary>
    /// <param name="iResult"> Результат работы решателя </param>
    static void ResultToConsole(Result iResult);
};
