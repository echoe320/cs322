#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>

using namespace std;

namespace L2 {
  //string generate_liveness_list(Function f) {}

  void create_liveness_file(Function f) {

    /*
     * Open output file
     */
    std::ofstream outputFile;
    outputFile.open("liveness.out");

    //* =========================== START OF LIVENESS ANALYSIS ===========================
    int numInstructions = f.instructions.size();
    std::vector<std::unordered_set> GEN[numInstructions];
    std::vector<std::unordered_set> KILL[numInstructions];
    std::vector<std::unordered_set> IN[numInstructions];
    std::vector<std::unordered_set> OUT[numInstructions];

    for (int i = 0; i < numInstructions; i++) {
      GEN[i] = {};
      KILL[i] = {};
    }

    for (int i = 0; i < numInstructions; i++) {
      IN[i] = {}; // IN[i] = {};
      OUT[i] = {}; // OUT[i] = {};
    }

    // do {
    //   for (int i = 0; i < numInstructions; i++) {
    //     result
    //   }
    // }

    //* =========================== END OF LIVENESS ANALYSIS ===========================
    
    /*
     *close output file
     */
    outputFile.close();
    return ;
  }
}