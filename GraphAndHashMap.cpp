#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <stdexcept>
#include <list>
#include <utility>

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf {
public:
    UsmjereniGraf(int brojCvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int brojCvorova) = 0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina = 0) = 0;
    virtual void obrisiGranu(int polazni, int dolazni) = 0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina) = 0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const = 0;
    virtual bool postojiGrana(int polazni, int dolazni) const = 0;
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int cvor) const = 0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) const = 0;
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) = 0;
    virtual Cvor<TipOznake> dajCvor(int cvor) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake>
class Cvor {
    UsmjereniGraf<TipOznake>* graf;
    int redniBroj;

public:
    Cvor(UsmjereniGraf<TipOznake>* g, int rb) : graf(g), redniBroj(rb) {}

    TipOznake dajOznaku() const {
        return graf->dajOznakuCvora(redniBroj);
    }

    void postaviOznaku(TipOznake oznaka) {
        graf->postaviOznakuCvora(redniBroj, oznaka);
    }

    int dajRedniBroj() const {
        return redniBroj;
    }
};

template <typename TipOznake>
class Grana {
    UsmjereniGraf<TipOznake>* graf;
    int polazni;
    int dolazni;

public:
    Grana(UsmjereniGraf<TipOznake>* g, int pol, int dol) : graf(g), polazni(pol), dolazni(dol) {}

    float dajTezinu() const {
        return graf->dajTezinuGrane(polazni, dolazni);
    }

    void postaviTezinu(float tezina) {
        graf->postaviTezinuGrane(polazni, dolazni, tezina);
    }

    TipOznake dajOznaku() const {
        return graf->dajOznakuGrane(polazni, dolazni);
    }

    void postaviOznaku(TipOznake oznaka) {
        graf->postaviOznakuGrane(polazni, dolazni, oznaka);
    }

    Cvor<TipOznake> dajPolazniCvor() const {
        return graf->dajCvor(polazni);
    }

    Cvor<TipOznake> dajDolazniCvor() const {
        return graf->dajCvor(dolazni);
    }
};

template <typename TipOznake>
class GranaIterator {
    UsmjereniGraf<TipOznake>* graf;
    int trenutniPolazni;
    int trenutniDolazni;
    int brojCvorova;

    void pronadjiSljedecuGranu() {
        while(trenutniPolazni < brojCvorova) {
            while(trenutniDolazni < brojCvorova) {
                if(graf->postojiGrana(trenutniPolazni, trenutniDolazni))
                    return;
                trenutniDolazni++;
            }
            trenutniPolazni++;
            trenutniDolazni = 0;
        }
    }

public:
    GranaIterator(UsmjereniGraf<TipOznake>* g, int pol, int dol)
        : graf(g), trenutniPolazni(pol), trenutniDolazni(dol) {
        brojCvorova = graf->dajBrojCvorova();
        if(trenutniPolazni < brojCvorova && !graf->postojiGrana(trenutniPolazni, trenutniDolazni))
            pronadjiSljedecuGranu();
    }

    Grana<TipOznake> operator*() {
        return graf->dajGranu(trenutniPolazni, trenutniDolazni);
    }

    bool operator==(const GranaIterator &iter) const {
        return trenutniPolazni == iter.trenutniPolazni && trenutniDolazni == iter.trenutniDolazni;
    }

    bool operator!=(const GranaIterator &iter) const {
        return !(*this == iter);
    }

    GranaIterator& operator++() {
        if(trenutniPolazni >= brojCvorova)
            throw std::runtime_error("Iterator je van opsega");
        trenutniDolazni++;
        pronadjiSljedecuGranu();
        return *this;
    }

    GranaIterator operator++(int) {
        GranaIterator temp = *this;
        ++(*this);
        return temp;
    }
};

template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
    struct CvorGrane {
        int dolazniCvor;
        float tezina;
        TipOznake oznaka;
        CvorGrane(int d, float t) : dolazniCvor(d), tezina(t) {}
    };

    std::vector<std::list<CvorGrane>> listeSusjedstva;
    std::vector<TipOznake> oznakeCvorova;
    int brojCvorova;

public:
    ListaGraf(int n) : UsmjereniGraf<TipOznake>(n), brojCvorova(n) {
        listeSusjedstva.resize(n);
        oznakeCvorova.resize(n);
    }

    ~ListaGraf() {}

    int dajBrojCvorova() const {
        return brojCvorova;
    }

    void postaviBrojCvorova(int n) {
        if(n < brojCvorova)
            throw std::logic_error("Ne moze se smanjiti broj cvorova");

        listeSusjedstva.resize(n);
        oznakeCvorova.resize(n);
        brojCvorova = n;
    }

    void dodajGranu(int polazni, int dolazni, float tezina = 0) {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");

        for(auto& grana : listeSusjedstva[polazni]) {
            if(grana.dolazniCvor == dolazni) {
                grana.tezina = tezina;
                return;
            }
        }

        listeSusjedstva[polazni].push_back(CvorGrane(dolazni, tezina));
    }

    void obrisiGranu(int polazni, int dolazni) {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");

        listeSusjedstva[polazni].remove_if([dolazni](const CvorGrane& g) {
            return g.dolazniCvor == dolazni;
        });
    }

    void postaviTezinuGrane(int polazni, int dolazni, float tezina) {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");

        for(auto& grana : listeSusjedstva[polazni]) {
            if(grana.dolazniCvor == dolazni) {
                grana.tezina = tezina;
                return;
            }
        }
        throw std::logic_error("Grana ne postoji");
    }

    float dajTezinuGrane(int polazni, int dolazni) const {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");

        for(const auto& grana : listeSusjedstva[polazni]) {
            if(grana.dolazniCvor == dolazni)
                return grana.tezina;
        }
        throw std::logic_error("Grana ne postoji");
    }

    bool postojiGrana(int polazni, int dolazni) const {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            return false;

        for(const auto& grana : listeSusjedstva[polazni]) {
            if(grana.dolazniCvor == dolazni)
                return true;
        }
        return false;
    }

    void postaviOznakuCvora(int cvor, TipOznake oznaka) {
        if(cvor < 0 || cvor >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");
        oznakeCvorova[cvor] = oznaka;
    }

    TipOznake dajOznakuCvora(int cvor) const {
        if(cvor < 0 || cvor >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");
        return oznakeCvorova[cvor];
    }

    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");

        for(auto& grana : listeSusjedstva[polazni]) {
            if(grana.dolazniCvor == dolazni) {
                grana.oznaka = oznaka;
                return;
            }
        }
        throw std::logic_error("Grana ne postoji");
    }

    TipOznake dajOznakuGrane(int polazni, int dolazni) const {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");

        for(const auto& grana : listeSusjedstva[polazni]) {
            if(grana.dolazniCvor == dolazni)
                return grana.oznaka;
        }
        throw std::logic_error("Grana ne postoji");
    }

    Grana<TipOznake> dajGranu(int polazni, int dolazni) {
        if(polazni < 0 || polazni >= brojCvorova || dolazni < 0 || dolazni >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");
        return Grana<TipOznake>(this, polazni, dolazni);
    }

    Cvor<TipOznake> dajCvor(int cvor) {
        if(cvor < 0 || cvor >= brojCvorova)
            throw std::domain_error("Nevalidan cvor");
        return Cvor<TipOznake>(this, cvor);
    }

    GranaIterator<TipOznake> dajGranePocetak() {
        return GranaIterator<TipOznake>(this, 0, 0);
    }

    GranaIterator<TipOznake> dajGraneKraj() {
        return GranaIterator<TipOznake>(this, brojCvorova, 0);
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake>* graf, int pocetni, void (*funkcija)(Cvor<TipOznake>)) {
    std::vector<bool> posjecen(graf->dajBrojCvorova(), false);
    std::queue<int> red;

    posjecen[pocetni] = true;
    red.push(pocetni);

    while(!red.empty()) {
        int trenutni = red.front();
        red.pop();

        funkcija(graf->dajCvor(trenutni));

        for(int i = 0; i < graf->dajBrojCvorova(); i++) {
            if(graf->postojiGrana(trenutni, i) && !posjecen[i]) {
                posjecen[i] = true;
                red.push(i);
            }
        }
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* graf, int pocetni, void (*funkcija)(Cvor<TipOznake>)) {
    std::vector<bool> posjecen(graf->dajBrojCvorova(), false);
    std::stack<int> stek;

    stek.push(pocetni);

    while(!stek.empty()) {
        int trenutni = stek.top();
        stek.pop();

        if(!posjecen[trenutni]) {
            posjecen[trenutni] = true;
            funkcija(graf->dajCvor(trenutni));

            for(int i = graf->dajBrojCvorova() - 1; i >= 0; i--) {
                if(graf->postojiGrana(trenutni, i) && !posjecen[i]) {
                    stek.push(i);
                }
            }
        }
    }
}

template <typename K, typename V>
class Mapa {
public:
    virtual ~Mapa() {}
    virtual V& operator[](const K& kljuc) = 0;
    virtual const V operator[](const K& kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const K& kljuc) = 0;
};

template <typename K, typename V>
class HashMapaLan : public Mapa<K, V> {
    struct Element {
        K kljuc;
        V vrijednost;
        Element* sljedeci;
        Element(const K& k, const V& v) : kljuc(k), vrijednost(v), sljedeci(nullptr) {}
    };

    Element** tabela;
    int kapacitet;
    int velicina;
    int (*hashFunc)(K, int);

    void umetniSortirano(Element*& lista, const K& kljuc, const V& vrijednost) {
        if(lista == nullptr || lista->kljuc >= kljuc) {
            if(lista != nullptr && lista->kljuc == kljuc) {
                lista->vrijednost = vrijednost;
                return;
            }
            Element* novi = new Element(kljuc, vrijednost);
            novi->sljedeci = lista;
            lista = novi;
            velicina++;
            return;
        }

        Element* trenutni = lista;
        while(trenutni->sljedeci != nullptr && trenutni->sljedeci->kljuc < kljuc) {
            trenutni = trenutni->sljedeci;
        }

        if(trenutni->sljedeci != nullptr && trenutni->sljedeci->kljuc == kljuc) {
            trenutni->sljedeci->vrijednost = vrijednost;
            return;
        }

        Element* novi = new Element(kljuc, vrijednost);
        novi->sljedeci = trenutni->sljedeci;
        trenutni->sljedeci = novi;
        velicina++;
    }

    void prosiri() {
        int stariKapacitet = kapacitet;
        Element** staraTabela = tabela;

        kapacitet *= 2;
        tabela = new Element*[kapacitet];
        for(int i = 0; i < kapacitet; i++)
            tabela[i] = nullptr;

        velicina = 0;

        for(int i = 0; i < stariKapacitet; i++) {
            Element* trenutni = staraTabela[i];
            while(trenutni != nullptr) {
                (*this)[trenutni->kljuc] = trenutni->vrijednost;
                Element* temp = trenutni;
                trenutni = trenutni->sljedeci;
                delete temp;
            }
        }

        delete[] staraTabela;
    }

public:
    HashMapaLan() : kapacitet(100), velicina(0), hashFunc(nullptr) {
        tabela = new Element*[kapacitet];
        for(int i = 0; i < kapacitet; i++)
            tabela[i] = nullptr;
    }

    ~HashMapaLan() {
        for(int i = 0; i < kapacitet; i++) {
            Element* trenutni = tabela[i];
            while(trenutni != nullptr) {
                Element* temp = trenutni;
                trenutni = trenutni->sljedeci;
                delete temp;
            }
        }
        delete[] tabela;
    }

    void setHashFunction(int (*f)(K, int)) {
        hashFunc = f;
    }

    V& operator[](const K& kljuc) {
        if(hashFunc == nullptr)
            throw std::logic_error("Hash funkcija nije definisana");

        if(velicina >= kapacitet * 0.7)
            prosiri();

        int indeks = hashFunc(kljuc, kapacitet);

        Element* trenutni = tabela[indeks];
        while(trenutni != nullptr) {
            if(trenutni->kljuc == kljuc)
                return trenutni->vrijednost;
            trenutni = trenutni->sljedeci;
        }

        umetniSortirano(tabela[indeks], kljuc, V());

        trenutni = tabela[indeks];
        while(trenutni != nullptr) {
            if(trenutni->kljuc == kljuc)
                return trenutni->vrijednost;
            trenutni = trenutni->sljedeci;
        }

        throw std::logic_error("Greska u HashMapi");
    }

    const V operator[](const K& kljuc) const {
        if(hashFunc == nullptr)
            throw std::logic_error("Hash funkcija nije definisana");

        int indeks = hashFunc(kljuc, kapacitet);
        Element* trenutni = tabela[indeks];

        while(trenutni != nullptr) {
            if(trenutni->kljuc == kljuc)
                return trenutni->vrijednost;
            trenutni = trenutni->sljedeci;
        }

        return V();
    }

    int brojElemenata() const {
        return velicina;
    }

    void obrisi() {
        for(int i = 0; i < kapacitet; i++) {
            Element* trenutni = tabela[i];
            while(trenutni != nullptr) {
                Element* temp = trenutni;
                trenutni = trenutni->sljedeci;
                delete temp;
            }
            tabela[i] = nullptr;
        }
        velicina = 0;
    }

    void obrisi(const K& kljuc) {
        if(hashFunc == nullptr)
            throw std::logic_error("Hash funkcija nije definisana");

        int indeks = hashFunc(kljuc, kapacitet);
        Element* trenutni = tabela[indeks];
        Element* prethodni = nullptr;

        while(trenutni != nullptr) {
            if(trenutni->kljuc == kljuc) {
                if(prethodni == nullptr)
                    tabela[indeks] = trenutni->sljedeci;
                else
                    prethodni->sljedeci = trenutni->sljedeci;
                delete trenutni;
                velicina--;
                return;
            }
            prethodni = trenutni;
            trenutni = trenutni->sljedeci;
        }
    }
};

int hashString(std::string s, int max) {
    int h = 5381;
    for(char c : s)
        h = h * 33 + c;
    return std::abs(h % max);
}

void ispisiCvor(Cvor<bool> cvor) {
    std::cout << cvor.dajRedniBroj() << " ";
}

int main() {
    try {
        UsmjereniGraf<bool>* lg = new ListaGraf<bool>(4);
        lg->dodajGranu(0, 1, 1.5);
        lg->dodajGranu(0, 2, 2.5);
        lg->dodajGranu(1, 2, 0.5);
        lg->dodajGranu(2, 3, 1.0);

        std::cout << "Ispis svih grana:" << std::endl;
        for(GranaIterator<bool> iter = lg->dajGranePocetak(); iter != lg->dajGraneKraj(); ++iter) {
            std::cout << (*iter).dajPolazniCvor().dajRedniBroj() << " -> "
                      << (*iter).dajDolazniCvor().dajRedniBroj() << " (tezina: "
                      << (*iter).dajTezinu() << ")" << std::endl;
        }

        std::cout << std::endl << "BFS obilazak (pocetak 0): ";
        bfs(lg, 0, ispisiCvor);
        std::cout << std::endl;

        std::cout << "DFS obilazak (pocetak 0): ";
        dfs(lg, 0, ispisiCvor);
        std::cout << std::endl << std::endl;
        delete lg;

        HashMapaLan<std::string, int> mapa;
        mapa.setHashFunction(hashString);

        mapa["jabuka"] = 5;
        mapa["kruska"] = 3;
        mapa["banana"] = 7;
        mapa["ananas"] = 2;

        std::cout << "jabuka: " << mapa["jabuka"] << std::endl;
        std::cout << "kruska: " << mapa["kruska"] << std::endl;
        std::cout << "banana: " << mapa["banana"] << std::endl;
        std::cout << "Broj elemenata: " << mapa.brojElemenata() << std::endl;

        mapa.obrisi("kruska");
        std::cout << "Nakon brisanja kruske, broj elemenata: " << mapa.brojElemenata() << std::endl;

    } catch(const std::exception& e) {
        std::cout << "Greska: " << e.what() << std::endl;
    }

    return 0;
}
