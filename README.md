## Источники данных

- [Open-Meteo](https://open-meteo.com/en/docs#latitude=59.94&longitude=30.31&hourly=temperature_2m&forecast_days=16) для прогноза
- [Api-Ninjas](https://api-ninjas.com/api/city) для определения координат по названию города

## Работа программы и ее функции

 - Отображение прогноз погоды на несколько дней вперед (значение по умолчанию задается конфигом)
 - Обновление с некоторой частотой (задается конфигом)
 - Переключение между городами с помощью клавиш "n", "p"
 - Заканчивать работу программы по Esc
 - Увеличение\уменьшение количества дней прогноза при нажатии клавиш "+", "-"

Список городов, частота обновления, количество дней прогноза должны быть определены в конфигe в формате json.

Чтобы запустить программу, нужно ввести в командную строку 2 аргумента: путь к файлу конфигурации и АПИ ключ для работы с Api-Ninjas.

## Отображение

![](/interface/1.jpg)


![](/interface/2.jpg)


![](/interface/3.jpg)


![](/interface/4.jpg)


## Реализация

В качестве библиотеки для HTTP-запросов используется [C++ Requests](https://github.com/libcpr/cpr)

В качестве библиотеки для работы с JSON файлами используется [nlohmann/json](https://github.com/nlohmann/json)

В качестве библиотеки для отображения работы исользуется [ftxui](https://github.com/
ArthurSonzogni/ftxui)
