#include <iostream>
#include <stdexcept>

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
    virtual Iterator<Tip> end() const =0;
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
    return duzina; }

    Tip& trenutni() override {
        if (duzina==0 || tekuci==nullptr)
            throw std::logic_error("Prazna lista");
    return tekuci->element;
    }

    const Tip&trenutni() const override {
    if (duzina==0 || tekuci==nullptr)
            throw std::logic_error("Prazna lista");
    return tekuci->element; }

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
    return true;;
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
        poc = kr = tekuci = novi;
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
        poc = kr = tekuci = novi;
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
      }
      else {
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
      }
      else {
        Cvor<Tip> *it = poc;
        int pomak=0;
        while (pomak<i) {
            it=it->sljedeci;
            pomak++;
        }
        return it->element;
      }
  }

  //kopirajuci,operator dodjele
DvostrukaLista(const DvostrukaLista<Tip>& druga)
    : poc(nullptr), tekuci(nullptr), kr(nullptr), duzina(0)
{
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
    return Iterator<Tip>(*this, poc); }
    Iterator<Tip> end() const override {
     return Iterator<Tip>(*this,nullptr);
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

template<typename Tip>
Tip dajMaksimum (const Lista<Tip>& n) {
    if (n.brojElemenata() == 0) {
        throw std::logic_error("Lista je prazna");
    }
    Iterator<Tip> it = n.begin();
    Tip maks = it.trenutni();
    it.sljedeci();
    Iterator<Tip> it_kraj = n.end();
    while (!it.jednak(it_kraj)) {
        if (it.trenutni() > maks)
            maks = it.trenutni();
        it.sljedeci();
    }
    return maks;
}

//pomocna funkcija za ispis da ne ponavljamo kod
template <typename Tip>
void ispisiListu(const DvostrukaLista<Tip> &l) {
    std::cout<<"[";
    for (int i=0; i<l.brojElemenata(); i++) {
        std::cout<<l[i];
        if(i<l.brojElemenata()-1)
        std::cout<<", ";
    }
    std::cout<<"]";
}


//funkcije za testove
void test_brojElemenata() {
    DvostrukaLista<int> l;
    std::cout << "broj elemenata za praznu listu " << l.brojElemenata() << std::endl;
    l.dodajIza(1);
    l.dodajIza(2);
    std::cout << " dodali smo dva, sad je broj elemenata " << l.brojElemenata() << std::endl;
}

void test_dodavanje() {
    DvostrukaLista<int> l;
    std::cout << "pocetna lista "; ispisiListu(l); std::cout << std::endl;
    l.dodajIza(20);
    std::cout << "Dodali 20 iza "; ispisiListu(l); std::cout << ", trenutni je " << l.trenutni() << std::endl;
    l.dodajIza(30);
    std::cout << "Dodali 30 iza "; ispisiListu(l); std::cout << ", trenutni je " << l.trenutni() << std::endl;
    l.dodajIspred(10);
    std::cout << "Dodali 10 ispred: "; ispisiListu(l); std::cout << ", trenutni je " << l.trenutni() << std::endl;
}

void test_kretanje() {
    DvostrukaLista<int> l;
    l.dodajIza(10);
    l.dodajIza(20);
    l.dodajIza(30);
    std::cout << "lista "; ispisiListu(l); std::cout << ". pocetni trenutni " << l.trenutni() << std::endl;
    l.pocetak();
    std::cout << "nakon pocetak(), trenutni je " << l.trenutni() << std::endl;
    l.sljedeci();
    std::cout << "nakon sljedeci(), trenutni je " << l.trenutni() << std::endl;
    l.kraj();
    std::cout << "nakon kraj(),trenutni je " << l.trenutni() << std::endl;
    l.prethodni();
    std::cout << "nakon prethodni(),trenutni je " << l.trenutni() << std::endl;
}

void test_trenutni() {
    DvostrukaLista<int> l;
    l.dodajIza(1);
    l.dodajIza(2);
    l.sljedeci();
    std::cout << "prije izmjene, trenutni je " << l.trenutni() << std::endl;
    l.trenutni() = 22;
    std::cout << "nakon izmjene, trenutni je " << l.trenutni() << std::endl;
}

void test_obrisi() {
    DvostrukaLista<int> l;
    l.dodajIza(1);
    l.dodajIza(2);
    l.dodajIza(3);
    l.sljedeci();
    std::cout << "lista prije brisanja "; ispisiListu(l); std::cout << ", trenutni je " << l.trenutni() << std::endl;
    l.obrisi();
    std::cout << "lista nakon brisanja srednjeg elementa "; ispisiListu(l); std::cout << ", trenutni je " << l.trenutni() << std::endl;
    l.pocetak();
    l.obrisi();
    std::cout << "lista nakon brisanja pocetnog "; ispisiListu(l); std::cout << ", trenutni jr " << l.trenutni() << std::endl;
    l.obrisi();
    std::cout << "lista nakon brisanja jedinog elementa u listi "; ispisiListu(l); std::cout << ", broj elemenata je " << l.brojElemenata() << std::endl;
}

void test_operator_uglaste_zagrade() {
    DvostrukaLista<int> l;
    l.dodajIza(100);
    l.dodajIza(200);
    l.dodajIza(300);
    l.dodajIza(400);

    std::cout << "element na indeksu 0 je " << l[0] << std::endl;
    std::cout << "element na indeksu 3 " << l[3] << std::endl;
    std::cout << "element na indeksu 2 " << l[2] << std::endl;
}

void test_kopiranje() {
    DvostrukaLista<int> original;
    original.dodajIza(1);
    original.dodajIza(2);
    original.dodajIza(3);
    original.sljedeci();

    DvostrukaLista<int> kopija = original;
    original.obrisi();

    std::cout << "originalna lista: "; ispisiListu(original); std::cout << ", trenutni element je" << original.trenutni() << std::endl;
    std::cout << "kopija lista "; ispisiListu(kopija); std::cout << ", trenutni je " << kopija.trenutni() << std::endl;

    DvostrukaLista<int> dodjela;
    dodjela = kopija;
    kopija.kraj();
    kopija.obrisi();
    std::cout << "Dodjela lista  "; ispisiListu(dodjela); std::cout << ", trenutni je " << dodjela.trenutni() << std::endl;
}

void test_iterator_i_dajMaksimum() {
    DvostrukaLista<float> l;
    l.dodajIza(5.5f);
    l.dodajIza(10.2f);
    l.dodajIza(1.0f);

    std::cout << "ispis liste "; ispisiListu(l); std::cout << std::endl;
    std::cout << "maks element " << dajMaksimum(l) << std::endl;

    std::cout << "ispis elemenata pomocu iteratora ";
    Iterator<float> it = l.begin();
    Iterator<float> it_kraj = l.end();
    while (!it.jednak(it_kraj)) {
        std::cout << it.trenutni() << " ";
        it.sljedeci();
    }
    std::cout << std::endl;
}

int main()
{
    test_brojElemenata();
    test_dodavanje();
    test_kretanje();
    test_trenutni();
    test_obrisi();
    test_operator_uglaste_zagrade();
    test_kopiranje();
    test_iterator_i_dajMaksimum();
    return 0;
}
