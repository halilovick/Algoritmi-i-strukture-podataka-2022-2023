#include <iostream>
#include <vector>

using namespace std;

template <typename Tip> class Stek {
  Tip *stek;
  int kapacitet, top;

public:
  Stek() : kapacitet(100), top(-1) { stek = new Tip[kapacitet]; }
  ~Stek() { delete[] stek; }
  Stek(const Stek &s) : kapacitet(s.kapacitet), top(s.top) {
    stek = new Tip[kapacitet];
    for (int i = 0; i <= top; i++)
      stek[i] = s.stek[i];
  }
  Stek &operator=(const Stek &s) {
    if (this == &s)
      return *this;
    delete[] stek;
    kapacitet = s.kapacitet;
    top = s.top;
    stek = new Tip[kapacitet];
    for (int i = 0; i <= top; i++) {
      stek[i] = s.stek[i];
    }
    return *this;
  }
  void brisi() { top = -1; }
  void stavi(const Tip &el) {
    if (brojElemenata() == kapacitet) {
      kapacitet *= 2;
      Tip *t = stek;
      stek = new Tip[kapacitet];
      for (int i = 0; i < brojElemenata(); i++)
        stek[i] = t[i];
      delete[] t;
    }
    stek[++top] = el;
  }
  Tip skini() {
    if (top == -1)
      throw "Stek je prazan!";
    return stek[top--];
  }
  Tip &vrh() {
    if (top == -1)
      throw "Stek je prazan!";
    return stek[top];
  }
  int brojElemenata() { return top + 1; }
};

void pretraga(Stek<vector<int>> &s, int trazeni) {
  if (s.brojElemenata() == 0) {
    std::cout << "Nema elementa";
    return;
  } else {
    std::vector<int> temp = s.skini();
    if (temp.empty() || trazeni < temp[0])
      pretraga(s, trazeni);
    else if (trazeni > temp[temp.size() - 1])
      std::cout << "Nema elementa";
    else {
      int dno = 0, vrh = temp.size() - 1;
      bool nadjen = false;
      while (nadjen == false && vrh >= dno) {
        int srednji = (vrh + dno) / 2;
        if (temp[srednji] == trazeni) {
          std::cout << srednji << " " << s.brojElemenata();
          nadjen = true;
        } else if (temp[srednji] <= trazeni)
          dno = srednji + 1;
        else
          vrh = srednji - 1;
      }
      if (nadjen == false)
        std::cout << "Nema elementa";
    }
    s.stavi(temp);
  }
}

void test1() { // testiranje osnovnog slucaja. treba ispisati 1 1.
  Stek<vector<int>> s;
  vector<int> a, b;
  for (int i = 0; i < 5; i++)
    a.push_back(i);
  s.stavi(a);
  for (int i = 5; i < 10; i++)
    b.push_back(i);
  s.stavi(b);
  pretraga(s, 6);
  std::cout << std::endl;
}

void test2() { // testiranje slucaja kada trazenog elementa nema. treba ispisati
               // "Nema elementa".
  Stek<vector<int>> s;
  vector<int> a;
  for (int i = 0; i < 5; i++)
    a.push_back(i);
  s.stavi(a);
  pretraga(s, 300);
  std::cout << std::endl;
}

void test3() { // testiranje osnovnog slucaja. treba ispisati 1 3.
  Stek<vector<int>> s;
  vector<int> a, b, c, d;
  for (int i = 0; i < 5; i++)
    a.push_back(i);
  s.stavi(a);
  for (int i = 5; i < 10; i++)
    b.push_back(i);
  s.stavi(b);
  for (int i = 10; i < 15; i++)
    c.push_back(i);
  s.stavi(c);
  for (int i = 15; i < 20; i++)
    d.push_back(i);
  s.stavi(d);
  pretraga(s, 16);
  std::cout << std::endl;
}

int main() {
  test1();
  test2();
  test3();
}