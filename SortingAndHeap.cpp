#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

int maksimumVektora(const std::vector<int>& a) {
    if (a.empty()) return 0;
    int maksimum = a[0];
    for (int i = 1; i < (int)a.size(); i++) {
        if (a[i] > maksimum) maksimum = a[i];
    }
    return maksimum;
}

void countingPoCifri(std::vector<int>& a, int exp) {
    int n = (int)a.size();
    if (n == 0) return;

    std::vector<int> izlaz(n);
    int brojac[10] = {0};

    for (int i = 0; i < n; i++) {
        int cifra = (a[i] / exp) % 10;
        brojac[cifra]++;
    }

    for (int i = 1; i < 10; i++) brojac[i] += brojac[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        int cifra = (a[i] / exp) % 10;
        izlaz[brojac[cifra] - 1] = a[i];
        brojac[cifra]--;
    }

    a = izlaz;
}

void radixSort(std::vector<int>& a) {
    if (a.empty()) return;

    int maxVrijednost = maksimumVektora(a);
    for (int exp = 1; maxVrijednost / exp > 0; exp *= 10) {
        countingPoCifri(a, exp);
    }
}

static int roditelj(int i) { return (i - 1) / 2; }
static int lijevoDijete(int i) { return 2 * i + 1; }
static int desnoDijete(int i) { return 2 * i + 2; }

static void popraviDolje(std::vector<int>& a, int indeks, int velicina) {
    while (true) {
        int lijevo = lijevoDijete(indeks);
        if (lijevo >= velicina) break;

        int desno = desnoDijete(indeks);
        int veci = lijevo;

        if (desno < velicina && a[desno] > a[lijevo]) veci = desno;
        if (a[indeks] >= a[veci]) break;

        std::swap(a[indeks], a[veci]);
        indeks = veci;
    }
}

void popraviGore(std::vector<int>& a, int indeks) {
    while (indeks > 0) {
        int r = roditelj(indeks);
        if (a[indeks] <= a[r]) break;
        std::swap(a[indeks], a[r]);
        indeks = r;
    }
}

void stvoriGomilu(std::vector<int>& a) {
    int n = (int)a.size();
    if (n <= 1) return;

    for (int i = n / 2 - 1; i >= 0; i--) {
        popraviDolje(a, i, n);
        if (i == 0) break;
    }
}

void umetniUGomilu(std::vector<int>& a, int umetnuti, int& velicina) {
    if (velicina < 0) velicina = 0;
    if (velicina > (int)a.size()) velicina = (int)a.size();

    if (velicina == (int)a.size()) a.push_back(umetnuti);
    else a[velicina] = umetnuti;

    velicina++;
    popraviGore(a, velicina - 1);
}

int izbaciPrvi(std::vector<int>& a, int& velicina) {
    if (velicina <= 0) throw std::domain_error("nema prvog elementa");

    int korijen = a[0];
    velicina--;
    if (velicina > 0) {
        a[0] = a[velicina];
        popraviDolje(a, 0, velicina);
    }
    return korijen;
}

void gomilaSort(std::vector<int>& a) {
    stvoriGomilu(a);
    int n = (int)a.size();

    for (int i = n - 1; i > 0; i--) {
        std::swap(a[0], a[i]);
        popraviDolje(a, 0, i);
    }
}

int main() {
    std::vector<int> a = {170, 45, 75, 90, 802, 24, 2, 66};
    gomilaSort(a);
    for (int x : a) std::cout << x << " ";
    std::cout << std::endl;
    std::vector<int> b = {12, 3, 999, 41, 7, 0, 18, 5};
    radixSort(b);
    for (int x : b) std::cout << x << " ";
    std::cout << std::endl;
    std::vector<int> h = {10, 4, 8, 1, 3};
    stvoriGomilu(h);
    int velicina = (int)h.size();
    umetniUGomilu(h, 25, velicina);
    std::cout << izbaciPrvi(h, velicina) << std::endl;
    return 0;
}

