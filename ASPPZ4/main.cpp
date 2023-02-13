#include <iostream>
#include <vector>

using namespace std;

template <typename Tip> class Lista {
public:
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip &trenutni() = 0;
  virtual Tip trenutni() const = 0;
  virtual Tip &operator[](int i) = 0;
  virtual Tip operator[](int i) const = 0;
};

template <typename Tip> class Cvor {
public:
  Tip v;
  Cvor<Tip> *prethodni;
  Cvor<Tip> *sljedeci;
  Cvor() : v(), prethodni(nullptr), sljedeci(nullptr) {}
  Cvor(const Cvor &c) : v(c.v), prethodni(c.prethodni), sljedeci(c.sljedeci) {}
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
  int duzina;
  Cvor<Tip> *trenutacni;
  Cvor<Tip> *prvi;
  Cvor<Tip> *zadnji;

public:
  DvostrukaLista() : duzina(0) {
    trenutacni = nullptr;
    prvi = nullptr;
    zadnji = nullptr;
  }
  ~DvostrukaLista() {
    Cvor<Tip> *temp;
    while (prvi != nullptr) {
      temp = prvi->sljedeci;
      delete prvi;
      prvi = temp;
    }
  }
  DvostrukaLista(const DvostrukaLista &dl) {
    duzina = dl.duzina;
    if (duzina != 0) {
      prvi = new Cvor<Tip>(*dl.prvi);
      Cvor<Tip> *noviPrvi = prvi;
      while (noviPrvi->sljedeci != nullptr) {
        noviPrvi->sljedeci = new Cvor<Tip>(*noviPrvi->sljedeci);
        noviPrvi = noviPrvi->sljedeci;
      }
      zadnji = noviPrvi;
    }
  }
  int brojElemenata() const { return duzina; }
  bool prethodni() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (trenutacni == prvi)
      return false;
    trenutacni = trenutacni->prethodni;
    return true;
  }
  bool sljedeci() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (trenutacni == zadnji)
      return false;
    trenutacni = trenutacni->sljedeci;
    return true;
  }
  Tip trenutni() const {
    if (duzina == 0)
      throw "Lista je prazna!";
    return trenutacni->v;
  }
  Tip &trenutni() {
    if (duzina == 0)
      throw "Lista je prazna!";
    return trenutacni->v;
  }
  void pocetak() {
    if (duzina == 0)
      throw "Lista je prazna!";
    trenutacni = prvi;
  }
  void kraj() {
    if (duzina == 0)
      throw "Lista je prazna!";
    trenutacni = zadnji;
  }
  void obrisi() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (trenutacni != prvi) {
      Cvor<Tip> *noviPrethodni = trenutacni->prethodni;
      Cvor<Tip> *noviSljedeci = trenutacni->sljedeci;
      noviPrethodni->sljedeci = noviSljedeci;
      if (trenutacni->sljedeci != nullptr)
        trenutacni->sljedeci->prethodni = noviPrethodni;
      delete trenutacni;
      if (noviPrethodni->sljedeci != nullptr)
        trenutacni = noviPrethodni->sljedeci;
      else
        trenutacni = noviPrethodni;
    } else if (trenutacni == prvi) {
      Cvor<Tip> *noviSljedeci = trenutacni->sljedeci;
      delete trenutacni;
      trenutacni = noviSljedeci;
      prvi = noviSljedeci;
    }
    duzina--;
  }
  void dodajIspred(const Tip &el) {
    if (duzina == 0) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->v = el;
      trenutacni = novi;
      prvi = novi;
      zadnji = novi;
    } else if (trenutacni != prvi) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->sljedeci = trenutacni;
      novi->prethodni = trenutacni->prethodni;
      novi->v = el;
      trenutacni->prethodni->sljedeci = novi;
      trenutacni->prethodni = novi;
    } else if (trenutacni == prvi) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->sljedeci = trenutacni;
      novi->v = el;
      trenutacni->prethodni = novi;
      prvi = novi;
    }
    duzina++;
  }
  void dodajIza(const Tip &el) { // 1 2 3 -> 1 2 5 3
    if (duzina == 0) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->v = el;
      trenutacni = novi;
      prvi = novi;
      zadnji = novi;
    } else if (trenutacni != zadnji) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->prethodni = trenutacni;
      novi->sljedeci = trenutacni->sljedeci;
      novi->v = el;
      trenutacni->sljedeci = novi;
      if (trenutacni->sljedeci != nullptr)
        trenutacni->sljedeci->prethodni = novi;
    } else if (trenutacni == zadnji) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->sljedeci = nullptr;
      novi->prethodni = trenutacni;
      novi->v = el;
      trenutacni->sljedeci = novi;
      zadnji = trenutacni->sljedeci;
      if (trenutacni->sljedeci != nullptr)
        trenutacni->sljedeci->prethodni = novi;
    }
    duzina++;
  }
  Tip operator[](int i) const {
    if (i < 0 || i >= duzina)
      throw "Neispravan indeks!";
    Cvor<Tip> *novi = prvi;
    for (int j = 0; j < i; j++)
      novi = novi->sljedeci;
    return novi->v;
  }
  Tip &operator[](int i) {
    if (i < 0 || i >= duzina)
      throw "Neispravan indeks!";
    Cvor<Tip> *novi = prvi;
    for (int j = 0; j < i; j++)
      novi = novi->sljedeci;
    return novi->v;
  }
  DvostrukaLista &operator=(const DvostrukaLista &dl) {
    if (this == &dl)
      return *this;
    if (dl.duzina == 0) {
      prvi = nullptr;
      zadnji = nullptr;
      trenutacni = nullptr;
    } else {
      Cvor<Tip> *noviPrvi = prvi;
      while (prvi != nullptr) {
        noviPrvi = prvi->sljedeci;
        delete prvi;
        prvi = noviPrvi;
      }
      prvi = new Cvor<Tip>(*dl.prvi);
      duzina = dl.duzina;
      Cvor<Tip> *temp = prvi;
      while (temp->sljedeci != nullptr) {
        temp->sljedeci = new Cvor<Tip>(*temp->sljedeci);
        temp = temp->sljedeci;
      }
    }
    return *this;
  }
};

template <typename Tip> class Red {
  DvostrukaLista<Tip> dl;

public:
  Red() {}
  void brisi() {
    int n = dl.brojElemenata();
    if (n != 0) {
      dl.pocetak();
      for (int i = 0; i < n; i++) {
        dl.obrisi();
      }
    }
  }
  Tip skini() {
    if (dl.brojElemenata() == 0)
      throw "Red je prazan!";
    dl.pocetak();
    Tip el = dl.trenutni();
    dl.obrisi();
    return el;
  }
  Tip &celo() {
    if (dl.brojElemenata() == 0)
      throw "Red je prazan!";
    dl.pocetak();
    return dl.trenutni();
  }
  int brojElemenata() { return dl.brojElemenata(); }
  void stavi(const Tip &el) {
    if (dl.brojElemenata() != 0)
      dl.kraj();
    dl.dodajIza(el);
  }
};

void test1() { // test metoda stavi() i brojElemenata()
  Red<int> r;
  for (int i = 1; i <= 5; i++)
    r.stavi(i);
  if (r.brojElemenata() == 5)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test2() { // test metode skini()
  Red<int> r;
  for (int i = 1; i <= 5; i++)
    r.stavi(i);
  r.skini();
  if (r.brojElemenata() == 4)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test3() { // test metode brisi()
  Red<int> r;
  for (int i = 1; i <= 5; i++)
    r.stavi(i);
  r.brisi();
  if (r.brojElemenata() == 0)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test4() { // test metode celo()
  Red<int> r;
  for (int i = 1; i <= 5; i++)
    r.stavi(i);
  if (r.celo() == 1)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
}