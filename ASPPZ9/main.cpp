#include <ctime>
#include <functional>
#include <iostream>
#include <string>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  Mapa() {}
  virtual ~Mapa() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
  virtual TipVrijednosti operator[](TipKljuca kljuc) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  std::pair<TipKljuca, TipVrijednosti> *nm = nullptr;
  int kapacitet, duzina;

public:
  NizMapa() {
    nm = new std::pair<TipKljuca, TipVrijednosti>[100];
    kapacitet = 100, duzina = 0;
  }
  ~NizMapa() { delete[] nm; }
  int brojElemenata() const override { return duzina; }
  void obrisi() override { duzina = 0; }
  void obrisi(const TipKljuca &k) override {
    bool pronadjen = false;
    for (int i = 0; i < duzina; i++) {
      if (nm[i].first == k) {
        pronadjen = true;
        for (int j = i; j < duzina - 1; j++)
          nm[j] = nm[j + 1];
        duzina--;
      }
    }
    if (pronadjen == false) {
      throw "Uneseni par ne postoji!";
    }
  }
  TipVrijednosti operator[](TipKljuca k) const override {
    for (int i = 0; i < duzina; i++)
      if (nm[i].first == k)
        return nm[i].second;
    return TipVrijednosti();
  }
  TipVrijednosti &operator[](TipKljuca k) override {
    for (int i = 0; i < duzina; i++)
      if (nm[i].first == k)
        return nm[i].second;
    if (duzina < kapacitet) {
      nm[duzina].first = k, nm[duzina].second = TipVrijednosti();
      duzina++;
      return nm[duzina - 1].second;
    } else {
      kapacitet *= 2;
      std::pair<TipKljuca, TipVrijednosti> *t =
          new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
      for (int i = 0; i < duzina; i++)
        t[i] = nm[i];
      t[duzina].first = k, t[duzina].second = TipVrijednosti();
      duzina++;
      delete[] nm;
      nm = t;
      t = nullptr;
      delete[] t;
      return nm[duzina - 1].second;
    }
  }
  NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &t) {
    duzina = t.duzina, kapacitet = t.kapacitet;
    nm = new std::pair<TipKljuca, TipVrijednosti>[t.kapacitet];
    for (int i = 0; i < duzina; i++)
      nm[i] = t.nm[i];
  }
  NizMapa &operator=(const NizMapa<TipKljuca, TipVrijednosti> &t) {
    if (&t == this)
      return *this;
    duzina = t.duzina, kapacitet = t.kapacitet;
    delete[] nm;
    nm = new std::pair<TipKljuca, TipVrijednosti>[t.kapacitet];
    for (int i = 0; i < duzina; i++)
      nm[i] = t.nm[i];
    return *this;
  }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:
  struct Cvor {
    Cvor *d;
    Cvor *l;
    Cvor *rod;
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
  };
  Cvor *korijen;
  int vel;

  void prekopiraj(BinStabloMapa<TipKljuca, TipVrijednosti> &bsm, Cvor *c) {
    if (c != nullptr) {
      bsm[c->kljuc] = c->vrijednost;
      prekopiraj(bsm, c->l);
      prekopiraj(bsm, c->d);
    }
  }
  void BrisiStablo(Cvor *c) {
    if (c != nullptr) {
      BrisiStablo(c->d);
      BrisiStablo(c->l);
      delete c;
      c = nullptr;
    }
  }
  TipVrijednosti &UbaciuStablo(const TipKljuca &kljuc) {
    Cvor *p = korijen;
    Cvor *pRod = nullptr;
    while (p != nullptr) {
      pRod = p;
      if (kljuc > p->kljuc)
        p = p->d;
      else
        p = p->l;
    }
    Cvor *temp = new Cvor{nullptr, nullptr, nullptr, kljuc, TipVrijednosti()};
    if (pRod == nullptr)
      korijen = temp;
    else {
      if (kljuc < pRod->kljuc)
        pRod->l = temp;
      else
        pRod->d = temp;
    }
    vel++;
    return temp->vrijednost;
  }

public:
  BinStabloMapa() { korijen = nullptr, vel = 0; }
  BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &bsm) {
    korijen = nullptr;
    vel = 0;
    prekopiraj(*this, bsm.korijen);
  }
  ~BinStabloMapa() {
    BrisiStablo(korijen);
    vel = 0;
    korijen = nullptr;
  }
  int brojElemenata() const { return vel; }
  void obrisi() {
    BrisiStablo(korijen);
    vel = 0;
    korijen = nullptr;
  }
  void obrisi(const TipKljuca &kljuc) {
    Cvor *temp = korijen;
    Cvor *noviRod = nullptr;
    while (temp != nullptr && kljuc != temp->kljuc) {
      noviRod = temp;
      if (temp->kljuc <= kljuc)
        temp = temp->d;
      else
        temp = temp->l;
    }
    if (temp == nullptr)
      throw "Ne postoji element u stablu sa unesenim kljucem!";
    Cvor *t = nullptr;
    Cvor *tRod = nullptr;
    if (temp->l == nullptr) {
      t = temp->d;
    } else {
      if (temp->d == nullptr)
        t = temp->l;
      else {
        tRod = temp;
        t = temp->l;
        Cvor *provjera = t->d;
        while (provjera != nullptr) {
          tRod = t;
          t = t->d;
          provjera = t->d;
        }
        if (tRod != temp) {
          tRod->d = t->l;
          t->l = temp->l;
        }
        t->d = temp->d;
      }
    }
    if (noviRod == nullptr)
      korijen = t;
    else {
      if (temp == noviRod->l)
        noviRod->l = t;
      else
        noviRod->d = t;
    }
    vel--;
    delete temp;
  }
  BinStabloMapa<TipKljuca, TipVrijednosti> &
  operator=(BinStabloMapa<TipKljuca, TipVrijednosti> bsm) {
    std::swap(korijen, bsm.korijen);
    std::swap(vel, bsm.vel);
    return *this;
  }
  TipVrijednosti operator[](TipKljuca kljuc) const {
    Cvor *p = korijen;
    while (p != nullptr && p->kljuc != kljuc) {
      if (kljuc < p->kljuc) {
        p = p->l;
      } else {
        p = p->d;
      }
    }
    if (p != nullptr)
      return p->vrijednost;
    return TipVrijednosti();
  }

  TipVrijednosti &operator[](TipKljuca kljuc) {
    Cvor *p = korijen;
    while (p != nullptr && p->kljuc != kljuc) {
      if (kljuc < p->kljuc) {
        p = p->l;
      } else {
        p = p->d;
      }
    }
    if (p != nullptr)
      return p->vrijednost;
    return UbaciuStablo(kljuc);
  }
};

template <typename TipKljuca, typename TipVrijednosti> class Pair {
public:
  TipKljuca k;
  TipVrijednosti v;
  bool b;
  Pair() { b = false; }
  Pair(const TipKljuca &kljuc, const TipVrijednosti &vrijednost,
       bool brisanje = false) {
    k = kljuc;
    v = vrijednost;
    b = brisanje;
  }
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
  int vel, kapacitet;
  Pair<TipKljuca, TipVrijednosti> **niz, **kraj;
  function<unsigned int(const TipKljuca &, unsigned int)> h;
  unsigned int HesLin(const TipKljuca &k, unsigned int i) const {
    return (h(k, kapacitet) + i) % kapacitet;
  }
  int umetniHash(const TipKljuca &kljuc) {
    int br = 0;
    int i;
    do {
      i = HesLin(kljuc, br);
      if (niz[i] == nullptr || niz[i]->b == true) {
        if (niz[i] != nullptr)
          delete niz[i];
        niz[i] = new Pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
        vel++;
        return i;
      } else {
        br++;
      }
    } while (br < kapacitet);
    return -1;
  }
  int traziHash(const TipKljuca &kljuc) const {
    unsigned int poz, br = 0;
    do {
      poz = HesLin(kljuc, br);
      if (niz[poz] && !niz[poz]->b && niz[poz]->k == kljuc) {
        return poz;
      } else {
        br++;
      }
    } while (niz[poz] != nullptr && br != kapacitet);
    return -1;
  }
  bool obrisiHash(const TipKljuca &kljuc) {
    int i = traziHash(kljuc);
    if (i != -1) {
      vel--;
      niz[i]->b = true;
      return true;
    }
    return false;
  }
  void povecaj() {
    Pair<TipKljuca, TipVrijednosti> **t =
        new Pair<TipKljuca, TipVrijednosti> *[vel * 2] {};
    Pair<TipKljuca, TipVrijednosti> **p1, **p2;
    for (p1 = niz, p2 = t; p1 != kraj; p1++) {
      if (*p1 != nullptr) {
        *p2++ = new Pair<TipKljuca, TipVrijednosti>(**p1++);
      }
    }
    int novaVel = vel * 2;
    obrisi(); // provjerit kasno povezivanje
    delete[] niz;
    kapacitet = novaVel;
    vel = novaVel;
    kraj = p2;
    niz = t;
  }

public:
  HashMapa()
      : vel(0), kapacitet(1000000),
        niz(new Pair<TipKljuca, TipVrijednosti> *[kapacitet] {}),
        kraj(niz + kapacitet) {}
  HashMapa(const HashMapa &hm) {
    vel = hm.vel;
    kapacitet = hm.kapacitet;
    niz = new Pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
    kraj = (niz + kapacitet);
    h = hm.h;
    for (int i = 0; i < hm.kapacitet; i++) {
      if (hm.niz[i]) {
        niz[i] = new Pair<TipKljuca, TipVrijednosti>(*hm.niz[i]);
      }
    }
  }
  HashMapa(HashMapa &&hm) {
    vel = hm.vel;
    kapacitet = hm.kapacitet;
    niz = hm.niz;
    kraj = hm.kraj;
    h = hm.h;
    hm.niz = nullptr;
    hm.kraj = nullptr;
  }
  int brojElemenata() const { return vel; }
  void obrisi(const TipKljuca &k) {
    if (obrisiHash(k) == false) {
      throw "Kljuc nije pronadjen";
    }
  }
  void obrisi() {
    Pair<TipKljuca, TipVrijednosti> **temp;
    for (temp = niz; temp != kraj; temp++) {
      delete *temp;
      *temp = nullptr;
    }
    vel = 0;
  }
  ~HashMapa() {
    obrisi();
    delete[] niz;
  }
  HashMapa &operator=(const HashMapa &hm) {
    if (this == &hm)
      return *this;
    obrisi();
    delete[] niz;
    vel = hm.vel;
    kapacitet = hm.kapacitet;
    niz = new Pair<TipKljuca, TipVrijednosti> *[kapacitet] {};
    kraj = niz + kapacitet;
    h = hm.h;
    for (int i = 0; i < hm.kapacitet; i++) {
      if (hm.niz[i]) {
        niz[i] = new Pair<TipKljuca, TipVrijednosti>(*hm.niz[i]);
      }
    }
    return *this;
  }
  HashMapa &operator=(HashMapa &&hm) {
    if (this == &hm)
      return *this;
    obrisi();
    delete[] niz;
    vel = hm.vel;
    kapacitet = hm.kapacitet;
    niz = hm.niz;
    kraj = hm.kraj;
    h = hm.h;
    hm.niz = nullptr;
    hm.kraj = nullptr;
    return *this;
  }
  TipVrijednosti &operator[](TipKljuca k) {
    if (!h)
      throw "Nije definisana hash funkcija.";
    int poz = traziHash(k);
    if (poz != -1) {
      return niz[poz]->v;
    } else {
      return niz[umetniHash(k)]->v;
    }
  }
  TipVrijednosti operator[](TipKljuca k) const {
    if (!h)
      throw "Nije definisana hash funkcija.";
    int poz = traziHash(k);
    if (poz != -1) {
      return niz[poz]->v;
    } else {
      return TipVrijednosti();
    }
  }
  void definisiHashFunkciju(
      function<unsigned int(const TipKljuca &, unsigned int)> h) {
    this->h = h;
  }
};

unsigned int hashfunkcija(int input, unsigned int max) { return input % max; }

int main() {
  NizMapa<int, int> nm;
  BinStabloMapa<int, int> bsm;
  HashMapa<int, int> hm;
  hm.definisiHashFunkciju(hashfunkcija);
  clock_t nmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    nm[i] = rand();
  }
  clock_t nmt2 = clock();
  std::cout << "Vrijeme upisivanja za NizMapu: "
            << (nmt2 - nmt1) / (CLOCKS_PER_SEC / 1000.);
  clock_t bsmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    bsm[i] = rand();
  }
  clock_t bsmt2 = clock();
  std::cout << "\nVrijeme upisivanja za BinStabloMapu: "
            << (bsmt2 - bsmt1) / (CLOCKS_PER_SEC / 1000.);
  clock_t hmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    hm[i] = rand();
  }
  clock_t hmt2 = clock();
  std::cout << "\nVrijeme upisivanja za HashMapu: "
            << (hmt2 - hmt1) / (CLOCKS_PER_SEC / 1000.);

  nmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    int p = nm[i];
  }
  nmt2 = clock();
  std::cout << "Vrijeme pristupa za NizMapu: "
            << (nmt2 - nmt1) / (CLOCKS_PER_SEC / 1000.);
  bsmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    int p = bsm[i];
  }
  bsmt2 = clock();
  std::cout << "\nVrijeme pristupa za BinStabloMapu: "
            << (bsmt2 - bsmt1) / (CLOCKS_PER_SEC / 1000.);
  hmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    int p = hm[i];
  }
  hmt2 = clock();
  std::cout << "\nVrijeme pristupa za HashMapu: "
            << (hmt2 - hmt1) / (CLOCKS_PER_SEC / 1000.);

  nmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    nm.obrisi(i);
  }
  nmt2 = clock();
  std::cout << "Vrijeme brisanja za NizMapu: "
            << (nmt2 - nmt1) / (CLOCKS_PER_SEC / 1000.);
  bsmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    bsm.obrisi(i);
  }
  bsmt2 = clock();
  std::cout << "\nVrijeme brisanja za BinStabloMapu: "
            << (bsmt2 - bsmt1) / (CLOCKS_PER_SEC / 1000.);
  hmt1 = clock();
  for (int i = 0; i < 10000; i++) {
    hm.obrisi(i);
  }
  hmt2 = clock();
  std::cout << "\nVrijeme brisanja za HashMapu: "
            << (hmt2 - hmt1) / (CLOCKS_PER_SEC / 1000.);
}