#include <cmath>
#include <iostream>
#include <vector>

double dt = 0.001; // тик
int Leng = 4;       // длина коробки
double half = Leng / 2.0; // половина длины коробки

class Particle {
public:
    std::vector<double> coord; // координата
    std::vector<double> v;     // скорость
    std::vector<double> a;     // ускорение
    std::vector<double> lc;    // последняя координата
    std::vector<double> way;   // перемещение частицы с начала времени

    Particle(std::vector<double> coord_init = {0.0, 0.0, 0.0}, std::vector<double> v_init = {0.0, 0.0, 0.0},
             std::vector<double> a_init = {0.0, 0.0, 0.0}, std::vector<double> lc_init = {0.0, 0.0, 0.0},
             std::vector<double> way_init = {0.0, 0.0, 0.0}) : coord(coord_init), v(v_init), a(a_init), lc(lc_init), way(way_init) {}

    static std::vector<double> vec_to_virtual_copy(const std::vector<double>& partc, const std::vector<double>& part1c) {
        std::vector<double> vect_r(3);
        for (std::vector<double>::size_type i = 0; i < 3; ++i) {
            vect_r[i] = part1c[i] - partc[i];
            if (vect_r[i] > half)
                vect_r[i] -= Leng;
            if (vect_r[i] < -half)
                vect_r[i] += Leng;
        }
        return vect_r;
    }

    static void to_border(std::vector<double>& c) {
        for (std::vector<double>::size_type i = 0; i < 3; ++i) {
            while (c[i] >= Leng || c[i] < 0) {
                c[i] = std::fmod(c[i], Leng); // Заменяем fmod на std::fmod
            }
        }
    }

    void first_move() {
        lc = coord;
        std::vector<double> delta_r(3);
        for (std::vector<double>::size_type i = 0; i < 3; ++i) {
            delta_r[i] = dt * v[i] + 0.5 * a[i] * dt * dt;
            way[i] += delta_r[i];
            coord[i] += delta_r[i];
        }
        to_border(coord);
        for (std::vector<double>::size_type i = 0; i < 3; ++i) {
            v[i] += dt * a[i];
        }
    }

    void move() {
        std::vector<double> delta_r = vec_to_virtual_copy(lc, coord);
        for (std::vector<double>::size_type i = 0; i < 3; ++i) {
            delta_r[i] += a[i] * dt * dt;
            lc[i] = coord[i];
            way[i] += delta_r[i];
            coord[i] += delta_r[i];
        }
        to_border(coord);
        for (std::vector<double>::size_type i = 0; i < 3; ++i) {
            v[i] += a[i] * dt;
        }
    }
};

int main() {
    std::vector<double> coord_init = {0.0, 0.0, 0.0};
    std::vector<double> v_init = {0.0, 0.0, 0.0};
    std::vector<double> a_init = {0.0, 0.0, 0.0};
    std::vector<double> lc_init = {0.0, 0.0, 0.0};
    std::vector<double> way_init = {0.0, 0.0, 0.0};

    Particle particle(coord_init, v_init, a_init, lc_init, way_init);
    particle.first_move();
    particle.move();

    return 0;
}
