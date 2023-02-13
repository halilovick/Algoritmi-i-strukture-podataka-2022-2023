#include <iostream>

using namespace std;

int fib2_0(int n, int pr = 1, int ppr = 0) {
  if (n == 1)
    return pr;
  return fib2_0(n - 1, pr + ppr, pr);
}

int nzd(int x, int y) {
  if (y == 0)
    return x;
  return nzd(y, x % y);
}

int main() {}