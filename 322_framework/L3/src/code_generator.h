#pragma once

#include <L3.h>
#include <tiling.h>

namespace L3{
  void generate_L2_file(Program p);
  
  /* Visitor Pattern */
  class CodeGen {
    public:
    CodeGen(/*Function* f*/); 
    vector<std::string> L2_instructions;
     void visit(Tile_return_t *t) ;
     void visit(Tile_return *t) ;
     void visit(Tile_br *t) ;
     void visit(Tile_br_t *t) ;
     void visit(Tile_math *t) ;
     void visit(Tile_math_specialized *t) ;
     void visit(Tile_load *t) ;
     void visit(Tile_store *t) ;
     void visit(Tile_assign *t) ;
     void visit(Tile_increment *t); 
     void visit(Tile_at *t); 
     void visit(Tile_compare *t);
  };
}
