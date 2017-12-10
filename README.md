# cinkaboutit
The Cursach
8. Реализовать АТД «Файловая структура данных» в соответствии с вариантом.

Операции АТД:

Insert – вставка записи в файл,

Delete – удаление записи из файла,

Search - поиск записи в файле.

1. Элементом данных, хранящихся в файле, является запись, имеющая уникальное ключевое значение.

2. Запись в файле представлена индексом, т.е. парой (k,p), где k – ключевое значение, p – файловый указатель на начало записи в файле.

3. Для файла данных поддерживается файловая структура индексов, указанная в варианте задания.

4. Файл индексов имеет страничную структуру. Страницы содержат индексы записей и имеют фиксированный размер.

5. Чтение и запись в файл индексов ведется постранично.

6. Тестирование файловой структуры ведется для различных значений параметров:

N – число записей в файле данных, N = 10^3, 10^4, 10^5, 10^6,

M – число индексов на странице файла индексов, M = 10, 100, 1000.

7. Число обращений к блокам файла индексов в процессе выполнения операций должно соответствовать:

- для хешированного файла - N/(M*K), где K – число сегментов хеш – таблицы,

- для индексированного файла – 2 + log(N/M),

- для B- дерева файла – 2 + logt (N/M), где t – степень В – дерева.
