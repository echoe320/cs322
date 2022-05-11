#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

#include <L3.h>
#include <parser.h>
#include <codegenerator.h>

using namespace std;

void print_help(char *progName)
{
  std::cerr << "Usage: " << progName << " [-v] [-g 0|1] [-O 0|1|2] [-s] [-l] [-i] SOURCE" << std::endl;
  return;
}

bool is_debug = false;

int main(
    int argc,
    char **argv)
{
  auto enable_code_generator = true;
  int32_t optLevel = 3;

  /*
   * Check the compiler arguments.
   */
  auto verbose = false;
  bool tiling_test = false;
  if (argc < 2)
  {
    print_help(argv[0]);
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "vg:O:slit")) != -1)
  {
    switch (opt)
    {

    case 'O':
      optLevel = strtoul(optarg, NULL, 0);
      break;

    case 'g':
      enable_code_generator = (strtoul(optarg, NULL, 0) == 0) ? false : true;
      break;

    case 'v':
      verbose = true;
      is_debug = true;
      break;
    
    case 't':
      tiling_test = true;
      break;

    default:
      print_help(argv[0]);
      return 1;
    }
  }
  /*
   * Parse the input file.
   */
  L3::Program p;
/*
    * Parse the L3 program.
    */
  p = L3::parse_file(argv[optind]);



  /*
   * Generate the target code.
   */
  if (enable_code_generator)
  {
    // TODO
    L3::generate_code(p);
  }

  return 0;
}
