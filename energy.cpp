#include <iostream>
#include <fstream>
#include <vector>
#include "gnuplot-iostream.h" // Подключаем gnuplot-iostream

int main() {
    // Загрузка данных из файлов
    std::ifstream f_mec("mec.txt");
    std::ifstream f_pot("pot.txt");
    std::ifstream f_kin("kin.txt");

    std::vector<double> x, ymec, ypot, ykin;
    double value;
    int index = 0;

    // Считываем данные из файлов и сохраняем в вектора
    while (f_mec >> value) {
        ymec.push_back(value);
        x.push_back(index++);
    }
    index = 0;
    while (f_pot >> value) {
        ypot.push_back(value);
        index++;
    }
    index = 0;
    while (f_kin >> value) {
        ykin.push_back(value);
        index++;
    }

    // Создание объекта Gnuplot
    Gnuplot gp;
    // Устанавливаем названия осей и заголовок
    gp << "set xlabel 'Время работы программы, тиков'\n";
    gp << "set ylabel 'Энергия, у.е.'\n";
    gp << "set title 'График зависимости разных видов энергии от времени'\n";

    // Построение графиков
    gp << "plot '-' with lines title 'Полная механическая энергия', "
          "'-' with lines title 'Кинетическая энергия частиц', "
          "'-' with lines title 'Потенциальная энергия частиц'\n";

    gp.send1d(std::make_tuple(x, ymec)); // Полная механическая энергия
    gp.send1d(std::make_tuple(x, ykin)); // Кинетическая энергия частиц
    gp.send1d(std::make_tuple(x, ypot)); // Потенциальная энергия частиц

    return 0;
}
