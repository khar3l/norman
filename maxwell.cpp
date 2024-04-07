#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "gnuplot-iostream.h"

int main() {
    Gnuplot gp;

    std::ifstream file("maxw.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::vector<double> v_x;
    double value;
    while (file >> value) {
        v_x.push_back(value);
    }
    file.close();

    int N = v_x.size();
    double T = 0.0;
    for (double val : v_x) {
        T += val * val;
    }
    T /= N;

    gp << "set title 'Распределение проекции скоростей частиц на ось X'\n";
    gp << "set xlabel 'Скорость'\n";
    gp << "set ylabel 'Частота'\n";
    gp << "bin_width = " << T << "\n";
    gp << "bin_number = " << N / 3 << "\n";
    gp << "bin(x, width) = width*floor(x/width)\n";
    gp << "plot '-' using (bin($1, bin_width)):(1.0) smooth freq with boxes title 'Histogram'\n";

    for (double val : v_x) {
        gp << val << "\n";
    }
    gp << "e\n";
    
    std::cout << "T/N: " << T / N << std::endl;

    return 0;
}
