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
protected:
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
    Cvor<Tip> *temp = prvi;
    while (temp != nullptr) {
      Cvor<Tip> *n = temp;
      temp = temp->sljedeci;
      delete n;
    }
    prvi = nullptr;
    trenutacni = nullptr;
    zadnji = nullptr;
    duzina = 0;
  }
  DvostrukaLista(const DvostrukaLista &dl) {
    if (&dl == this)
      return;
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
    if (trenutacni->prethodni == nullptr)
      return false;
    trenutacni = trenutacni->prethodni;
    return true;
  }
  bool sljedeci() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (trenutacni->sljedeci == nullptr)
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
      throw "Lista je prazna";
    else if (duzina == 1) {
      delete trenutacni;
      trenutacni = nullptr;
      prvi = nullptr;
      zadnji = nullptr;
    } else if (trenutacni == zadnji) {
      zadnji = zadnji->prethodni;
      delete trenutacni;
      trenutacni = zadnji;
      trenutacni->sljedeci = nullptr;
    } else if (trenutacni == prvi) {
      prvi = prvi->sljedeci;
      delete trenutacni;
      trenutacni = prvi;
      trenutacni->prethodni = nullptr;
    } else {
      auto temp = trenutacni;
      trenutacni = trenutacni->sljedeci;
      temp->prethodni->sljedeci = trenutacni;
      trenutacni->prethodni = temp->prethodni;
      delete temp;
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
  void dodajIza(const Tip &el) {
    if (duzina == 0) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->v = el;
      prvi = trenutacni = zadnji = novi;
    } else {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->prethodni = trenutacni;
      novi->sljedeci = trenutacni->sljedeci;
      novi->v = el;
      trenutacni->sljedeci = novi;
      if (novi->sljedeci != nullptr)
        novi->sljedeci->prethodni = novi;
      if (trenutacni == zadnji)
        zadnji = trenutacni->sljedeci;
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
  void obrisiSve() {
    while (prvi != nullptr) {
      trenutacni = prvi;
      prvi = prvi->sljedeci;
      delete trenutacni;
    }
    prvi = nullptr;
    zadnji = nullptr;
    trenutacni = nullptr;
    duzina = 0;
  }
  void napraviNovu() {
    trenutacni = nullptr;
    prvi = nullptr;
    zadnji = nullptr;
    duzina = 0;
  }
};

template <typename Tip> class DvostraniRed {
  DvostrukaLista<Tip> dl;

public:
  DvostraniRed() : dl() {}
  ~DvostraniRed() {}
  void brisi() {
    dl.obrisiSve();
    dl.napraviNovu();
  }
  int brojElemenata() const { return dl.brojElemenata(); }
  Tip &vrh() {
    if (dl.brojElemenata() == 0)
      throw "Red je prazan";
    dl.kraj();
    return dl.trenutni();
  }
  Tip &celo() {
    if (dl.brojElemenata() == 0)
      throw "Red je prazan";
    dl.pocetak();
    return dl.trenutni();
  }
  void staviNaVrh(const Tip &el) {
    if (dl.brojElemenata() != 0) {
      dl.kraj();
    }
    dl.dodajIza(el);
  }
  void staviNaCelo(const Tip &el) {
    if (dl.brojElemenata() != 0) {
      dl.pocetak();
    }
    dl.dodajIspred(el);
  }
  Tip skiniSaVrha() {
    if (dl.brojElemenata() == 0)
      throw "Red je prazan";
    dl.kraj();
    Tip temp = dl.trenutni();
    dl.obrisi();
    return temp;
  }
  Tip skiniSaCela() {
    if (dl.brojElemenata() == 0)
      throw "Red je prazan";
    dl.pocetak();
    Tip temp = dl.trenutni();
    dl.obrisi();
    return temp;
  }
  DvostraniRed &operator=(const DvostraniRed &dr) {
    if (&dr == this)
      return *this;
    dl = dr.dl;
    return *this;
  }
  DvostraniRed(const DvostraniRed &dr) { dl = dr.dl; }
};

void test1() { // test metode brojElemenata()
  DvostraniRed<int> dr;
  for (int i = 1; i <= 5; i++)
    dr.staviNaVrh(i);
  if (dr.brojElemenata() == 5)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test2() { // test metoda staviNaVrh i vrh
  DvostraniRed<int> dr;
  dr.staviNaVrh(10);
  if (dr.vrh() == 10)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test3() { // test metoda staviNaCelo i celo
  DvostraniRed<int> dr;
  dr.staviNaCelo(10);
  if (dr.celo() == 10)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test4() { // test metode skiniSaVrha
  DvostraniRed<int> dr;
  for (int i = 1; i <= 5; i++)
    dr.staviNaVrh(i);
  dr.skiniSaVrha();
  if (dr.vrh() == 4)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test5() { // test metode skiniSaCela
  DvostraniRed<int> dr;
  for (int i = 1; i <= 5; i++)
    dr.staviNaCelo(i);
  dr.skiniSaCela();
  if (dr.celo() == 4)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test6() { // test metode brisi
  DvostraniRed<int> dr;
  for (int i = 1; i <= 5; i++)
    dr.staviNaCelo(i);
  dr.brisi();
  if (dr.brojElemenata() == 0)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test7() { // test konstruktora kopije
  DvostraniRed<int> dr;
  for (int i = 1; i <= 5; i++)
    dr.staviNaCelo(i);
  DvostraniRed<int> temp(dr);
  if (temp.brojElemenata() == dr.brojElemenata())
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test8() { // test operatora dodjele
  DvostraniRed<int> dr;
  for (int i = 1; i <= 5; i++)
    dr.staviNaCelo(i);
  DvostraniRed<int> temp = dr;
  if (temp.brojElemenata() == dr.brojElemenata())
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
  test5();
  test6();
  test7();
  test8();
}