#include <iostream>
#include <fstream>
#include <cmath>

int main() {
    std::ifstream file("maxw.txt");
    double sum = 0.0;
    double value;

    while (file >> value) {
        sum += std::abs(value);
    }

    file.close();

    int N = 0;
    file.open("maxw.txt");

    while (file >> value) {
        ++N;
    }

    file.close();

    double V = sum * sqrt(3) / N;

    std::cout << V << std::endl;

    return 0;
}
