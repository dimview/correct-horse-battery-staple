// Pick four random words from dictionary for use as password.
// See https://xkcd.com/936/ for rationale.

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

// According to Apple developer library, in OS X one can get cryptographically secure 
// pseudorandom numbers by reading bytes from the /dev/random device file
uint64_t get_random_uint64(void)
{
  FILE *fp = fopen("/dev/random", "r"); 
  if (!fp) {
    perror("Error opening /dev/random");
    exit(-1);
  }
  uint64_t value = 0;
  for (int i = 0; i < sizeof(value); i++) {
    value <<= 8;
    value |= fgetc(fp);
  }
  fclose(fp);
  return value;
}

int main(int argc, char* argv[])
{
  std::ifstream f("/usr/share/dict/words");
  if (!f.is_open()) {
    perror("Error opening file");
    return -1;
  }
  std::string line;
  std::vector<std::string> word;
  while (getline(f, line)) {
    word.push_back(line);
  }
  if (f.bad()) {
    perror("Error reading file");
    return -2;
  }
  int candidates = 1;
  if (argc == 2) {
    candidates = atoi(argv[1]);
  }
  for (int i = 0; i < candidates; i++) {
    for (int w = 0; w < 4; w++) {
      if (w) {
        std::cout << '-';
      }
      // Given dictionary size, bias introduces by modulo operation is negligible
      std::cout << word.at(get_random_uint64() % word.size());
    }
    std::cout << std::endl;
  }
  return 0;
}
