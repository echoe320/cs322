#include <string>
#include <iostream>
#include <fstream>

#include <liveness.h>

// included libraries
#include <unordered_set>

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
    
    //? what type should the elements of the unordered_set be? i think Item is okay
    std::vector<std::unordered_set<Item>> GEN[numInstructions]; //* GEN[i] = all variables read by instruction i
    std::vector<std::unordered_set<Item>> KILL[numInstructions]; //* KILL[i] = all variables written/defined by instruction i
    std::vector<std::unordered_set<Item>> IN[numInstructions];
    std::vector<std::unordered_set<Item>> OUT[numInstructions];

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