#include "PrimeFactorDecomposition.h"

#include <sstream>

// PRIMES

std::vector<const int> Primes::_primes;

const std::vector<const int>& Primes::Get(const int iUpTo)
{
    if (_primes.empty())
    {
        _primes.push_back(2);
        _primes.push_back(3);
    }

    if (iUpTo <= _primes.back())
        return _primes;

    auto maxPrime = _primes.back();
    auto currentNum = maxPrime;
    
    while (maxPrime < iUpTo)
    {
        currentNum += 2;    // Шаг равен двум, т.к. проверяем только нечётные числа (чётные всегда делятся на 2)
        bool isPrime = true;
    
        for (auto primeNumIter = _primes.cbegin(); *primeNumIter <= sqrt(currentNum); ++primeNumIter)
        {
            if ((currentNum % *primeNumIter) == 0)
            {	
                isPrime = false;
                break;
            }
        }
	 	
        if (isPrime)
        {
            _primes.push_back(currentNum);
            maxPrime = currentNum;
        }
    }

    return _primes;
}

// PRIME FACTOR DECOMPOSITION RESULT

std::string PrimeFactorDecompositionResult::GetAsString() const
{
    auto getString =
        [](int iNumber)
    {
        std::ostringstream strs;
        strs << iNumber;
        return strs.str();
    };

    std::string resString;
    if (IsCorrect())
    {
        resString = getString(_number) + " =";
        for (const auto& factor : _factors)
        {
            resString += " " + getString(factor) + " *";
        }
        resString.pop_back();
    }
    else
    {
        resString = "Data is incorrect: " + getString(_number);
    }

    return resString;
}

bool PrimeFactorDecompositionResult::IsCorrect() const
{
    if (_factors.size() == 0)
        return false;

    int res = 1;
    for (const auto& factor : _factors)
    {
        res *= factor;
    }

    return res == _number;
}

// DECOMPOSE INTO PRIME FACTORS

bool DecomposeIntoPrimeFactors(const int iN, std::vector<PrimeFactorDecompositionResult>& oResults)
{
    oResults.clear();

    if (iN < 1)
        return false;

    oResults.reserve(iN);

	// Ищем простые числа от 1 до квадратного корня из iN
    auto primes = Primes::Get(static_cast<int>(sqrt(iN) + 0.5));

    // Раскладываем на простые множители числа от 1 до iN
    for (int currentNum = 1; currentNum <= iN; currentNum++)
    {
        oResults.push_back(PrimeFactorDecompositionResult(currentNum));
        auto remainder = currentNum;

        for (auto primeNumIter = primes.cbegin(); *primeNumIter <= sqrt(currentNum); )
        {
            if (remainder % *primeNumIter == 0)
            {
                oResults.back().AddFactor(*primeNumIter);
                remainder /= *primeNumIter;
                primeNumIter = primes.cbegin();
            }
            else
                ++primeNumIter;
        }

        if (remainder == currentNum || sqrt(currentNum) < remainder) 
            oResults.back().AddFactor(remainder);
    }

    return true;
}
