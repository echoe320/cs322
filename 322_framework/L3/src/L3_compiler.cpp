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
#include <L3_parser.h>
#include <code_generator.h>

using namespace std;

void print_help(char *progName){
  std::cerr << "Usage: " << progName << " [-v] [-g 0|1] [-O 0|1|2] [-s] [-l] [-i] SOURCE" << std::endl;
  return;
}

extern bool shouldPrint;

int main(
  int argc,
  char **argv
  ){
  // std::cout << "STARTING FROM THE VERY BEGINNING" << std::endl;
  auto enable_code_generator = true;
  // auto spill_only = false;
  // auto interference_only = false;
  // auto liveness_only = false;
  int32_t optLevel = 3;

  /*
   * Check the compiler arguments.
   */
  auto verbose = false;
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

  /* parse L3 file */
  p = L3::parse_file(argv[optind]);

  /*
   * Generate the target code.
   */
  if (enable_code_generator){
    L3::generate_L2_file(p);
  }

  return 0;
}
