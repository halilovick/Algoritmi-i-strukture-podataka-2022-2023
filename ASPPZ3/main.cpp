#include <iostream>

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

void test1() { // test konstruktora bez parametara, metode stavi() i metode
               // brojElemenata()
  Stek<int> s;
  for (int i = 1; i <= 3; i++)
    s.stavi(i);
  if (s.brojElemenata() == 3)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test2() { // test metode skini()
  Stek<int> s;
  for (int i = 1; i <= 3; i++)
    s.stavi(i);
  s.skini();
  if (s.brojElemenata() == 2)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test3() { // test metode vrh()
  Stek<int> s;
  for (int i = 1; i <= 3; i++)
    s.stavi(i);
  s.skini();
  if (s.vrh() == 2)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test4() { // test metode brisi()
  Stek<int> s;
  for (int i = 1; i <= 3; i++)
    s.stavi(i);
  s.brisi();
  if (s.brojElemenata() == 0)
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test5() { // test operatora dodjele
  Stek<int> s;
  for (int i = 1; i <= 3; i++)
    s.stavi(i);
  Stek<int> temp1 = s;
  if (s.brojElemenata() == temp1.brojElemenata())
    std::cout << "OK";
  else
    std::cout << "NOK";
  std::cout << std::endl;
}

void test6() { // test konstruktora kopije
  Stek<int> s;
  for (int i = 1; i <= 3; i++)
    s.stavi(i);
  Stek<int> temp1(s);
  if (s.brojElemenata() == temp1.brojElemenata())
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
  return 0;
}