#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cctype>

/**
 * Cleaner choosing of # particles
 */

int parse_n(const char *str)
{
  char *number = new char[strlen(str)];
  char suffix = 'i';
  for (int i=0; i<strlen(str); i++) {
    if (isdigit(str[i])) {
      number[i] = str[i];
    } else {
      suffix = str[i];
    }
  }

  int num = atoi(number);

  switch (suffix) {
    case 'i':
    case 'I':
       break;
    case 'k':
    case 'K':
       num *= 1000;
       break;
    case 'm':
    case 'M':
      num *= 1000000;
      break;
    case 'b':
    case 'B':
      num *= 1000000000;
      break;
    default:
      printf("Invalid suffix: [iIkKmMbB]\n");
  }
  return num;
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("Requires an argument\n");
  }

  int nparticles = parse_n(argv[1]);

  printf("# particles : %d\n", nparticles);
}
