#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void radixSort(std::vector<int> &a) {
  int max = a.at(0);
  for (int i = 0; i < a.size(); i++)
    if (a.at(i) > max)
      max = a.at(i);
  std::vector<int> brojac(0);
  brojac.resize(10);
  std::vector<int> temp;
  int stepen = 1;
  temp.resize(a.size());
  while (max / stepen > 0) {
    for (int i = 0; i < brojac.size(); i++)
      brojac.at(i) = 0;
    for (int i = 0; i < a.size(); i++)
      brojac.at((a.at(i) / stepen) % 10)++;
    for (int i = 1; i < 10; i++)
      brojac.at(i) = brojac.at(i) + brojac.at(i - 1);
    for (int i = a.size() - 1; i >= 0; i--) {
      temp.at(brojac.at((a.at(i) / stepen) % 10) - 1) = a.at(i);
      brojac.at((a.at(i) / stepen) % 10)--;
    }
    for (int i = 0; i < a.size(); i++)
      a.at(i) = temp.at(i);
    stepen = stepen * 10;
  }
}

bool jeLiList(std::vector<int> &a, int i, int vel) {
  return (i >= vel / 2) && (i < vel);
}

int lijevoDijete(int i) { return 2 * i + 1; }

int desnoDijete(int i) { return 2 * i + 2; }

void popraviDole(std::vector<int> &a, int i, int velicina) {
  if (jeLiList(a, i, velicina) == false) {
    int veci = lijevoDijete(i);
    int dd = desnoDijete(i);
    if (dd < velicina) {
      if (a[veci] < a[dd])
        veci = dd;
    }
    if (a[veci] > a[i]) {
      std::swap(a[i], a[veci]);
      popraviDole(a, veci, velicina);
    }
  }
}

void stvoriGomilu(std::vector<int> &a) {
  for (int i = a.size() / 2; i >= 0; i--)
    popraviDole(a, i, a.size());
}

int roditelj(int i) { return (i - 1) / 2; }

void popraviGore(std::vector<int> &a, int i) {
  if (i != 0 && a[i] > a[roditelj(i)]) {
    std::swap(a[i], a[roditelj(i)]);
    popraviGore(a, roditelj(i));
  }
}

void umetniUGomilu(std::vector<int> &a, int umetni, int &velicina) {
  a.push_back(umetni);
  velicina++;
  popraviGore(a, velicina - 1);
}

int izbaciPrvi(std::vector<int> &a, int &velicina) {
  if (velicina == 0)
    throw "Gomila je prazna!";
  velicina--;
  std::swap(a[0], a[velicina]);
  if (velicina != 0)
    popraviDole(a, 0, velicina);
  return a[velicina];
}

void gomilaSort(std::vector<int> &a) {
  stvoriGomilu(a);
  int vel = a.size();
  for (int i = 0; i <= a.size() - 2; i++) {
    izbaciPrvi(a, vel);
  }
}

int main() {}