#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <liveness.h>
#include <L2.h>

// included libraries
#include <unordered_set>

using namespace std;

namespace L2 {
  //Defining visitor functions
  void Instruction_ret::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_assignment::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_arithmetic::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_crement::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_shift::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_cmp::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_cjump::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_lea::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_calls::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_runtime::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_label::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_goto::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }
  void Instruction_stackarg::Accept (Visitor *visitor) {
    visitor->VisitInstruction(this);
  }

  // computing GEN and KILL sets
  std::unordered_set<Item *> Instruction_ret::getGEN() {

  }
  std::unordered_set<Item *> Instruction_ret::getKILL() {
      
  }


  std::unordered_set<Item *> Instruction_assignment::getGEN() {
      if (dynamic_cast<Variable *> this->)
  }

}