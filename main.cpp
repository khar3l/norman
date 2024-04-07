#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "particleclass.h" // Подключаем заголовочный файл для класса Particle

// Глобальные переменные
const int N = 64; // количество частиц
const double Vmax = 20.0; // максимальная скорость частицы
const double dt = 0.001; // тик
const int Leng = 4; // длина коробки
const double half = Leng / 2.0; // половина длины коробки

// Открываем файлы для записи данных
std::ofstream kint("kin.txt");
std::ofstream pott("pot.txt");
std::ofstream mect("mec.txt");
std::ofstream maxwt("maxw.txt");
std::ofstream wayt("way.txt");
// std::ofstream coord("coord.txt");

// Генерация ячейки
void cell_gen(std::vector<Particle>& particles) {
    int n = 0;
    bool particle_is_even = true;
    int edge = std::ceil(std::cbrt(N));
    double dl = static_cast<double>(Leng) / edge;
    double dl_half = dl / 2.0;
    for (int i = 0; i < edge; ++i) {
        for (int j = 0; j < edge; ++j) {
            for (int k = 0; k < edge; ++k) {
                double c[3] = {dl_half + i * dl, dl_half + j * dl, dl_half + k * dl};
                if (particle_is_even) {
                    double v[3] = {std::rand() / (RAND_MAX / (2.0 * Vmax)) - Vmax,
                                   std::rand() / (RAND_MAX / (2.0 * Vmax)) - Vmax,
                                   std::rand() / (RAND_MAX / (2.0 * Vmax)) - Vmax};
                    if (n == N - 1) {
                        for (int l = 0; l < 3; ++l) {
                            v[l] = 0.0;
                        }
                        particles.push_back(Particle(c, v));
                        return;
                    }
                    particles.push_back(Particle(c, v));
                    particle_is_even = false;
                    ++n;
                } else {
                    double v[3] = {-particles.back().v[0], -particles.back().v[1], -particles.back().v[2]};
                    particles.push_back(Particle(c, v));
                    if (n == N - 1) {
                        return;
                    }
                    particle_is_even = true;
                    ++n;
                }
            }
        }
    }
}

// Обнуление всех ускорений
void null_axel(std::vector<Particle>& particles) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 3; ++j) {
            particles[i].a[j] = 0.0;
        }
    }
}

// Расчет сил взаимодействия между частицами
void axel(Particle& part, Particle& part1) {
    double vect_r[3];
    for (int i = 0; i < 3; ++i) {
        vect_r[i] = part1.c[i] - part.c[i];
        if (vect_r[i] > half) {
            vect_r[i] -= Leng;
        }
        if (vect_r[i] < -half) {
            vect_r[i] += Leng;
        }
    }
    double abs_r = std::sqrt(vect_r[0] * vect_r[0] + vect_r[1] * vect_r[1] + vect_r[2] * vect_r[2]);
    double ac[3];
    for (int i = 0; i < 3; ++i) {
        ac[i] = 24 * (2 * std::pow(abs_r, -14) - std::pow(abs_r, -8)) * vect_r[i];
        part.a[i] -= ac[i];
        part1.a[i] += ac[i];
    }
}

// Расчет ускорений всех частиц
void calc_axel(std::vector<Particle>& particles) {
    null_axel(particles);
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            axel(particles[i], particles[j]);
        }
    }
}

// Первое движение всех частиц
void first_move(std::vector<Particle>& particles) {
    calc_axel(particles);
    for (int i = 0; i < N; ++i) {
        particles[i].first_move();
    }
}

// Движение всех частиц
void move(std::vector<Particle>& particles) {
    calc_axel(particles);
    for (int i = 0; i < N; ++i) {
        particles[i].move();
    }
}

// Вычисление потенциальной энергии взаимодействия двух частиц
double potentwo(const Particle& part, const Particle& part1) {
    double vect_r[3];
    for (int i = 0; i < 3; ++i) {
        vect_r[i] = part1.c[i] - part.c[i];
        if (vect_r[i] > half) {
            vect_r[i] -= Leng;
        }
        if (vect_r[i] < -half) {
            vect_r[i] += Leng;
        }
    }
    double abs_r = std::sqrt(vect_r[0] * vect_r[0] + vect_r[1] * vect_r[1] + vect_r[2] * vect_r[2]);
    return 4 * (std::pow(abs_r, -12) - std::pow(abs_r, -6));
}

// Вычисление суммарного импульса системы
void impulse(const std::vector<Particle>& particles) {
    double summ[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 3; ++j) {
            summ[j] += particles[i].v[j];
        }
    }
    for (int i = 0; i < 3; ++i) {
        kint << summ[i] << " ";
    }
    kint << std::endl;
}

// Вычисление потенциальной энергии взаимодействия всех частиц
double poten_eng(const std::vector<Particle>& particles) {
    double pot = 0.0;
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            pot += potentwo(particles[i], particles[j]);
        }
    }
    pott << pot << std::endl;
    return pot;
}

// Вычисление кинетической энергии системы
double kinetic_eng(const std::vector<Particle>& particles) {
    double kin = 0.0;
    for (int i = 0; i < N; ++i) {
        kin += (particles[i].v[0] * particles[i].v[0] + particles[i].v[1] * particles[i].v[1] + particles[i].v[2] * particles[i].v[2]) / 2.0;
    }
    kint << kin << std::endl;
    return kin;
}

// Вычисление механической энергии системы
void energy(const std::vector<Particle>& particles) {
    double pot = poten_eng(particles);
    double kin = kinetic_eng(particles);
    double summ = pot + kin;
    mect << summ << std::endl;
}

// Расчет скоростей частиц и запись в файл
void maxwellx(const std::vector<Particle>& particles) {
    std::vector<double> list(N);
    for (int i = 0; i < N; ++i) {
        list[i] = particles[i].v[0];
    }
    std::sort(list.begin(), list.end());
    for (int i = 0; i < N; ++i) {
        maxwt << list[i] << std::endl;
    }
}

// Вычисление среднего квадрата перемещения частиц и запись в файл
void average_way(const std::vector<Particle>& particles) {
    double summ = 0.0;
    for (int i = 0; i < N; ++i) {
        double norm_v = particles[i].way[0] * particles[i].way[0] + particles[i].way[1] * particles[i].way[1] + particles[i].way[2] * particles[i].way[2];
        summ += norm_v;
    }
    summ /= N;
    wayt << summ << std::endl;
}

// Время работы программы
void timego(std::vector<Particle>& particles, int tick) {
    std::cout << 0 << '%' << std::endl;
    first_move(particles);
    average_way(particles);
    for (int i = 1; i < tick; ++i) {
        move(particles);
        average_way(particles);
        if (i % (tick / 20) == 0) {
            std::cout << i * 100 / tick << '%' << std::endl;
        }
    }
    maxwellx(particles);
    std::cout << 100 << '%' << std::endl;
}

int main() {
    std::srand(std::time(nullptr)); // инициализируем генератор случайных чисел
    const int t = 50000; // тики
    std::vector<Particle> particles;
    cell_gen(particles);
    timego(particles, t);
    return 0;
}
