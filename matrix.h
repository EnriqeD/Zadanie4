#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <memory>
#include <fstream>

class matrix {
private:
    int n_dim;          // Aktualny wymiar (n)
    int n_alloc;        // Rozmiar zaalokowanej pamiêci
    std::unique_ptr<int[]> data; // Inteligentny wskaŸnik

    // Metoda pomocnicza do kontroli zakresu
    int& at(int x, int y) const;

public:
    // Konstruktory i destruktor
    matrix();
    matrix(int n);
    matrix(int n, int* t);
    matrix(const matrix& m);
    matrix(matrix&& m) noexcept;
    ~matrix();

    // Przypisania
    matrix& operator=(const matrix& m);
    matrix& operator=(matrix&& m) noexcept;

    // Zarz¹dzanie pamiêci¹
    matrix& alokuj(int n);

    // Dostêp i modyfikacja
    matrix& wstaw(int x, int y, int wartosc);
    int pokaz(int x, int y) const; // dodano const dla poprawnoœci
    int pobierz_rozmiar() const { return n_dim; }

    // Algorytmy
    matrix& dowroc();
    matrix& losuj();
    matrix& losuj(int x);
    matrix& diagonalna(int* t);
    matrix& diagonalna_k(int k, int* t);
    matrix& kolumna(int x, int* t);
    matrix& wiersz(int y, int* t);
    matrix& przekatna();
    matrix& pod_przekatna();
    matrix& nad_przekatna();
    matrix& szachownica();

    // Operatory arytmetyczne
    matrix operator+(const matrix& m) const;
    matrix operator*(const matrix& m) const;
    matrix operator+(int a) const;
    matrix operator*(int a) const;
    matrix operator-(int a) const;

    friend matrix operator+(int a, const matrix& m);
    friend matrix operator*(int a, const matrix& m);
    friend matrix operator-(int a, const matrix& m);

    matrix& operator++(int);
    matrix& operator--(int);
    matrix& operator+=(int a);
    matrix& operator-=(int a);
    matrix& operator*=(int a);
    matrix& operator()(double d);

    // Operatory porównania
    bool operator==(const matrix& m) const;
    bool operator>(const matrix& m) const;
    bool operator<(const matrix& m) const;

    // Wyjœcie
    // U¿ywamy const matrix&, aby móc drukowaæ obiekty tymczasowe (np. wynik A+B)
    friend std::ostream& operator<<(std::ostream& o, const matrix& m);
};

#endif