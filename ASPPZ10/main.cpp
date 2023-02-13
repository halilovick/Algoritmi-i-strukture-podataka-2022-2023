#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

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

template <typename Tip> class MatricaGraf : public UsmjereniGraf<Tip> {
  std::vector<std::vector<float>> v;
  std::vector<std::vector<Tip>> oznakeGrana;
  std::vector<Tip> oznakeCvorova;
  void validirajCvor(int i) const {
    if (i < 0 || i >= v.size())
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
  const float max = std::numeric_limits<float>::max();

public:
  MatricaGraf(int n) {
    if (n < 0)
      throw "Nevalidan broj cvorova";
    v = std::vector<std::vector<float>>(n, std::vector<float>(n, max));
    oznakeGrana = std::vector<std::vector<Tip>>(n, std::vector<Tip>(n));
    oznakeCvorova = std::vector<Tip>(n);
  }
  void postaviBrojCvorova(int n) {
    if (n < dajBrojCvorova())
      throw "Novi broj manji od trenutnog!";
    v.resize(n);
    for (auto &t : v)
      t.resize(n, max);
    oznakeGrana.resize(n);
    oznakeCvorova.resize(n);
    for (auto &t : oznakeGrana)
      t.resize(n);
  }
  int dajBrojCvorova() const { return v.size(); }
  void dodajGranu(int a, int b, float n = 0) {
    validirajGranu(a, b);
    v[a][b] = n;
  }
  void obrisiGranu(int a, int b) {
    validirajPostojanjeGrane(a, b);
    v[a][b] = max;
  }
  bool postojiGrana(int a, int b) const {
    validirajGranu(a, b);
    if (v[a][b] != max)
      return true;
    return false;
  }
  void postaviOznakuCvora(int a, Tip o) {
    validirajCvor(a);
    oznakeCvorova[a] = o;
  }
  Tip dajOznakuCvora(int a) const {
    validirajCvor(a);
    return oznakeCvorova[a];
  }
  void postaviOznakuGrane(int a, int b, Tip o) {
    validirajPostojanjeGrane(a, b);
    oznakeGrana[a][b] = o;
  }
  Tip dajOznakuGrane(int a, int b) const {
    validirajPostojanjeGrane(a, b);
    return oznakeGrana[a][b];
  }
  void postaviTezinuGrane(int a, int b, float n) {
    validirajPostojanjeGrane(a, b);
    v[a][b] = n;
  }
  float dajTezinuGrane(int a, int b) const {
    validirajPostojanjeGrane(a, b);
    return v[a][b];
  }
  Cvor<Tip> dajCvor(int a) {
    validirajCvor(a);
    return Cvor<Tip>(this, a);
  }
  const Cvor<Tip> dajCvor(int a) const {
    validirajCvor(a);
    return Cvor<Tip>(const_cast<MatricaGraf *>(this), a); 
  }
  Grana<Tip> dajGranu(int a, int b) {
    validirajPostojanjeGrane(a, b);
    return Grana<Tip>(this, a, b);
  }
  const Grana<Tip> dajGranu(int a, int b) const {
    validirajPostojanjeGrane(a, b);
    return Grana<Tip>(const_cast<MatricaGraf *>(this), a, b);
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
      dfs(g, &v, sljedeci);
    }
  }
}

int main() {
  UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
  g->dodajGranu(0, 1, 2.5);
  g->dodajGranu(1, 0, 1.2);
  g->dodajGranu(1, 2, 0.1);
  g->dodajGranu(0, 0, 3.14);
  for (GranaIterator<bool> iter = g->dajGranePocetak();
       iter != g->dajGraneKraj(); ++iter)
    cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
         << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
         << (*iter).dajTezinu() << "; ";
  delete g;
}