#include <ctime>
#include <iostream>
#include <vector>

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

int main() {
  NizMapa<int, int> nm;
  BinStabloMapa<int, int> bsm;
  for (int i = 1; i < 100000; i++) {
    int r = rand() % 100000;
    nm[r];
    bsm[r];
  }
  clock_t nmVrijeme1 = clock();
  nm[10000] = 1;
  clock_t nmVrijeme2 = clock();
  std::cout << "Niz mapa: 1. " << nmVrijeme1 << " 2. " << nmVrijeme2
            << " razlika: " << nmVrijeme2 - nmVrijeme1;
  // pretraga i dodavanje u NizMapa je O(n)
  clock_t bsmVrijeme1 = clock();
  bsm[10000] = 1;
  clock_t bsmVrijeme2 = clock();
  std::cout << "\nBinarno stablo mapa: 1. " << bsmVrijeme1 << " 2. "
            << bsmVrijeme2 << " razlika: " << bsmVrijeme2 - bsmVrijeme1;
  // pretraga i dodavanje u BinStabloMapa je O(log n)
  return 0;
}