#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

template <typename Tip> void bubble_sort(Tip *niz, int vel) {
  for (int i = vel - 1; i >= 1; i--) {
    for (int j = 1; j <= i; j++) {
      if (niz[j - 1] > niz[j])
        std::swap(niz[j - 1], niz[j]);
    }
  }
}

template <typename Tip> void selection_sort(Tip *niz, int vel) {
  for (int i = 0; i <= vel - 2; i++) {
    Tip min = niz[i];
    int min_index = i;
    for (int j = i + 1; j <= vel - 1; j++) {
      if (niz[j] < min) {
        min = niz[j];
        min_index = j;
      }
    }
    niz[min_index] = niz[i];
    niz[i] = min;
  }
}

template <typename Tip>
void Merge(Tip *niz, const int l, const int p, const int q, const int u) {
  int i = 0;
  int j = q - l;
  int k = l;
  Tip *temp = new Tip[u - l + 1];
  for (int m = 0; m <= u - l; m++)
    temp[m] = niz[l + m];
  while (i <= p - l && j <= u - l) {
    if (temp[i] < temp[j]) {
      niz[k] = temp[i];
      i++;
    } else {
      niz[k] = temp[j];
      j++;
    }
    k++;
  }
  while (i <= p - l) {
    niz[k] = temp[i];
    k++;
    i++;
  }
  while (j <= u - l) {
    niz[k] = temp[j];
    k++;
    j++;
  }
  delete[] temp;
}

template <typename Tip> void Merge_SortR(Tip *niz, const int l, const int u) {
  if (u > l) {
    int p = (l + u - 1) / 2;
    int q = p + 1;
    Merge_SortR(niz, l, p);
    Merge_SortR(niz, q, u);
    Merge(niz, l, p, q, u);
  }
}

template <typename Tip> void merge_sort(Tip *niz, int vel) {
  Merge_SortR(niz, 0, vel - 1);
}

template <typename Tip> int particija(Tip *niz, int prvi, int zadnji) {
  Tip pivot = niz[prvi];
  int p = prvi + 1;
  while (p <= zadnji && niz[p] < pivot)
    p++;
  for (int i = p + 1; i <= zadnji; i++) {
    if (niz[i] < pivot) {
      // std::swap(niz[i], niz[p]);
      Tip temp = niz[i];
      niz[i] = niz[p];
      niz[p] = temp;
      p++;
    }
  }
  // std::swap(niz[prvi], niz[p - 1]);
  Tip temp = niz[prvi];
  niz[prvi] = niz[p - 1];
  niz[p - 1] = temp;
  return p - 1;
}

template <typename Tip> void Quick_SortR(Tip *niz, int p, int z) {
  if (p < z) {
    int j = particija(niz, p, z);
    Quick_SortR(niz, p, j - 1);
    Quick_SortR(niz, j + 1, z);
  }
}

template <typename Tip> void quick_sort(Tip *niz, int vel) {
  Quick_SortR(niz, 0, vel - 1);
}

void ucitaj(std::string filename, int *&niz, int &vel) {
  std::ifstream ulaz(filename);
  if (!ulaz)
    throw "Greska";
  int br = -1;
  int el;
  while (ulaz >> el)
    br++;
  niz = new int[br];
  ulaz.close();
  ulaz.open(filename);
  for (int i = 0; i < br; i++) {
    ulaz >> niz[i];
  }
}

void generisi(std::string filename, int vel) {
  std::ofstream izlaz(filename);
  std::srand(time(NULL));
  for (int i = 0; i < vel; i++) {
    izlaz << std::rand() % 100 + 1 << " ";
  }
}

void start(std::string filename, int vel) {
  int *niz;
  generisi(filename, vel);
  ucitaj(filename, niz, vel);
  std::cout << "Odaberite koji algoritam sortiranja ce se koristiti: "
            << std::endl;
  std::cout << "1 za Bubble sort\n2 za Selection sort\n3 za Quick sort\n4 "
               "za Merge sort"
            << std::endl;
  char unos;
  std::cin >> unos;
  clock_t t1, t2;
  if (unos == '1') {
    t1 = clock();
    bubble_sort(niz, vel);
    t2 = clock();
  } else if (unos == '2') {
    t1 = clock();
    selection_sort(niz, vel);
    t2 = clock();
  } else if (unos == '3') {
    t1 = clock();
    quick_sort(niz, vel);
    t2 = clock();
  } else if (unos == '4') {
    t1 = clock();
    merge_sort(niz, vel);
    t2 = clock();
  } else
    throw "Neispravan unos!";
  long long int vrijemeIzvrsavanja = (t2 - t1) / (CLOCKS_PER_SEC / 1000);
  for (int i = 0; i < vel - 1; i++)
    if (niz[i] > niz[i + 1])
      throw "Niz nije sortiran!";
  std::cout << "Niz uspjesno sortiran!" << std::endl;
  std::cout << "Vrijeme izvrsavanja funkcije za sortiranje: "
            << vrijemeIzvrsavanja;
  std::ofstream file("sortiraniNiz.txt");
  for (int i = 0; i < vel; i++)
    file << niz[i] << " ";
  file.close();
}

int main() {
  int n;
  std::cout << "Unesite broj elemenata: ";
  std::cin >> n;
  generisi("brojevi.txt", n);
  start("brojevi.txt", n);
}
