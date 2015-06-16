#include "BioCartesian.hpp"

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  typedef BioBEM kernel_type;

  kernel_type K(5,0.1,3);
}
