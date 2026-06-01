#include <iostream>
#include <string>
#include <stdexcept>

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    virtual ~Mapa() {}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti& operator[](const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti operator[](const TipKljuca& kljuc) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        int balans;

        Cvor(const TipKljuca& k, const TipVrijednosti& v, Cvor* r)
            : kljuc(k), vrijednost(v), lijevo(nullptr), desno(nullptr), roditelj(r), balans(1) {}
    };

    Cvor* korijen;
    int velicina;

    int visina(Cvor* p) const { return p ? p->balans : 0; }

    int balansFaktor(Cvor* p) const {
        if (!p) return 0;
        return visina(p->lijevo) - visina(p->desno);
    }

    void azurirajVisinu(Cvor* p) {
        if (p) {
            int hl = visina(p->lijevo);
            int hd = visina(p->desno);
            p->balans = (hl > hd ? hl : hd) + 1;
        }
    }

    Cvor* desnaRotacija(Cvor* y) {
        Cvor* x = y->lijevo;
        y->lijevo = x->desno;
        if (x->desno) x->desno->roditelj = y;

        x->roditelj = y->roditelj;
        if (y->roditelj) {
            if (y == y->roditelj->lijevo) y->roditelj->lijevo = x;
            else y->roditelj->desno = x;
        }

        x->desno = y;
        y->roditelj = x;

        azurirajVisinu(y);
        azurirajVisinu(x);
        return x;
    }

    Cvor* lijevaRotacija(Cvor* x) {
        Cvor* y = x->desno;
        x->desno = y->lijevo;
        if (y->lijevo) y->lijevo->roditelj = x;

        y->roditelj = x->roditelj;
        if (x->roditelj) {
            if (x == x->roditelj->lijevo) x->roditelj->lijevo = y;
            else x->roditelj->desno = y;
        }

        y->lijevo = x;
        x->roditelj = y;

        azurirajVisinu(x);
        azurirajVisinu(y);
        return y;
    }

    Cvor* balansiraj(Cvor* p) {
        if (!p) return nullptr;
        azurirajVisinu(p);
        int b = balansFaktor(p);

        if (b > 1) {
            if (balansFaktor(p->lijevo) < 0) {
                p->lijevo = lijevaRotacija(p->lijevo);
            }
            return desnaRotacija(p);
        }
        if (b < -1) {
            if (balansFaktor(p->desno) > 0) {
                p->desno = desnaRotacija(p->desno);
            }
            return lijevaRotacija(p);
        }
        return p;
    }


    Cvor* umetni(Cvor*& p, const TipKljuca& k, Cvor* rod, Cvor*& novi, bool& dodano) {
        if (!p) {
            novi = p = new Cvor(k, TipVrijednosti(), rod);
            dodano = true;
            return p;
        }
        if (k < p->kljuc) p->lijevo = umetni(p->lijevo, k, p, novi, dodano);
        else if (k > p->kljuc) p->desno = umetni(p->desno, k, p, novi, dodano);
        else {
            novi = p;
            dodano = false;
            return p;
        }
        return balansiraj(p);
    }

    Cvor* nadjiMin(Cvor* p) {
        while (p && p->lijevo) p = p->lijevo;
        return p;
    }

    Cvor* obrisiRek(Cvor* p, const TipKljuca& k, bool& obrisano) {
        if (!p) return nullptr;
        if (k < p->kljuc) p->lijevo = obrisiRek(p->lijevo, k, obrisano);
        else if (k > p->kljuc) p->desno = obrisiRek(p->desno, k, obrisano);
        else {
            obrisano = true;
            if (!p->lijevo || !p->desno) {
                Cvor* temp = p->lijevo ? p->lijevo : p->desno;
                if (temp) temp->roditelj = p->roditelj;
                delete p;
                return temp;
            } else {
                Cvor* temp = nadjiMin(p->desno);
                p->kljuc = temp->kljuc;
                p->vrijednost = temp->vrijednost;
                p->desno = obrisiRek(p->desno, temp->kljuc, obrisano);
            }
        }
        return balansiraj(p);
    }

    void preorder(Cvor* p, bool& prvi) const {
        if (!p) return;
        if (!prvi) std::cout << ",";
        std::cout << p->kljuc;
        prvi = false;
        preorder(p->lijevo, prvi);
        preorder(p->desno, prvi);
    }

    void obrisiSve(Cvor* p) {
        if (!p) return;
        obrisiSve(p->lijevo);
        obrisiSve(p->desno);
        delete p;
    }

public:
    AVLStabloMapa() : korijen(nullptr), velicina(0) {}
    ~AVLStabloMapa() { obrisi(); }

    AVLStabloMapa& operator=(const AVLStabloMapa& m) {
        if (this != &m) {
            obrisi();
        }
        return *this;
    }

    int brojElemenata() const override { return velicina; }

    void obrisi() override {
        obrisiSve(korijen);
        korijen = nullptr;
        velicina = 0;
    }

    void Preorder() const {
        bool prvi = true;
        preorder(korijen, prvi);
        std::cout << std::endl;
    }

    void obrisi(const TipKljuca& kljuc) override {
        bool obrisano = false;
        korijen = obrisiRek(korijen, kljuc, obrisano);
        if (obrisano) velicina--;
    }

    TipVrijednosti& operator[](const TipKljuca& kljuc) override {
        Cvor* novi = nullptr;
        bool dodano = false;
        korijen = umetni(korijen, kljuc, nullptr, novi, dodano);
        if (dodano) {
            velicina++;
        }
        return novi->vrijednost;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const override {
        Cvor* p = korijen;
        while (p) {
            if (kljuc == p->kljuc) return p->vrijednost;
            p = (kljuc < p->kljuc) ? p->lijevo : p->desno;
        }
        return TipVrijednosti();
    }
};

int main() {
    std::cout << "Uzastopno umetanje" << std::endl;
    AVLStabloMapa<int, int> m1;
    for(int i=1; i<=10; i++) m1[i] = i;
    m1.Preorder();

    std::cout << "Obrnuto umetanje " << std::endl;
    AVLStabloMapa<int, int> m2;
    for(int i=10; i>=1; i--) m2[i] = i;
    m2.Preorder();

    std::cout << "Nasumicni redoslijed" << std::endl;
    AVLStabloMapa<int, int> m3;
    int n3[] = {15, 10, 20, 5, 12, 17, 25, 3, 7, 11};
    for(int x : n3) m3[x] = x;
    m3.Preorder();

    std::cout << "S balansiranjem nakon brisanja" << std::endl;
    AVLStabloMapa<int, int> m4;
    for(int i=1; i<=10; i++) m4[i] = i;
    m4.obrisi(4);
    m4.Preorder();

    std::cout << "Cik-cak struktura" << std::endl;
    AVLStabloMapa<int, int> m5;
    int n5[] = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27};
    for(int x : n5) m5[x] = x;
    m5.Preorder();

    return 0;
}
