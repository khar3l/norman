#include <iostream>
#include <fstream>
#include <vector>
#include "gnuplot-iostream.h"

int main() {
    Gnuplot gp;

    std::ifstream f("way.txt");
    if (!f.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<double> y;
    double value;
    while (f >> value) {
        y.push_back(sqrt(value * 64));
    }
    f.close();

    std::vector<double> x(y.size());
    for (size_t i = 0; i < x.size(); ++i) {
        x[i] = i + 1;
    }

    gp << "set xlabel 'Время работы программы, тиков'\n";
    gp << "set ylabel 'Средний квадрат перемещения частиц, у.е.^2'\n";
    gp << "set title 'График зависимости перемещения частиц от времени'\n";
    gp << "plot '-' with errorbars title 'Среднее перемещение частиц'\n";
    gp.send1d(boost::make_tuple(x, y));

    std::vector<std::pair<double, double>> data_points(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        data_points[i] = std::make_pair(x[i], y[i]);
    }

    std::vector<std::pair<double, double>> fitted_line(2);
    fitted_line[0] = std::make_pair(x.front(), y.front());
    fitted_line[1] = std::make_pair(x.back(), y.back());

    gp << "plot '-' with lines title 'Линейная аппроксимация'\n";
    gp.send1d(fitted_line);

    return 0;
}
