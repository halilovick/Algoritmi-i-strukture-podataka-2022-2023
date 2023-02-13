#include <iostream>
#include <vector>

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

template <typename Tip> class NizLista : public Lista<Tip> {
private:
  int kapacitet;
  int duzina;
  int tekuci;
  Tip **L;

public:
  NizLista() : kapacitet(100), duzina(0), tekuci(0), L(nullptr) {}
  NizLista(const NizLista &nova)
      : kapacitet(nova.kapacitet), duzina(nova.duzina), tekuci(nova.tekuci) {
    L = new Tip *[kapacitet];
    for (int i = 0; i < nova.duzina; i++)
      L[i] = new Tip(*nova.L[i]);
  }
  ~NizLista() {
    if (L != nullptr)
      for (int i = 0; i < duzina; i++)
        delete L[i];
    delete[] L;
  }
  int brojElemenata() const { return duzina; }
  Tip &trenutni() {
    if (duzina == 0)
      throw "Lista je prazna!";
    return *L[tekuci];
  }
  Tip trenutni() const {
    if (duzina == 0)
      throw "Lista je prazna!";
    return *L[tekuci];
  }
  bool prethodni() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (tekuci == 0)
      return false;
    tekuci--;
    return true;
  }
  bool sljedeci() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (tekuci == duzina - 1)
      return false;
    tekuci++;
    return true;
  }
  void pocetak() {
    if (duzina == 0)
      throw "Lista je prazna!";
    tekuci = 0;
  }
  void kraj() {
    if (duzina == 0)
      throw "Lista je prazna!";
    tekuci = duzina - 1;
  }
  void obrisi() {
    if (duzina == 0)
      throw "Lista je prazna!";
    else {
      delete L[tekuci];
      duzina--;
      for (int i = tekuci; i < duzina; i++)
        L[i] = L[i + 1];
      if (tekuci == duzina)
        tekuci--;
    }
    if (duzina == 0) {
      delete[] L;
      L = nullptr;
    }
  }
  NizLista &operator=(const NizLista &novi) {
    if (this == &novi)
      return *this;
    if (duzina > 0) {
      for (int i = 0; i < duzina; i++)
        delete L[i];
    }
    delete[] L;
    kapacitet = novi.kapacitet;
    duzina = novi.duzina;
    tekuci = novi.tekuci;
    L = new Tip *[kapacitet];
    for (int i = 0; i < duzina; i++)
      L[i] = new Tip(*novi.L[i]);
    return *this;
  }
  void dodajIza(const Tip &el) {
    if (duzina == 0) {
      tekuci = 0;
      L = new Tip *[kapacitet];
      L[0] = new Tip(el);
    } else {
      if (duzina == kapacitet) {
        Tip **p = L;
        kapacitet *= 2;
        L = new Tip *[kapacitet];
        for (int i = 0; i < duzina; i++)
          L[i] = p[i];
        delete[] p;
      }
      for (int i = duzina; i > tekuci + 1; i--)
        L[i] = L[i - 1];
      L[tekuci + 1] = new Tip(el);
    }
    duzina++;
  }
  void dodajIspred(const Tip &el) {
    if (duzina == 0) {
      tekuci = 0;
      L = new Tip *[kapacitet];
      L[0] = new Tip(el);
    } else {
      if (duzina == kapacitet) {
        Tip **p = L;
        kapacitet *= 2;
        L = new Tip *[kapacitet];
        for (int i = 0; i < duzina; i++)
          L[i] = p[i];
        delete[] p;
      }
      for (int i = duzina; i > tekuci; i--)
        L[i] = L[i - 1];
      L[tekuci] = new Tip(el);
      tekuci++;
    }
    duzina++;
  }

  Tip &operator[](int indeks) {
    if (indeks < 0 || indeks >= duzina)
      throw "Neispravan indeks";
    return *L[indeks];
  }
  Tip operator[](int indeks) const {
    if (indeks < 0 || indeks >= duzina)
      throw "Neispravan indeks";
    return *L[indeks];
  }
};

void NLtest1() { // test konstruktora bez parametara, indeksiranja, i metoda
                 // dodajIspred() i pocetak()
  NizLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIspred(i);
    l.pocetak();
  }
  std::cout << "Dodaj ispred: ";
  for (int i = 0; i < l.brojElemenata(); i++) {
    std::cout << l[i] << " ";
  }
  std::cout << std::endl;
}

void NLtest2() { // test konstruktora bez parametara i metoda dodajIza() i
                 // kraj()
  NizLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  std::cout << "Dodaj iza: ";
  for (int i = 0; i < l.brojElemenata(); i++)
    std::cout << l[i] << " ";
  std::cout << std::endl;
}

void NLtest3() { // test metoda sljedeci(), prethodni() i trenutni()
  NizLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  l.pocetak();
  std::cout << "Trenutni element: " << l.trenutni();
  l.sljedeci();
  std::cout << ", sljedeci element: " << l.trenutni();
  l.prethodni();
  std::cout << ", prethodni element: " << l.trenutni();
  std::cout << std::endl;
}

void NLtest4() { // test metoda obrisi() i brojElemenata()
  NizLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  std::cout << "Broj elemenata prije brisanja: " << l.brojElemenata();
  l.obrisi();
  std::cout << ", broj elemenata poslije brisanja: " << l.brojElemenata();
  std::cout << std::endl;
}

void NLtest5() { // test operatora dodjele
  NizLista<int> l;
  for (int i = 1; i <= 3; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  NizLista<int> temp = l;
  std::cout << "Elementi pocetne liste: ";
  for (int i = 0; i < l.brojElemenata(); i++)
    std::cout << l[i] << " ";
  std::cout << "Elementi kopirane liste: ";
  for (int i = 0; i < temp.brojElemenata(); i++)
    std::cout << temp[i] << " ";
  std::cout << std::endl;
}

void NLtest6() { // test konstruktora kopije
  NizLista<int> l;
  for (int i = 1; i <= 3; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  NizLista<int> temp(l);
  std::cout << "Elementi pocetne liste: ";
  for (int i = 0; i < l.brojElemenata(); i++)
    std::cout << l[i] << " ";
  std::cout << "Elementi kopirane liste: ";
  for (int i = 0; i < temp.brojElemenata(); i++)
    std::cout << temp[i] << " ";
  std::cout << std::endl;
}

template <typename Tip> class Cvor {
public:
  Tip v;
  Cvor *s;
  Cvor() : v(), s() {}
  Cvor(const Tip &x, Cvor *p) : v(x), s(p) {}
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
  Cvor<Tip> *p;
  Cvor<Tip> *tekuci;
  int duzina;

public:
  JednostrukaLista() : p(nullptr), tekuci(nullptr), duzina(0) {}
  JednostrukaLista(const JednostrukaLista &l) {
    duzina = l.duzina;
    if (duzina != 0) {
      p = new Cvor<Tip>(l.p->v, l.p->s);
      Cvor<Tip> *t = p;
      while (t->s != nullptr) {
        t->s = new Cvor<Tip>(*t->s);
        t = t->s;
      }
    }
  }
  ~JednostrukaLista() {
    Cvor<Tip> *t = p;
    while (p != nullptr) {
      t = p->s;
      delete p;
      p = t;
    }
  }
  int brojElemenata() const { return duzina; }
  Tip trenutni() const {
    if (duzina == 0)
      throw "Lista je prazna!";
    return tekuci->v;
  }
  Tip &trenutni() {
    if (duzina == 0)
      throw "Lista je prazna!";
    return tekuci->v;
  }
  bool prethodni() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (p == tekuci)
      return false;
    Cvor<Tip> *t = p;
    while (t->s != tekuci)
      t = t->s;
    tekuci = t;
    return true;
  }
  bool sljedeci() {
    if (duzina == 0)
      throw "Lista je prazna!";
    if (tekuci->s == nullptr)
      return false;
    tekuci = tekuci->s;
    return true;
  }
  void pocetak() {
    if (duzina == 0)
      throw "Lista je prazna!";
    tekuci = p;
  }
  void kraj() {
    if (duzina == 0)
      throw "Lista je prazna!";
    while (tekuci->s != nullptr)
      tekuci = tekuci->s;
  }
  void obrisi() {
    if (duzina == 0)
      throw "Lista je prazna!";
    else if (tekuci == p) {
      tekuci = p->s;
      delete p;
      p = tekuci;
    } else {
      Cvor<Tip> *t = p;
      while (t->s != tekuci)
        t = t->s;
      t->s = tekuci->s;
      delete tekuci;
      if (t->s == nullptr)
        tekuci = t;
      else
        tekuci = t->s;
    }
    duzina--;
  }
  void dodajIspred(const Tip &el) {
    if (duzina == 0) {
      Cvor<Tip> *t = new Cvor<Tip>;
      t->v = el;
      t->s = p;
      p = t;
      tekuci = p;
    } else if (p != tekuci) {
      Cvor<Tip> *y = p;
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->v = el;
      while (y->s != tekuci)
        y = y->s;
      y->s = novi;
      novi->s = tekuci;
    } else {
      Cvor<Tip> *t = new Cvor<Tip>;
      t->v = el;
      t->s = p;
      p = t;
    }
    duzina++;
  }
  void dodajIza(const Tip &el) {
    if (duzina == 0) {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->v = el;
      novi->s = nullptr;
      p = novi;
      tekuci = novi;
    } else {
      Cvor<Tip> *novi = new Cvor<Tip>;
      novi->v = el;
      novi->s = tekuci->s;
      tekuci->s = novi;
    }
    duzina++;
  }
  JednostrukaLista &operator=(const JednostrukaLista &l) {
    if (&l == this)
      return *this;
    else if (l.duzina != 0) {
      Cvor<Tip> *t = p;
      while (p != nullptr) {
        t = p->s;
        delete p;
        p = t;
      }
      p = new Cvor<Tip>(l.p->v, l.p->s);
      duzina = l.duzina;
      Cvor<Tip> *t2 = p;
      while (t2->s != nullptr) {
        t2->s = new Cvor<Tip>(t2->s->v, t2->s->s);
        t2 = t2->s;
      }
    }
    return *this;
  }
  Tip operator[](int i) const {
    if (i < 0 || i >= duzina)
      throw "Neispravan indeks!";
    Cvor<Tip> *t = p;
    for (int j = 0; j < i; j++)
      t = t->s;
    return t->v;
  }
  Tip &operator[](int i) {
    if (i < 0 || i >= duzina)
      throw "Neispravan indeks!";
    Cvor<Tip> *t = p;
    for (int j = 0; j < i; j++)
      t = t->s;
    return t->v;
  }
};

void JLtest1() {
  JednostrukaLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIspred(i);
    l.pocetak();
  }
  std::cout << "Dodaj ispred: ";
  for (int i = 0; i < l.brojElemenata(); i++) {
    std::cout << l[i] << " ";
  }
  std::cout << std::endl;
}

void JLtest2() { // test konstruktora bez parametara i metoda dodajIza() i
                 // kraj()
  JednostrukaLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  std::cout << "Dodaj iza: ";
  for (int i = 0; i < l.brojElemenata(); i++)
    std::cout << l[i] << " ";
  std::cout << std::endl;
}

void JLtest3() { // test metoda sljedeci(), prethodni() i trenutni()
  JednostrukaLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  l.pocetak();
  std::cout << "Trenutni element: " << l.trenutni();
  l.sljedeci();
  std::cout << ", sljedeci element: " << l.trenutni();
  l.prethodni();
  std::cout << ", prethodni element: " << l.trenutni();
  std::cout << std::endl;
}

void JLtest4() { // test metoda obrisi() i brojElemenata()
  JednostrukaLista<int> l;
  for (int i = 1; i <= 5; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  std::cout << "Broj elemenata prije brisanja: " << l.brojElemenata();
  l.obrisi();
  std::cout << ", broj elemenata poslije brisanja: " << l.brojElemenata();
  std::cout << std::endl;
}

void JLtest5() { // test operatora dodjele
  JednostrukaLista<int> l;
  for (int i = 1; i <= 3; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  JednostrukaLista<int> temp = l;
  std::cout << "Elementi pocetne liste: ";
  for (int i = 0; i < l.brojElemenata(); i++)
    std::cout << l[i] << " ";
  std::cout << "Elementi kopirane liste: ";
  for (int i = 0; i < temp.brojElemenata(); i++)
    std::cout << temp[i] << " ";
  std::cout << std::endl;
}

void JLtest6() { // test konstruktora kopije
  JednostrukaLista<int> l;
  for (int i = 1; i <= 3; i++) {
    l.dodajIza(i);
    l.kraj();
  }
  JednostrukaLista<int> temp(l);
  std::cout << "Elementi pocetne liste: ";
  for (int i = 0; i < l.brojElemenata(); i++)
    std::cout << l[i] << " ";
  std::cout << "Elementi kopirane liste: ";
  for (int i = 0; i < temp.brojElemenata(); i++)
    std::cout << temp[i] << " ";
  std::cout << std::endl;
}

int main() {
  NLtest1();
  NLtest2();
  NLtest3();
  NLtest4();
  NLtest5();
  NLtest6();
  std::cout << "-------------------------------" << std::endl;
  JLtest1();
  JLtest2();
  JLtest3();
  JLtest4();
  JLtest5();
  JLtest6();
}