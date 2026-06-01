#include <iostream>
#include <stdexcept>
#include <vector>

template <typename Tip>
class Iterator;

template <typename Tip>
class Lista {
public:
    friend class Iterator<Tip>;
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual const Tip& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip& operator[](int i) = 0;
    virtual const Tip& operator[](int i) const = 0;
    virtual Iterator<Tip> begin() const = 0;
    virtual Iterator<Tip> end() const = 0;
};

template <typename Tip>
struct Cvor {
    Tip element;
    Cvor<Tip>* prethodni;
    Cvor<Tip>* sljedeci;
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip> {
    Cvor<Tip> *poc;
    Cvor<Tip> *tekuci;
    Cvor<Tip> *kr;
    int duzina;
public:
    friend class Iterator<Tip>;
    DvostrukaLista() : poc(nullptr), tekuci(nullptr), kr(nullptr), duzina(0) {}

    ~DvostrukaLista() {
        Cvor<Tip> *it = poc;
        while(it!=nullptr) {
            Cvor<Tip> *brisanje = it;
            it=it->sljedeci;
            delete brisanje;
        }
        poc=nullptr;
        kr=nullptr;
        tekuci=nullptr;
        duzina=0;
    }

    int brojElemenata() const override {
        return duzina;
    }

    Tip& trenutni() override {
        if (duzina==0 || tekuci==nullptr)
            throw std::logic_error("Prazna lista");
        return tekuci->element;
    }

    const Tip& trenutni() const override {
        if (duzina==0 || tekuci==nullptr)
            throw std::logic_error("Prazna lista");
        return tekuci->element;
    }

    bool prethodni() override {
        if (duzina==0 || tekuci==nullptr)
            throw std::logic_error("Prazna lista");
        if(tekuci->prethodni==nullptr) return false;
        tekuci=tekuci->prethodni;
        return true;
    }

    bool sljedeci() override {
        if (duzina==0 || tekuci==nullptr)
            throw std::logic_error("Prazna lista");
        if(tekuci->sljedeci==nullptr) return false;
        tekuci=tekuci->sljedeci;
        return true;
    }

    void pocetak() override {
        if(duzina==0)
            throw std::logic_error("Prazna lista");
        tekuci=poc;
    }

    void kraj() override {
        if(duzina==0)
            throw std::logic_error("Prazna lista");
        tekuci=kr;
    }

    void dodajIspred(const Tip& el) override {
        Cvor<Tip>* novi = new Cvor<Tip>;
        novi->element = el;
        if (duzina == 0) {
            novi->prethodni = nullptr;
            novi->sljedeci = nullptr;
            poc = novi;
            kr = novi;
            tekuci = novi;
        } else {
            novi->sljedeci = tekuci;
            novi->prethodni = tekuci->prethodni;
            if (tekuci->prethodni != nullptr)
                tekuci->prethodni->sljedeci = novi;
            else
                poc = novi;
            tekuci->prethodni = novi;
        }
        duzina++;
    }

    void dodajIza(const Tip& el) override {
        Cvor<Tip>* novi = new Cvor<Tip>;
        novi->element = el;
        if (duzina == 0) {
            novi->prethodni = nullptr;
            novi->sljedeci = nullptr;
            poc = novi;
            kr = novi;
            tekuci = novi;
        } else {
            novi->prethodni = tekuci;
            novi->sljedeci = tekuci->sljedeci;
            if (tekuci->sljedeci != nullptr)
                tekuci->sljedeci->prethodni = novi;
            else
                kr = novi;
            tekuci->sljedeci = novi;
        }
        duzina++;
    }

    void obrisi() override {
        if (duzina==0)
            throw std::logic_error("Prazna lista");

        Cvor<Tip>* brisanje = tekuci;
        if(duzina==1) {
            poc=nullptr;
            kr=nullptr;
            tekuci=nullptr;
            delete brisanje;
            duzina--;
        } else if (tekuci==poc && duzina>1) {
            poc=poc->sljedeci;
            poc->prethodni=nullptr;
            tekuci=poc;
            delete brisanje;
            duzina--;
        } else if(tekuci==kr && duzina>1) {
            kr=kr->prethodni;
            kr->sljedeci=nullptr;
            tekuci=kr;
            delete brisanje;
            duzina--;
        } else {
            tekuci->prethodni->sljedeci=tekuci->sljedeci;
            tekuci->sljedeci->prethodni=tekuci->prethodni;
            tekuci=tekuci->sljedeci;
            delete brisanje;
            duzina--;
        }
    }

    Tip& operator[](int i) override {
        if(i<0 || i>=duzina) throw std::logic_error("Indeks nije validan");
        if (duzina==0) throw std::logic_error("Prazna lista");
        if (i>duzina/2) {
            Cvor<Tip> *it = kr;
            int pomak=0;
            while (pomak<duzina-1-i) {
                it=it->prethodni;
                pomak++;
            }
            return it->element;
        } else {
            Cvor<Tip> *it = poc;
            int pomak=0;
            while (pomak<i) {
                it=it->sljedeci;
                pomak++;
            }
            return it->element;
        }
    }

    const Tip& operator[](int i) const override {
        if(i<0 || i>=duzina) throw std::logic_error("Indeks nije validan");
        if (duzina==0) throw std::logic_error("Prazna lista");
        if (i>duzina/2) {
            Cvor<Tip> *it = kr;
            int pomak=0;
            while (pomak<duzina-1-i) {
                it=it->prethodni;
                pomak++;
            }
            return it->element;
        } else {
            Cvor<Tip> *it = poc;
            int pomak=0;
            while (pomak<i) {
                it=it->sljedeci;
                pomak++;
            }
            return it->element;
        }
    }

    DvostrukaLista(const DvostrukaLista<Tip>& druga)
        : poc(nullptr), tekuci(nullptr), kr(nullptr), duzina(0) {
        Cvor<Tip>* itDruga = druga.poc;
        Cvor<Tip>* prethodniNovi = nullptr;
        while (itDruga != nullptr) {
            Cvor<Tip>* novi = new Cvor<Tip>;
            novi->element = itDruga->element;
            novi->prethodni = prethodniNovi;
            novi->sljedeci = nullptr;
            if (prethodniNovi == nullptr) {
                poc = novi;
            } else {
                prethodniNovi->sljedeci = novi;
            }
            kr = novi;
            if (itDruga == druga.tekuci)
                tekuci = novi;
            prethodniNovi = novi;
            itDruga = itDruga->sljedeci;
            duzina++;
        }
    }

    DvostrukaLista<Tip>& operator=(const DvostrukaLista<Tip>& druga) {
        if (this == &druga)
            return *this;
        Cvor<Tip>* it = poc;
        while (it != nullptr) {
            Cvor<Tip>* zaBrisanje = it;
            it = it->sljedeci;
            delete zaBrisanje;
        }
        poc = kr = tekuci = nullptr;
        duzina = 0;
        Cvor<Tip>* itDruga = druga.poc;
        Cvor<Tip>* prethodniNovi = nullptr;

        while (itDruga != nullptr) {
            Cvor<Tip>* novi = new Cvor<Tip>;
            novi->element = itDruga->element;
            novi->prethodni = prethodniNovi;
            novi->sljedeci = nullptr;

            if (prethodniNovi == nullptr) {
                poc = novi;
            } else {
                prethodniNovi->sljedeci = novi;
            }
            kr = novi;
            if (itDruga == druga.tekuci)
                tekuci = novi;

            prethodniNovi = novi;
            itDruga = itDruga->sljedeci;
            duzina++;
        }
        return *this;
    }

    Iterator<Tip> begin() const override {
        return Iterator<Tip>(*this, poc);
    }

    Iterator<Tip> end() const override {
        return Iterator<Tip>(*this, nullptr);
    }
};

template <typename Tip>
class Iterator {
    const DvostrukaLista<Tip> *lista;
    const Cvor<Tip> *trenutnicvor;
public:
    Iterator(const DvostrukaLista<Tip>& l) : lista(&l), trenutnicvor(l.poc) {}
    Iterator(const DvostrukaLista<Tip>& l, const Cvor<Tip>* c)
        : lista(&l), trenutnicvor(c) {}
    const Tip& trenutni() const {
        if (trenutnicvor == nullptr)
            throw std::logic_error("Ne postoji tekuci element");
        return trenutnicvor->element;
    }
    void sljedeci() {
        trenutnicvor=trenutnicvor->sljedeci;
    }
    void prethodni() {
        trenutnicvor=trenutnicvor->prethodni;
    }
    void pocetak() {
        trenutnicvor=lista->poc;
    }
    void kraj() {
        trenutnicvor=lista->kr;
    }
    bool jednak(const Iterator &iter) const {
        return trenutnicvor==iter.trenutnicvor;
    }
};

template <typename Tip>
class DvostraniRed {
private:
    DvostrukaLista<Tip> lista;

public:
    DvostraniRed() {}

    ~DvostraniRed() {}

    DvostraniRed(const DvostraniRed& dr) : lista(dr.lista) {}

    DvostraniRed& operator=(const DvostraniRed& dr) {
        if(this != &dr) {
            lista = dr.lista;
        }
        return *this;
    }

    void brisi() {
        while(lista.brojElemenata() > 0) {
            lista.pocetak();
            lista.obrisi();
        }
    }

    int brojElemenata() const {
        return lista.brojElemenata();
    }

    void staviNaVrh(const Tip& el) {
        if(lista.brojElemenata() == 0) {
            lista.dodajIza(el);
        } else {
            lista.kraj();
            lista.dodajIza(el);
        }
    }

    Tip skiniSaVrha() {
        if(lista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        lista.kraj();
        Tip el = lista.trenutni();
        lista.obrisi();
        return el;
    }

    void staviNaCelo(const Tip& el) {
        if(lista.brojElemenata() == 0) {
            lista.dodajIza(el);
        } else {
            lista.pocetak();
            lista.dodajIspred(el);
        }
    }

    Tip skiniSaCela() {
        if(lista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        lista.pocetak();
        Tip el = lista.trenutni();
        lista.obrisi();
        return el;
    }

    Tip& vrh() {
        if(lista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        lista.kraj();
        return lista.trenutni();
    }

    const Tip& vrh() const {
        if(lista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        DvostrukaLista<Tip>& l = const_cast<DvostrukaLista<Tip>&>(lista);
        l.kraj();
        return l.trenutni();
    }

    Tip& celo() {
        if(lista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        lista.pocetak();
        return lista.trenutni();
    }

    const Tip& celo() const {
        if(lista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        DvostrukaLista<Tip>& l = const_cast<DvostrukaLista<Tip>&>(lista);
        l.pocetak();
        return l.trenutni();
    }
};

template <typename Tip>
class Stek {
private:
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor(const Tip& el, Cvor* slj = nullptr) : element(el), sljedeci(slj) {}
    };
    Cvor* vrh_steka;
    int velicina;

    void brisiSve() {
        while(vrh_steka != nullptr) {
            Cvor* temp = vrh_steka;
            vrh_steka = vrh_steka->sljedeci;
            delete temp;
        }
        velicina = 0;
    }

    void kopiraj(const Stek& s) {
        if(s.vrh_steka == nullptr) return;
        Stek<Tip> pomocni;
        Cvor* temp = s.vrh_steka;
        while(temp != nullptr) {
            pomocni.stavi(temp->element);
            temp = temp->sljedeci;
        }
        while(pomocni.velicina > 0) {
            stavi(pomocni.skini());
        }
    }

public:
    Stek() : vrh_steka(nullptr), velicina(0) {}

    ~Stek() { brisiSve(); }

    Stek(const Stek& s) : vrh_steka(nullptr), velicina(0) {
        kopiraj(s);
    }

    Stek& operator=(const Stek& s) {
        if(this != &s) {
            brisiSve();
            kopiraj(s);
        }
        return *this;
    }

    void brisi() { brisiSve(); }

    void stavi(const Tip& el) {
        Cvor* novi = new Cvor(el, vrh_steka);
        vrh_steka = novi;
        velicina++;
    }

    Tip skini() {
        if(velicina == 0) throw std::logic_error("Stek je prazan");
        Tip el = vrh_steka->element;
        Cvor* temp = vrh_steka;
        vrh_steka = vrh_steka->sljedeci;
        delete temp;
        velicina--;
        return el;
    }

    Tip& vrh() {
        if(velicina == 0) throw std::logic_error("Stek je prazan");
        return vrh_steka->element;
    }

    const Tip& vrh() const {
        if(velicina == 0) throw std::logic_error("Stek je prazan");
        return vrh_steka->element;
    }

    int brojElemenata() const { return velicina; }
};

void pretraga(Stek<std::vector<int>>& s, int trazeni) {
    if(s.brojElemenata() == 0) {
        std::cout << "Nema elementa" << std::endl;
        return;
    }

    Stek<std::vector<int>> pomocni;
    std::vector<int> trenutni;
    int brojac = 0;
    bool pronadjen = false;
    int indeks = -1;
    int dubina = -1;

    while(s.brojElemenata() > 0) {
        trenutni = s.skini();
        pomocni.stavi(trenutni);

        int lijevo = 0, desno = trenutni.size() - 1;
        bool nadjen = false;

        while(lijevo <= desno) {
            int sredina = (lijevo + desno) / 2;
            if(trenutni[sredina] == trazeni) {
                indeks = sredina;
                nadjen = true;
                break;
            } else if(trenutni[sredina] < trazeni) {
                lijevo = sredina + 1;
            } else {
                desno = sredina - 1;
            }
        }

        if(nadjen) {
            pronadjen = true;
            dubina = brojac;
            break;
        }
        brojac++;
    }

    while(pomocni.brojElemenata() > 0) {
        s.stavi(pomocni.skini());
    }

    if(pronadjen) {
        std::cout << indeks << " " << dubina << std::endl;
    } else {
        std::cout << "Nema elementa" << std::endl;
    }
}

int main() {
    DvostraniRed<int> dr;
    dr.staviNaVrh(1);
    dr.staviNaVrh(2);
    dr.staviNaCelo(0);
    std::cout << dr.celo() << " " << dr.vrh() << std::endl;
    std::cout << dr.skiniSaCela() << " " << dr.skiniSaVrha() << std::endl;
    std::cout << dr.brojElemenata() << std::endl;

    Stek<std::vector<int>> s;
    s.stavi({1,2,3,4,5});
    s.stavi({6,8});
    s.stavi({9,10,130});
    s.stavi({157,226,3424});

    pretraga(s, 10);
    pretraga(s, 8);
    pretraga(s, 999);

    return 0;
}
