#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "matrix.h"

using namespace std;

// Generuje plik z danymi (sekwencja liczb), aby wczytać je do macierzy
void przygotuj_dane(const string& nazwa, int n) {
    ofstream plik(nazwa);
    if (!plik.is_open()) {
        cerr << "Blad: nie mozna utworzyc pliku: " << nazwa << endl;
        return;
    }
    // Zapisujemy sekwencję 0, 1, 2... dla całej macierzy
    for (int i = 0; i < n * n; ++i) {
        plik << i << ' ';
    }
    plik.close();
}

// Wypisuje tylko fragment macierzy (lewy górny róg), aby nie zaśmiecać konsoli przy N=30
void wypisz_fragment(const matrix& m, const string& opis) {
    int rozmiar = m.pobierz_rozmiar();
    int limit = (rozmiar < 8) ? rozmiar : 8; // Pokazujemy max 8x8

    cout << "\n--- " << opis << " (Fragment " << limit << "x" << limit << ") ---" << endl;
    for (int i = 0; i < limit; ++i) {
        cout << "| ";
        for (int j = 0; j < limit; ++j) {
            // Używamy metody pokaz()
            cout.width(3);
            cout << m.pokaz(i, j) << " ";
        }
        cout << "|" << endl;
    }
    if (rozmiar > limit) cout << "... (reszta macierzy ukryta)" << endl;
}

int main() {
    const int N = 30; // Wymagane N >= 30
    const string PLIK = "dane.txt";

    cout << "=== ROZPOCZYNAM TESTY (N=" << N << ") ===" << endl;

    // 1. Przygotowanie danych
    przygotuj_dane(PLIK, N);
    vector<int> bufor;
    bufor.resize(static_cast<size_t>(N) * static_cast<size_t>(N));
    ifstream plik_we(PLIK);
    if (!plik_we.is_open()) {
        cerr << "Blad: nie mozna otworzyc pliku: " << PLIK << endl;
        return 1;
    }
    for (int i = 0; i < N * N; ++i) {
        if (!(plik_we >> bufor[i])) {
            cerr << "Blad: nie udalo sie wczytac danych (pozycja " << i << ")" << endl;
            return 1;
        }
    }
    plik_we.close();

    // 1. Konstruktor domyślny + alokuj
    matrix m1;
    m1.alokuj(N);
    wypisz_fragment(m1, "1. Konstruktor domyslny + alokuj(N) [powinny byc 0]");

    // 2. Konstruktor (int)
    matrix m2(N);
    wypisz_fragment(m2, "2. Konstruktor(int) [powinny byc 0]");

    // 3. Konstruktor (int, int*) - wczytanie z tabeli
    matrix m3(N, bufor.data());
    wypisz_fragment(m3, "3. Konstruktor(int, int*) [sekwencja liczb]");

    // 4. Konstruktor kopiujący
    matrix m_copy(m3);
    wypisz_fragment(m_copy, "4. Konstruktor kopiujacy [kopia powyzszego]");

    // 5. wstaw() i pokaz()
    m1.wstaw(0, 0, 999);
    m1.wstaw(1, 1, 777);
    cout << "\n--- 5. Test wstaw() i pokaz() ---" << endl;
    cout << "Wartosc pod (0,0): " << m1.pokaz(0, 0) << " (oczekiwane 999)" << endl;
    cout << "Wartosc pod (1,1): " << m1.pokaz(1, 1) << " (oczekiwane 777)" << endl;

    // 6. Realokacja (zmniejszenie/zwiększenie) - test logiczny metody alokuj
    m1.alokuj(N + 5); // Zwiększenie
    cout << "\n--- 6. Test ponownej alokacji ---" << endl;
    cout << "Nowy rozmiar m1: " << m1.pobierz_rozmiar() << " (oczekiwane 35)" << endl;
    m1.alokuj(N); // Powrót do N

    // 7. losuj()
    m1.losuj();
    wypisz_fragment(m1, "7. losuj() [wszystkie elementy losowe]");

    // 8. losuj(x) - losuje x elementów, reszta bez zmian
    matrix m_czysta(N); // same zera
    m_czysta.losuj(10); // wstawia 10 losowych cyfr w losowe miejsca
    wypisz_fragment(m_czysta, "8. losuj(10) [tylko 10 elementow niezerowych]");

    // 9. szachownica()
    m2.szachownica();
    wypisz_fragment(m2, "9. szachownica() [wzor 0 1 0 1...]");

    // 10. przekatna()
    m2.przekatna();
    wypisz_fragment(m2, "10. przekatna() [1 na glownej osi]");

    // 11. pod_przekatna()
    m2.pod_przekatna();
    wypisz_fragment(m2, "11. pod_przekatna() [1 ponizej osi]");

    // 12. nad_przekatna()
    m2.nad_przekatna();
    wypisz_fragment(m2, "12. nad_przekatna() [1 powyzej osi]");

    // 13. diagonalna(int* t)
    // Tworzymy tablicę wartości dla przekątnej (10, 20, 30...)
    vector<int> t_diag(N);
    for (int i = 0; i < N; ++i) t_diag[i] = (i + 1) * 10;

    m2.diagonalna(t_diag.data());
    wypisz_fragment(m2, "13. diagonalna(t) [10, 20, 30...]");

    // 14. diagonalna_k(k, t)
    m2.diagonalna_k(2, t_diag.data());
    wypisz_fragment(m2, "14. diagonalna_k(2) [przesunieta w gore o 2]");

    m2.diagonalna_k(-3, t_diag.data());
    wypisz_fragment(m2, "14b. diagonalna_k(-3) [przesunieta w dol o 3]");

    // 15. kolumna(x, t)
    m2.alokuj(N); // reset
    m2.kolumna(1, t_diag.data()); // Wstawiamy do kolumny o indeksie 1 (drugiej)
    wypisz_fragment(m2, "15. kolumna(1, t) [druga kolumna wypelniona]");

    // 16. wiersz(y, t)
    m2.wiersz(2, t_diag.data()); // Wstawiamy do wiersza o indeksie 2 (trzeci)
    wypisz_fragment(m2, "16. wiersz(2, t) [trzeci wiersz wypelniony]");

