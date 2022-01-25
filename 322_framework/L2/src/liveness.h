#pragma once

#include <L2.h>

namespace L2{
  class reg_var {
    //enum reg {regs, var}; //num -> string
    int enum_reg;
    string v; // reg string also?
  };
  void create_liveness_list(Function f);
}