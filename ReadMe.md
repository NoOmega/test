# Tests
<i> Решения некоторых задач:</i>

#### <a href="https://github.com/NoOmega/tests/tree/master/FormulaSolver">1 - Formula Solver</a>
Есть формула с цифрами, операциями +-*/ и скобками в формате строки. Нужно написать программу, которая ее вычисляет. Формула может быть большой. Хочется, чтобы программу можно было легко дорабатывать, вводя функции, параметры и т.д.

#### <a href="https://github.com/NoOmega/tests/tree/master/PrimeFactors">2 - Prime Factors</a>
Напишите функцию, которая перебирает натуральные числа от 1 до N включительно и раскладывает каждое число на простые множители. Результат можно выводить на экран либо копить в любой структуре данных.

#### <a href="https://github.com/NoOmega/tests/tree/master/VectorDivision">3 - Vector Division</a>
Имеется вектор размера N. Необходимо разделить его на M < N равных частей (то есть чтобы количество элементов в каждой части было одинаковым). Части не должны пересекаться и должны покрывать весь вектор (исключение - допускаются промежутки от начала вектора до начала первой части и от конда последней части до конца вектора, но в этом случае необходимо добиться, чтобы разница в величине этих промежутков была минимальной).
Результатом должны являться индексы начала и конца каждой части (либо вывод на экран, либо сохранение в любую структуру данных).

#### <a href="https://github.com/NoOmega/tests/tree/master/GetoptLongOnly">4 - Getopt Long Only</a>
Напишите простую реализацию функции для разбора параметров командной строки (getopt).
Полагаем, что нам требуется структура данных, содержащая заранее известный допустимый набор параметров различного типа - строки, целые, числа с плавающей точкой и т. д., а также признак, является ли этот параметр обязательным.
Полагаем, что параметры могут передаваться только в "длинном" формате с обязательным "--" перед именем параметра (то есть "--my-option-name my-option-val", "--my-option-name=my-option-val", "--my-boolean-option").
Параметров может быть несколько.
Функция должна обязательно уметь обрабатывать параметр "--help" (если он указан в списке параметров при вызове функции), выводящий пример использования (необязательные параметры должны выводиться в квадратных скобках).

#### <a href="https://github.com/NoOmega/tests/tree/master/SinglyLinkedList">5 - Singly Linked List</a>
Имеется простой односвязный список размера N. Необходимо реализовать алгоритм, который перепаковывает список так, чтобы за первым элеметом следовал последний, затем второй, затем предпоследний и т. д. Пример работы алгоритма:
исходный список: 1-2-3-4-5-6-7-8
перепакованный список: 1-8-2-7-3-6-4-5.
Оценить сложность предложенного алгоритма.

#### <a href="https://github.com/NoOmega/tests/tree/master/IntersectionOfTwoTetrahedrons">6 - Intersection of Two Tetrahedrons</a>
Имеются два произвольных тетраэдра, заданные четырьмя точками (вершинами). Необходимо написать функцию поиска объема их пересечения. Результат можно выводить на экран либо сохранить в любой структуре данных.

#### <a href="https://github.com/NoOmega/tests/tree/master/Figures">7 - Figures</a>
Необходимо привести пример программы или библиотеки на C++, которая выводит на экран различные геометрические фигуры: круг, квадрат, треугольник, прямоугольник и т.п. Глубина проработки примера (количество фигур, параметры, методы) на ваше усмотрение. В задании вы должны продемонстрировать умение использовать ООП.

<i><b>Все задачи реализованы на языке C++.  
Автор: Комиссаров А.И.</b></i>