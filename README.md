# tests
#####Решение некоторых задач:
####1 - Prime-Factors.
Напишите функцию, которая перебирает натуральные числа от 1 до N включительно и раскладывает каждое число на простые множители. Результат можно выводить на экран либо копить в любой структуре данных.

####2 - Vector-Division.
Имеется вектор размера N. Необходимо разделить его на M < N равных частей (то есть чтобы количество элементов в каждой части было одинаковым). Части не должны пересекаться и должны покрывать весь вектор (исключение - допускаются промежутки от начала вектора до начала первой части и от конда последней части до конца вектора, но в этом случае необходимо добиться, чтобы разница в величине этих промежутков была минимальной).
Результатом должны являться индексы начала и конца каждой части (либо вывод на экран, либо сохранение в любую структуру данных).

####3 - Getopt-Long-Only.
Напишите простую реализацию функции для разбора параметров командной строки (getopt).
Полагаем, что нам требуется структура данных, содержащая заранее известный допустимый набор параметров различного типа - строки, целые, числа с плавающей точкой и т. д., а также признак, является ли этот параметр обязательным.
Полагаем, что параметры могут передаваться только в "длинном" формате с обязательным "--" перед именем параметра (то есть "--my-option-name my-option-val", "--my-option-name=my-option-val", "--my-boolean-option").
Параметров может быть несколько.
Функция должна обязательно уметь обрабатывать параметр "--help" (если он указан в списке параметров при вызове функции), выводящий пример использования (необязательные параметры должны выводиться в квадратных скобках).

####4 - Singly-Linked-List.
Имеется простой односвязный список размера N. Необходимо реализовать алгоритм, который перепаковывает список так, чтобы за первым элеметом следовал последний, затем второй, затем предпоследний и т. д. Пример работы алгоритма:
исходный список: 1-2-3-4-5-6-7-8
перепакованный список: 1-8-2-7-3-6-4-5.
Оценить сложность предложенного алгоритма.

####5 - Intersection-of-Two-Tetrahedrons.
Имеется два произвольных тетраэдра, заданных четырьмя точками (вершинами). Необходимо написать функцию поиска объем их пересечения. Результат можно выводить на экран либо сохранить в любой структуре данных.

<i>Все задачи реализованы на языке C++.  
Автор: Комиссаров А.И.</i>
