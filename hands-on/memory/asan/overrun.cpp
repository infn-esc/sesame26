#include <cstdlib>

int main() {
  auto *c = static_cast<char *>(std::malloc(10 * sizeof(char)));
  c[10] = 'c';
}
