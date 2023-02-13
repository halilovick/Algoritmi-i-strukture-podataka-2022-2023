#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  Mapa() {}
  virtual ~Mapa() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti &operator[](const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti operator[](const TipKljuca &kljuc) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
  class Cvor {
  public:
    int balans = 0;
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *roditelj;
    Cvor *lijevo;
    Cvor *desno;
    Cvor() {
      lijevo = nullptr;
      desno = nullptr;
      roditelj = nullptr;
    }
    Cvor(const TipKljuca &k, const TipVrijednosti &v, Cvor *l = nullptr,
         Cvor *d = nullptr, Cvor *r = nullptr) {
      kljuc = k;
      vrijednost = v;
      roditelj = r;
      lijevo = l;
      desno = d;
    }
  };
  Cvor *korijen;
  int vel;

public:
  AVLStabloMapa() : korijen(nullptr) { vel = 0; }
  AVLStabloMapa(const AVLStabloMapa &avl) {
    korijen = nullptr;
    vel = 0;
    prekopirajStablo(korijen, avl.korijen, nullptr);
  }
  void prekopirajStablo(Cvor *&c1, Cvor *c2, Cvor *r) {
    if (c2 != nullptr) {
      c1 = new Cvor(c2->kljuc, c2->vrijednost, nullptr, nullptr, r);
      prekopirajStablo(c1->lijevo, c2->lijevo, c1);
      prekopirajStablo(c1->desno, c2->desno, c1);
      vel++;
    }
  }
  int brojElemenata() const { return vel; }
  Cvor *dodaj(Cvor *&c, const TipKljuca &k, Cvor *r) {
    if (c == nullptr) {
      vel++;
      c = new Cvor(k, TipVrijednosti(), nullptr, nullptr, r);
      return c;
    } else if (c->kljuc > k) {
      return dodaj(c->lijevo, k, c);
    } else {
      return dodaj(c->desno, k, c);
    }
  }
  void obrisiSve(Cvor *c) {
    if (c == nullptr)
      return;
    obrisiSve(c->desno);
    obrisiSve(c->lijevo);
    delete c;
  }
  void obrisi() {
    obrisiSve(korijen);
    vel = 0;
    korijen = nullptr;
  }
  void obrisi(const TipKljuca &k) {
    Cvor *t = korijen;
    Cvor *pomocni = nullptr;
    while (t != nullptr && t->kljuc != k) {
      pomocni = t;
      if (k < t->kljuc)
        t = t->lijevo;
      else
        t = t->desno;
    }
    if (t == nullptr)
      throw;
    Cvor *pom1 = nullptr, *pom2 = nullptr, *temp = nullptr;
    if (t->desno == nullptr)
      pom1 = t->lijevo;
    else if (t->lijevo == nullptr)
      pom1 = t->desno;
    else {
      pom2 = t;
      pom1 = t->lijevo;
      temp = pom1->desno;
      while (temp != nullptr) {
        pom2 = pom1;
        pom1 = temp;
        temp = pom1->desno;
      }
      if (pom2 != t) {
        pom2->desno = pom1->lijevo;
        pom1->lijevo = t->lijevo;
        pom2->roditelj = pom1;
        pom1->roditelj = pomocni;
        if (pom2->desno) {
          pom2->desno->roditelj = pom2;
        }
      }
      pom1->desno = t->desno;
      t->desno->roditelj = pom1;
    }
    if (pomocni == nullptr) {
      korijen = pom1;
      if (korijen) {
        korijen->roditelj = nullptr;
      }
    } else if (t == pomocni->lijevo) {
      pomocni->lijevo = pom1;
      if (pom1)
        pom1->roditelj = pomocni;
    } else {
      pomocni->desno = pom1;
      if (pom1)
        pom1->roditelj = pomocni;
    }
    if (t->roditelj && t->roditelj->lijevo == t) {
      t->roditelj->balans--;
    } else if (t->roditelj) {
      t->roditelj->balans++;
    }
    delete t;
    vel--;
  }
  void umetni(Cvor *&c) {
    Cvor *k = korijen;
    Cvor *pomocni = nullptr;
    while (k != nullptr) {
      pomocni = k;
      if (c->kljuc < k->kljuc) {
        k = k->lijevo;
      } else {
        k = k->desno;
      }
    }
    bool postoji = false;
    if (pomocni == nullptr) {
      postoji = true;
      korijen = c;
      vel++;
    } else {
      if (c->kljuc < pomocni->kljuc) {
        pomocni->lijevo = c;
        c->roditelj = pomocni;
        vel++;
        if (pomocni->desno) {
          postoji = true;
          pomocni->balans = 0;
        }
      } else {
        pomocni->desno = c;
        c->roditelj = pomocni;
        vel++;
        if (pomocni->lijevo) {
          postoji = true;
          pomocni->balans = 0;
        }
      }
    }
    if (postoji == false) {
      azuriranjeBalansa(c);
    }
  }
  Cvor *pretraga(Cvor *c, const TipKljuca &k) const {
    if (c == nullptr || c->kljuc == k)
      return c;
    else if (k < c->kljuc)
      return pretraga(c->lijevo, k);
    else
      return pretraga(c->desno, k);
  }
  void rotirajDesno(Cvor *&c) {
    Cvor *temp;
    if (c->roditelj && c->roditelj->lijevo == c) {
      c->roditelj->lijevo = c->lijevo;
    } else if (c->roditelj) {
      c->roditelj->desno = c->lijevo;
    }
    temp = c->lijevo;
    bool postoji = false;
    if (c->lijevo->desno) {
      postoji = true;
      c->lijevo = temp->desno;
      temp->desno->roditelj = c;
      temp->desno = nullptr;
    }
    temp->roditelj = c->roditelj;
    if (c->roditelj) {
      c->roditelj = temp;
    } else {
      c->roditelj = temp;
      korijen = temp;
    }
    temp->desno = c;
    temp->balans = c->balans = 0;
    if (postoji == false)
      c->lijevo = nullptr;
  }
  void rotirajLijevo(Cvor *&c) {
    Cvor *temp;
    if (c->roditelj && c->roditelj->desno == c) {
      c->roditelj->desno = c->desno;
    } else if (c->roditelj) {
      c->roditelj->lijevo = c->desno;
    }
    temp = c->desno;
    bool postoji = false;
    if (c->desno->lijevo) {
      postoji = true;
      c->desno = temp->lijevo;
      temp->lijevo->roditelj = c;
      temp->lijevo = nullptr;
    }
    temp->roditelj = c->roditelj;
    if (c->roditelj) {
      c->roditelj = temp;
    } else {
      c->roditelj = temp;
      korijen = temp;
    }
    temp->lijevo = c;
    temp->balans = c->balans = 0;
    if (postoji == false)
      c->desno = nullptr;
  }
  void azuriranjeBalansa(Cvor *&c) {
    if (c->roditelj == nullptr)
      return;
    if (!(c->lijevo && c->desno && c->balans == 0)) {
      if (c->roditelj->lijevo == c)
        c->roditelj->balans++;
      else
        c->roditelj->balans--;
    }
    bool azur = true;
    Cvor *c1 = c->roditelj;
    Cvor *c2 = c;
    if (c->balans > 0 && c->roditelj->balans < -1) {
      rotirajDesno(c2);
      rotirajLijevo(c1);
      azur = false;
    } else if (c->balans < 0 && c->roditelj->balans > 1) {
      rotirajLijevo(c2);
      rotirajDesno(c1);
      azur = false;
    } else if (c->roditelj->balans < -1) {
      rotirajLijevo(c1);
      azur = false;
    } else if (c->roditelj->balans > 1) {
      rotirajDesno(c1);
      azur = false;
    }
    if (azur == true)
      azuriranjeBalansa(c->roditelj);
  }
  TipVrijednosti operator[](const TipKljuca &k) const {
    Cvor *t = pretraga(this->korijen, k);
    if (t != nullptr)
      return t->vrijednost;
    return TipVrijednosti();
  }
  TipVrijednosti &operator[](const TipKljuca &k) {
    Cvor *t = pretraga(this->korijen, k);
    if (t != nullptr)
      return t->vrijednost;
    Cvor *pomocni = new Cvor(k, TipVrijednosti{});
    this->umetni(pomocni);
    return pomocni->vrijednost;
  }
  AVLStabloMapa &operator=(const AVLStabloMapa &avl) {
    if (this == &avl)
      return *this;
    obrisiSve(korijen);
    korijen = nullptr;
    vel = 0;
    prekopirajStablo(korijen, avl.korijen, nullptr);
    return *this;
  }
  ~AVLStabloMapa() { obrisiSve(korijen); }
};

int main() {}