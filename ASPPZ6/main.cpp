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
  virtual TipVrijednosti &operator[](const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti operator[](const TipKljuca &kljuc) const = 0;
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
  int brojElemenata() const { return duzina; }
  void obrisi() { duzina = 0; }
  void obrisi(const TipKljuca &k) {
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
  TipVrijednosti operator[](const TipKljuca &k) const {
    for (int i = 0; i < duzina; i++)
      if (nm[i].first == k)
        return nm[i].second;
    return TipVrijednosti();
  }
  TipVrijednosti &operator[](const TipKljuca &k) {
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

void test1() { // test metode brojElemenata uz tipove <string, int>
  NizMapa<std::string, int> m;
  m["BiH"] = 387;
  m["Srbija"] = 381;
  m["Hrvatska"] = 385;
  if (m.brojElemenata() == 3)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test2() { // test metoda obrisi uz tipove <int, double>
  NizMapa<int, double> m;
  m[1] = 0.1;
  m[2] = 0.2;
  m[10] = 1.0;
  if (m.brojElemenata() == 3) {
    m.obrisi(1);
    if (m.brojElemenata() == 2) {
      m.obrisi();
      if (m.brojElemenata() == 0)
        std::cout << "OK";
      else
        std::cout << "NOK";
    } else
      std::cout << "NOK";
  } else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test3() { // test poređenja uz tipove <double, vector>
  NizMapa<double, std::vector<int>> m;
  m[7] = {1, 1, 1};
  std::vector<int> v{1, 1, 1};
  if (m[7] == v)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test4() { // test operatora dodjele uz tipove <bool, int>
  NizMapa<bool, int> m;
  m[true] = 1;
  m[false] = 0;
  m[false] = -1;
  NizMapa<bool, int> temp = m;
  if (m.brojElemenata() == temp.brojElemenata())
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test5() { // test konstruktora kopije uz tipove <string, string>
  NizMapa<std::string, std::string> m;
  m["BiH"] = "Sarajevo";
  m["Austrija"] = "Bec";
  m["Belgija"] = "Brisel";
  NizMapa<std::string, std::string> temp(m);
  if (m.brojElemenata() == temp.brojElemenata())
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
}