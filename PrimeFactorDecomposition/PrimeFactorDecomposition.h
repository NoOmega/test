#include <vector>
#include <string>

/// <summary> Список простых чисел от 2 до N </summary>
class Primes
{
public:
    /// <summary> Получить список простых чисел </summary>
    /// <remarks> Если параметер iUpTo больше максимального простого числа в списке - дополняет список </remarks>
    /// <param name="iUpTo"> Максимальное простое число в списке должно быть больше или равно этому параметру </param>
    /// <returns> Список простых чисел от 2 до больше или равно iUpTo </returns>
    static const std::vector<const int>& Get(const int iUpTo);

private:
    Primes();

private:
    static std::vector<const int> _primes;
};

/// <summary> Результат разложения числа на простые множители </summary>
class PrimeFactorDecompositionResult
{
public:
    /// Конструктор
    explicit PrimeFactorDecompositionResult(int iNumber) : _number(iNumber) {}

    /// <summary> Добавить множитель </summary>
    /// <param name="iFactor"> Множитель </param>
    void AddFactor(int iFactor) { _factors.push_back(iFactor); }
    
    /// <summary> Получить результат в формате строки ('number' = 'factor' * 'factor' * ...) </summary>
    /// <returns> Результат в формате строки </returns>
    std::string GetAsString() const;

    /// <summary> Проверка результата </summary>
    /// <returns> true - если результат верный, false - иначе </returns>
    bool IsCorrect() const;

private:
    int _number;
    std::vector<int> _factors;
};

/// <summary> Функция разложения натуральных чисел от 1 до N на простые множители. </summary>
/// <remarks> В данном алгоритме каждое натуральное число от 1 до N проходит проверку деления без остатка
/// на простые числа меньшие или равные квадратному корню из текущего числа. </remarks>
/// <param name="iN"> Число N, до которого числа раскладываются на простые множители </param>
/// <param name="oResults"> Сюда будут записаны результаты разложения </param>
/// <returns> true - в случае успеха, false - иначе </returns>
bool DecomposeIntoPrimeFactors(const int iN, std::vector<PrimeFactorDecompositionResult>& oResults);
