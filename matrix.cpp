#include "matrix.h"
#include <algorithm>
#include <random>
#include <iomanip>
#include <stdexcept>

// --- METODY PRYWATNE ---
int& matrix::at(int x, int y) const {
    if (x < 0 || x >= n_dim || y < 0 || y >= n_dim) {
        throw std::out_of_range("Indeks poza zakresem macierzy");
    }
    return data[x * n_dim + y];
}

// --- KONSTRUKTORY ---
matrix::matrix() : n_dim(0), n_alloc(0), data(nullptr) {}

matrix::matrix(int n) : n_dim(0), n_alloc(0), data(nullptr) {
    alokuj(n);
}

matrix::matrix(int n, int* t) : matrix(n) {
    if (t && data) {
        std::copy(t, t + (n * n), data.get());
    }
}

matrix::matrix(const matrix& m) : n_dim(m.n_dim), n_alloc(m.n_alloc) {
    if (m.data) {
        data = std::make_unique<int[]>(n_alloc * n_alloc);
        std::copy(m.data.get(), m.data.get() + (n_dim * n_dim), data.get());
    }
}

matrix::matrix(matrix&& m) noexcept : n_dim(m.n_dim), n_alloc(m.n_alloc), data(std::move(m.data)) {
    m.n_dim = 0;
    m.n_alloc = 0;
}

matrix::~matrix() = default;

matrix& matrix::operator=(const matrix& m) {
    if (this == &m) return *this;
    n_dim = m.n_dim;
    n_alloc = m.n_alloc;
    if (m.data) {
        data = std::make_unique<int[]>(n_alloc * n_alloc);
        std::copy(m.data.get(), m.data.get() + (n_dim * n_dim), data.get());
    } else {
        data.reset();
    }
    return *this;
}

matrix& matrix::operator=(matrix&& m) noexcept {
    if (this == &m) return *this;
    n_dim = m.n_dim;
    n_alloc = m.n_alloc;
    data = std::move(m.data);
    m.n_dim = 0;
    m.n_alloc = 0;
    return *this;
}

// --- ZARZ¥DZANIE PAMIÊCI¥ ---
matrix& matrix::alokuj(int n) {
    if (n <= 0) {
        n_dim = 0;
        return *this;
    }
    // Jeœli pamiêæ nie istnieje lub jest za ma³a -> nowa alokacja
    if (!data || n > n_alloc) {
        data = std::make_unique<int[]>(n * n);
        n_alloc = n;
    }
    // Jeœli pamiêæ jest wystarczaj¹ca, tylko zmieniamy n_dim
    n_dim = n;

    // Zerowanie przy ka¿dej alokacji/zmianie rozmiaru dla bezpieczeñstwa
    if (data) {
        std::fill(data.get(), data.get() + (n * n), 0);
    }
    return *this;
}

// --- DOSTÊP I MODYFIKACJA ---
matrix& matrix::wstaw(int x, int y, int wartosc) {
    at(x, y) = wartosc;
    return *this;
}

int matrix::pokaz(int x, int y) const {
    return at(x, y);
}

// --- ALGORYTMY ---
matrix& matrix::dowroc() {
    if (n_dim < 2) return *this;
    std::unique_ptr<int[]> new_data = std::make_unique<int[]>(n_dim * n_dim);
    for (int i = 0; i < n_dim; ++i) {
        for (int j = 0; j < n_dim; ++j) {
            new_data[j * n_dim + i] = at(i, j);
        }
    }
    data = std::move(new_data);
    return *this;
}

matrix& matrix::losuj() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    for (int i = 0; i < n_dim * n_dim; ++i) {
        data[i] = dis(gen);
    }
    return *this;
}

matrix& matrix::losuj(int x) {
    if (x <= 0) return *this;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disVal(0, 9);
    std::uniform_int_distribution<> disPos(0, (n_dim * n_dim) - 1);

    for (int i = 0; i < x; ++i) {
        data[disPos(gen)] = disVal(gen);
    }
    return *this;
}

matrix& matrix::diagonalna(int* t) {
    alokuj(n_dim); // czyœci macierz zerami
    for (int i = 0; i < n_dim; ++i) {
        at(i, i) = t ? t[i] : 0;
    }
    return *this;
}

matrix& matrix::diagonalna_k(int k, int* t) {
    alokuj(n_dim); // czyœci macierz zerami
    for (int i = 0; i < n_dim; ++i) {
        int row = i;
        int col = i + k;
        if (row >= 0 && row < n_dim && col >= 0 && col < n_dim) {
            at(row, col) = t ? t[i] : 0;
        }
    }
    return *this;
}

matrix& matrix::kolumna(int x, int* t) {
    if (x < 0 || x >= n_dim) return *this;
    for (int i = 0; i < n_dim; ++i) {
        at(i, x) = t[i];
    }
    return *this;
}

matrix& matrix::wiersz(int y, int* t) {
    if (y < 0 || y >= n_dim) return *this;
    for (int i = 0; i < n_dim; ++i) {
        at(y, i) = t[i];
    }
    return *this;
}

matrix& matrix::przekatna() {
    for (int i = 0; i < n_dim; ++i) {
        for (int j = 0; j < n_dim; ++j) {
            at(i, j) = (i == j) ? 1 : 0;
        }
    }
    return *this;
}

matrix& matrix::pod_przekatna() {
    for (int i = 0; i < n_dim; ++i) {
        for (int j = 0; j < n_dim; ++j) {
            at(i, j) = (i > j) ? 1 : 0;
        }
    }
    return *this;
}

matrix& matrix::nad_przekatna() {
    for (int i = 0; i < n_dim; ++i) {
        for (int j = 0; j < n_dim; ++j) {
            at(i, j) = (j > i) ? 1 : 0;
        }
    }
    return *this;
}

matrix& matrix::szachownica() {
    for (int i = 0; i < n_dim; ++i) {
        for (int j = 0; j < n_dim; ++j) {
            at(i, j) = (i + j) % 2;
        }
    }
    return *this;
}

// --- OPERATORY ARYTMETYCZNE ---

matrix matrix::operator+(const matrix& m) const {
    matrix result(n_dim);
    int limit = (n_dim < m.n_dim) ? n_dim : m.n_dim;
    for (int i = 0; i < limit; ++i) {
        for (int j = 0; j < limit; ++j) {
            result.at(i, j) = this->at(i, j) + m.at(i, j);
        }
    }
    return result;
}

matrix matrix::operator*(const matrix& m) const {
    if (n_dim != m.n_dim) return matrix(0);
    matrix result(n_dim);
    for (int i = 0; i < n_dim; ++i) {
        for (int j = 0; j < n_dim; ++j) {
            int sum = 0;
            for (int k = 0; k < n_dim; ++k) {
                sum += this->at(i, k) * m.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }
    return result;
}

matrix matrix::operator+(int a) const {
    matrix result(*this);
    return result += a;
}

matrix matrix::operator*(int a) const {
    matrix result(*this);
    return result *= a;
}

matrix matrix::operator-(int a) const {
    matrix result(*this);
    return result -= a;
}

matrix operator+(int a, const matrix& m) {
    matrix result(m);
    return result += a;
}

matrix operator*(int a, const matrix& m) {
    matrix result(m);
    return result *= a;
}

matrix operator-(int a, const matrix& m) {
    matrix result(m.n_dim);
    for (int i = 0; i < m.n_dim * m.n_dim; ++i) {
        result.data[i] = a - m.data[i];
    }
    return result;
}

matrix& matrix::operator++(int) {
    *this += 1;
    return *this;
}

matrix& matrix::operator--(int) {
    *this -= 1;
    return *this;
}

matrix& matrix::operator+=(int a) {
    for (int i = 0; i < n_dim * n_dim; ++i) {
        data[i] += a;
    }
    return *this;
}

matrix& matrix::operator-=(int a) {
    for (int i = 0; i < n_dim * n_dim; ++i) {
        data[i] -= a;
    }
    return *this;
}

matrix& matrix::operator*=(int a) {
    for (int i = 0; i < n_dim * n_dim; ++i) {
        data[i] *= a;
    }
    return *this;
}

matrix& matrix::operator()(double d) {
    int val = static_cast<int>(d);
    *this += val;
    return *this;
}

// --- OPERATORY PORÓWNANIA ---

bool matrix::operator==(const matrix& m) const {
    if (n_dim != m.n_dim) return false;
    for (int i = 0; i < n_dim * n_dim; ++i) {
        if (data[i] != m.data[i]) return false;
    }
    return true;
}

bool matrix::operator>(const matrix& m) const {
    if (n_dim != m.n_dim) return false;
    for (int i = 0; i < n_dim * n_dim; ++i) {
        if (!(data[i] > m.data[i])) return false;
    }
    return true;
}

bool matrix::operator<(const matrix& m) const {
    if (n_dim != m.n_dim) return false;
    for (int i = 0; i < n_dim * n_dim; ++i) {
        if (!(data[i] < m.data[i])) return false;
    }
    return true;
}

// --- WYJŒCIE ---
std::ostream& operator<<(std::ostream& o, const matrix& m) {
    for (int i = 0; i < m.n_dim; ++i) {
        o << "| ";
        for (int j = 0; j < m.n_dim; ++j) {
            o << std::setw(3) << m.at(i, j) << " ";
        }
        o << "|\n";
    }
    return o;
}