#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  Mapa() {}
  virtual ~Mapa() {}
  virtual TipVrijednosti &operator[](const TipKljuca &) = 0;
  virtual TipVrijednosti operator[](const TipKljuca &) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
  int vel;
  const int kapacitet = 10000;
  std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>>> vlista;
  std::function<unsigned int(const TipKljuca &, unsigned int)> f;
  typename std::list<std::pair<TipKljuca, TipVrijednosti>>::iterator
  Pretraga(const TipKljuca &k) {
    if (!f)
      throw "Nije definisana hash funkcija";
    int poz = f(k, vlista.size());
    auto pok = vlista[poz].begin();
    while (pok != vlista[poz].end() && k > pok->first) {
      pok++;
    }
    return pok;
  }
  typename std::list<std::pair<TipKljuca, TipVrijednosti>>::const_iterator
  Pretraga(const TipKljuca &k) const {
    if (!f)
      throw "Nije definisana hash funkcija";
    int poz = f(k, vlista.size());
    auto pok = vlista[poz].begin();
    while (pok != vlista[poz].end() && k > pok->first) {
      pok++;
    }
    return pok;
  }

public:
  HashMapaLan()
      : vlista(kapacitet, std::list<std::pair<TipKljuca, TipVrijednosti>>()) {
    vel = 0;
  }
  int brojElemenata() const { return vel; }
  void obrisi(const TipKljuca &k) {
    auto pok = Pretraga(k);
    int poz = f(k, vlista.size());
    if (pok == vlista[poz].end()) // ako it->first != kljuc mozda
      throw "Kljuc nije pronadjen!";
    auto temp = pok++;
    vlista[poz].erase(temp, pok);
    vel--;
  }
  void obrisi() {
    for (auto &e : vlista) {
      e.clear();
    }
    vel = 0;
  }
  TipVrijednosti &operator[](const TipKljuca &k) {
    auto pok = Pretraga(k);
    int poz = f(k, vlista.size());
    if (pok == vlista[poz].end() || pok->first != k) {
      pok = vlista[poz].insert(
          pok, std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti()));
      vel++;
    }
    return pok->second;
  }
  TipVrijednosti operator[](const TipKljuca &k) const {
    auto pok = Pretraga(k);
    int poz = f(k, vlista.size());
    if (pok == vlista[poz].end() || pok->first != k) {
      return TipVrijednosti();
    }
    return pok->second;
  }
  HashMapaLan<TipKljuca, TipVrijednosti> &
  operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &hm) {
    std::vector<std::list<std::pair<TipKljuca, TipVrijednosti>>> t(hm.vlista);
    vel = hm.vel;
    vlista = hm.vlista;
    f = hm.f;
    return *this;
  }
  void definisiHashFunkciju(
      std::function<unsigned int(const TipKljuca &k, unsigned int)> f) {
    this->f = f;
  }
  ~HashMapaLan() { f = nullptr; }
};

template <typename Tip> class UsmjereniGraf;

template <typename Tip> class Grana;

template <typename Tip> class Cvor;

template <typename Tip> class GranaIterator;

template <typename Tip> class UsmjereniGraf {
protected:
  UsmjereniGraf() {}

public:
  UsmjereniGraf(int a) {}
  virtual ~UsmjereniGraf() {}
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int n) = 0;
  virtual void dodajGranu(int a, int b, float t = 0) = 0;
  virtual void obrisiGranu(int a, int b) = 0;
  virtual bool postojiGrana(int a, int b) const = 0;
  virtual void postaviTezinuGrane(int a, int b, float t = 0) = 0;
  virtual float dajTezinuGrane(int a, int b) const = 0;
  virtual void postaviOznakuCvora(int a, Tip b) = 0;
  virtual Tip dajOznakuCvora(int a) const = 0;
  virtual void postaviOznakuGrane(int a, int b, Tip o) = 0;
  virtual Tip dajOznakuGrane(int a, int b) const = 0;
  virtual Grana<Tip> dajGranu(int a, int b) = 0;
  virtual Cvor<Tip> dajCvor(int a) = 0;
  virtual const Grana<Tip> dajGranu(int a, int b) const = 0;
  virtual const Cvor<Tip> dajCvor(int a) const = 0;
  GranaIterator<Tip> dajGranePocetak() {
    return ++GranaIterator<Tip>(this, 0, -1);
  }
  GranaIterator<Tip> dajGraneKraj() {
    return GranaIterator<Tip>(this, dajBrojCvorova(), 0);
  }
  const GranaIterator<Tip> dajGranePocetak() const {
    return ++GranaIterator<Tip>(const_cast<UsmjereniGraf *>(this), 0, -1);
  }
  const GranaIterator<Tip> dajGraneKraj() const {
    return GranaIterator<Tip>(const_cast<UsmjereniGraf *>(this),
                              dajBrojCvorova(), 0);
  }
};

template <typename Tip> class Grana {
  UsmjereniGraf<Tip> *graf;
  int polazniCvor, dolazniCvor;

public:
  Grana(UsmjereniGraf<Tip> *g, int p, int d) {
    graf = g;
    polazniCvor = p;
    dolazniCvor = d;
  }
  void postaviTezinu(float tezina) {
    graf->postaviTezinuGrane(polazniCvor, dolazniCvor, tezina);
  }
  void postaviOznaku(Tip oznaka) {
    graf->postaviOznakuGrane(polazniCvor, dolazniCvor, oznaka);
  }
  float dajTezinu() const {
    return graf->dajTezinuGrane(polazniCvor, dolazniCvor);
  }
  Tip dajOznaku() const {
    return graf->dajOznakuGrane(polazniCvor, dolazniCvor);
  }
  Cvor<Tip> dajPolazniCvor() { return graf->dajCvor(polazniCvor); }
  const Cvor<Tip> dajPolazniCvor() const { return graf->dajCvor(polazniCvor); }
  Cvor<Tip> dajDolazniCvor() { return graf->dajCvor(dolazniCvor); }
  const Cvor<Tip> dajDolazniCvor() const { return graf->dajCvor(dolazniCvor); }
};

template <typename Tip> class Cvor {
  UsmjereniGraf<Tip> *graf;
  int indeks;

public:
  Cvor(UsmjereniGraf<Tip> *g, int i) {
    graf = g;
    indeks = i;
  }
  Tip dajOznaku() const { return graf->dajOznakuCvora(indeks); }
  void postaviOznaku(Tip oznaka) { graf->postaviOznakuCvora(indeks, oznaka); }
  int dajRedniBroj() const { return indeks; }
};

template <typename Tip> class GranaIterator {
  UsmjereniGraf<Tip> *graf;
  int i, j;

public:
  GranaIterator(UsmjereniGraf<Tip> *g, int a, int b) {
    graf = g;
    i = a;
    j = b;
  }
  Grana<Tip> operator*() { return graf->dajGranu(i, j); }
  const Grana<Tip> operator*() const { return graf->dajGranu(i, j); }
  GranaIterator<Tip> operator++(int a) {
    auto temp = *this;
    ++(*this);
    return temp;
  }
  GranaIterator<Tip> &operator++() {
    do {
      if (j + 1 >= graf->dajBrojCvorova()) {
        j = 0;
        i++;
      } else {
        j++;
      }
    } while (i < graf->dajBrojCvorova() && graf->postojiGrana(i, j) == false);
    return *this;
  }
  bool operator==(const GranaIterator &g) const {
    if (i == g.i && j == g.j && graf == g.graf)
      return true;
    return false;
  }
  bool operator!=(const GranaIterator &g) const { return !(*this == g); }
};

template <typename Tip> class ListaGraf : public UsmjereniGraf<Tip> {
  struct El {
    int cvor;
    Tip oznaka;
    float tezinaCvora;
    El(int c, float t = 0, Tip o = Tip()) {
      cvor = c;
      oznaka = o;
      tezinaCvora = t;
    }
  };
  std::vector<Tip> listaOznaka;
  std::vector<std::list<El>> listaSusjednih;
  void validirajCvor(int i) const {
    if (i < 0 || i >= listaSusjednih.size())
      throw "Neispravan cvor";
  }
  void validirajGranu(int a, int b) const {
    validirajCvor(a);
    validirajCvor(b);
  }
  void validirajPostojanjeGrane(int a, int b) const {
    validirajGranu(a, b);
    if (postojiGrana(a, b) == false)
      throw "Grana ne postoji";
  }
  typename std::list<El>::iterator pronadji(int a, int b) {
    typename std::list<El>::iterator pok;
    for (pok = listaSusjednih[a].begin(); pok != listaSusjednih[a].end();
         pok++) {
      if (b == pok->cvor)
        return pok;
      else if (b < pok->cvor)
        return listaSusjednih[a].end();
    }
    return pok;
  }
  typename std::list<El>::const_iterator pronadji(int a, int b) const {
    typename std::list<El>::const_iterator pok;
    for (pok = listaSusjednih[a].begin(); pok != listaSusjednih[a].end();
         pok++) {
      if (b == pok->cvor)
        return pok;
      else if (b < pok->cvor)
        return listaSusjednih[a].end();
    }
    return pok;
  }

public:
  ListaGraf(int n) {
    listaSusjednih = std::vector<std::list<El>>(n, std::list<El>());
    listaOznaka = std::vector<Tip>(n);
  }
  void postaviBrojCvorova(int n) {
    listaSusjednih.resize(n);
    listaOznaka.resize(n);
  }
  int dajBrojCvorova() const { return listaSusjednih.size(); }
  void dodajGranu(int a, int b, float n = 0) {
    validirajGranu(a, b);
    typename std::list<El>::iterator pok = listaSusjednih[a].begin();
    while (pok != listaSusjednih[a].end() && b > pok->cvor)
      pok++;
    listaSusjednih[a].insert(pok, El(b, n));
  }
  void obrisiGranu(int a, int b) {
    validirajPostojanjeGrane(a, b);
    listaSusjednih[a].erase(pronadji(a, b));
  }
  bool postojiGrana(int a, int b) const {
    validirajGranu(a, b);
    return pronadji(a, b) != listaSusjednih[a].end();
  }
  void postaviOznakuCvora(int a, Tip o) {
    validirajCvor(a);
    listaOznaka[a] = o;
  }
  Tip dajOznakuCvora(int a) const {
    validirajCvor(a);
    return listaOznaka[a];
  }
  void postaviOznakuGrane(int a, int b, Tip o) {
    validirajPostojanjeGrane(a, b);
    pronadji(a, b)->oznaka = o;
  }
  Tip dajOznakuGrane(int a, int b) const {
    validirajPostojanjeGrane(a, b);
    return pronadji(a, b)->oznaka;
  }
  void postaviTezinuGrane(int a, int b, float n) {
    validirajPostojanjeGrane(a, b);
    pronadji(a, b)->tezinaCvora = n;
  }
  float dajTezinuGrane(int a, int b) const {
    validirajPostojanjeGrane(a, b);
    return pronadji(a, b)->tezinaCvora;
  }
  Cvor<Tip> dajCvor(int a) {
    validirajCvor(a);
    return Cvor<Tip>(this, a);
  }
  const Cvor<Tip> dajCvor(int a) const {
    validirajCvor(a);
    return Cvor<Tip>(const_cast<ListaGraf *>(this), a);
  }
  Grana<Tip> dajGranu(int a, int b) {
    validirajPostojanjeGrane(a, b);
    return Grana<Tip>(this, a, b);
  }
  const Grana<Tip> dajGranu(int a, int b) const {
    validirajPostojanjeGrane(a, b);
    return Grana<Tip>(const_cast<ListaGraf *>(this), a, b);
  }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *g, std::vector<Cvor<TipOznake>> &v,
         Cvor<TipOznake> pocetni) {
  pocetni.postaviOznaku(1);
  v.push_back(pocetni);
  std::queue<Cvor<TipOznake>> q;
  q.push(pocetni);
  while (q.empty() == false) {
    Cvor<TipOznake> temp = q.front();
    q.pop();
    for (GranaIterator<TipOznake> i = g->dajGranePocetak();
         i != g->dajGraneKraj(); ++i) {
      Cvor<TipOznake> sljedeci = (*i).dajDolazniCvor();
      if ((*i).dajPolazniCvor().dajRedniBroj() == temp.dajRedniBroj() &&
          sljedeci.dajOznaku() != 1) {
        sljedeci.postaviOznaku(1);
        v.push_back(sljedeci);
        q.push(sljedeci);
      }
    }
  }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *g, std::vector<Cvor<TipOznake>> &v,
         Cvor<TipOznake> pocetni) {
  pocetni.postaviOznaku(1);
  v.push_back(pocetni);
  for (GranaIterator<TipOznake> i = g->dajGranePocetak();
       i != g->dajGraneKraj(); ++i) {
    Cvor<TipOznake> sljedeci = (*i).dajDolazniCvor();
    if ((*i).dajPolazniCvor().dajRedniBroj() == pocetni.dajRedniBroj() &&
        sljedeci.dajOznaku() != 1) {
      dfs(g, v, sljedeci);
    }
  }
}

int main() {
  UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
  g->dodajGranu(0, 1, 2.5);
  g->dodajGranu(1, 2, 1.2);
  g->dodajGranu(1, 3, 0.1);
  g->dodajGranu(3, 3, -4.0);
  g->dodajGranu(2, 4, 3.14);
  g->dodajGranu(2, 3, 2.73);
  g->dodajGranu(3, 5, 1);
  g->dodajGranu(5, 2, -5.0);
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 6; j++)
      cout << g->postojiGrana(i, j);
  delete g;
}