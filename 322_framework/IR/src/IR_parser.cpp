#include <sched.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <IR_parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace IR {
  /* 
   * Keywords.
   */

  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
  struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {}; //!DO SOMETHIGN ABOUT THIS"

  /*
   * Basics
   */

  struct name:
    pegtl::seq<
      pegtl::plus< 
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >
        >
      >,
      pegtl::star<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >,
          pegtl::digit
        >
      >
    > {};
  
  struct label :
    pegtl::seq<
      pegtl::one<':'>,
      name
    > {};
  
  struct label_rule:
    label {};

  struct comment : 
    pegtl::disable< 
      TAOCPP_PEGTL_STRING( "//" ), 
      pegtl::until< pegtl::eolf > 
    > {};
  
  struct seps : 
    pegtl::star< 
      pegtl::sor< 
        pegtl::ascii::space, 
        comment 
      > 
    > {};
  
  struct var :
    pegtl::seq<
      pegtl::one<'%'>,
      name
    > {};
  
  struct var_rule:
    var {};
  
  struct number:
    pegtl::seq<
      pegtl::opt<
        pegtl::sor<
          pegtl::one< '-' >,
          pegtl::one< '+' >
        >
      >,
      pegtl::plus< 
        pegtl::digit
      >
    >{};
  
  struct number_rule :
    number {};

  struct t_rule :
    pegtl::sor<
      var_rule,
      number_rule
    > {};
  
  struct s_rule :
    pegtl::sor<
      t_rule,
      label_rule
    > {};
  
  struct u_rule :
    pegtl::sor<
      var_rule,
      label_rule
    > {};

  // type/T rules
  struct str_int64 : TAOCPP_PEGTL_STRING( "int64" ) {};
  struct str_brackets : TAOCPP_PEGTL_STRING( "[]" ) {};
  struct str_tuple : TAOCPP_PEGTL_STRING( "tuple" ) {};
  struct str_code : TAOCPP_PEGTL_STRING( "code" ) {};
  struct str_void : TAOCPP_PEGTL_STRING( "void" ) {};

  struct type_rule :
    pegtl::sor<
      pegtl::seq<
        str_int64, 
        pegtl::star<
          str_brackets
        >
      >,
      str_tuple, 
      str_code
    > {};
  
  struct T_rule :
    pegtl::sor<
      type_rule,
      str_void
    > {};

  /*
   * Instruction Rules
   */
  
  // Instruction_label_rule
  
  struct Instruction_label_rule :
    pegtl::seq<
      seps,
      label_rule,
      seps
    > {};
  
  // Instruction_define_rule

  struct Instruction_define_rule :
    pegtl::seq<
      seps,
      type_rule,
      seps,
      var_rule,
      seps
    > {};

  // Instruction_assignment_rule

  struct Instruction_assignment_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      s_rule,
      seps
    > {};

  // Instruction_op_rule
  struct str_plus : TAOCPP_PEGTL_STRING( "+" ) {};
  struct str_minus : TAOCPP_PEGTL_STRING( "-" ) {};
  struct str_mult : TAOCPP_PEGTL_STRING( "*" ) {};
  struct str_bitAND : TAOCPP_PEGTL_STRING( "&" ) {};
  struct str_leftShift : TAOCPP_PEGTL_STRING( "<<" ) {};
  struct str_rightShift : TAOCPP_PEGTL_STRING( ">>" ) {};
  struct str_lessEqual : TAOCPP_PEGTL_STRING( "<=" ) {};
  struct str_greatEqual : TAOCPP_PEGTL_STRING( ">=" ) {};
  struct str_less : TAOCPP_PEGTL_STRING( "<" ) {};
  struct str_great : TAOCPP_PEGTL_STRING( ">" ) {};
  struct str_equal : TAOCPP_PEGTL_STRING( "=" ) {};

  struct plus_rule : str_plus {};
  struct minus_rule : str_minus {};
  struct mult_rule : str_mult {};
  struct bitAND_rule : str_bitAND {};
  struct Lshift_rule : str_leftShift {};
  struct Rshift_rule : str_rightShift {};
  struct lessEq_rule : str_lessEqual {};
  struct greatEq_rule : str_lessEqual {};
  struct less_rule : str_less {};
  struct great_rule : str_less {};
  struct equal_rule : str_equal {};

  struct op_rule :
    pegtl::sor<
      plus_rule,
      minus_rule,
      mult_rule,
      bitAND_rule,
      Lshift_rule,
      Rshift_rule,
      lessEq_rule,
      greatEq_rule,
      less_rule,
      great_rule,
      equal_rule
    > {};

  struct Instruction_op_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      t_rule,
      seps,
      op_rule,
      seps,
      t_rule,
      seps
    > {};

  // Instruction_load_rule

  struct indices_rule : 
    pegtl::plus<
      pegtl::seq<
        pegtl::one< '[' >,
        pegtl::sor<
          t_rule
        >, 
        pegtl::one< ']' >
      >
    >{};

  struct Instruction_load_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      var_rule,
      seps,
      indices_rule,
      seps
    > {};

  // Instruction_store_rule

  struct Instruction_store_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      indices_rule,
      seps,
      str_arrow,
      seps,
      s_rule,
      seps
    > {};

  // Instruction_length_rule

  struct str_length : TAOCPP_PEGTL_STRING( "length" ) {};

  struct length_rule : str_length {};

  struct Instruction_length_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      length_rule,
      seps,
      var_rule,
      seps,
      t_rule,
      seps
    > {};

  // Instruction_call_rule

  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_tensor_error : TAOCPP_PEGTL_STRING( "tensor-error" ) {};

  struct call_rule : str_call {};
  struct print_rule : str_print {};
  struct input_rule : str_input {};
  struct tensor_error_rule : str_tensor_error {};

  struct callee :
    pegtl::sor<
      u_rule,
      print_rule,
      input_rule,
      tensor_error_rule
    > {};

  struct callee_rule : callee {};

  struct arguments_rule :
    pegtl::sor<
      pegtl::seq<
        pegtl::sor<
          variable_rule, 
          number_rule
        >,
        pegtl::star<
          pegtl::seq<
            pegtl::one<','>,
            seps,
            pegtl::sor<
              variable_rule, 
              number_rule
            >
          >
        >
      >,
      seps
    > {};

  struct Instruction_call_rule :
    pegtl::seq<
      seps,
      call_rule,
      seps,
      callee_rule,
      seps,
      pegtl::one< '(' >,
      seps,
      arguments_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > {};

  // Instruction_call_assign_rule

  struct Instruction_call_assign_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      call_rule,
      seps,
      callee_rule,
      seps,
      pegtl::one< '(' >,
      seps,
      arguments_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > {};

  // Instruction_new_array_rule

  struct str_new : TAOCPP_PEGTL_STRING( "new" ) {};
  struct str_array : TAOCPP_PEGTL_STRING( "Array" ) {};

  struct new_rule : str_new {};
  struct array_rule : str_array {};

  struct Instruction_new_array_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      new_rule,
      seps,
      array_rule,
      seps,
      pegtl::one< '(' >,
      seps,
      arguments_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > {};

  // Instruction_new_tuple_rule

  struct str_tuple : TAOCPP_PEGTL_STRING( "Tuple" ) {};

  struct tuple_rule : str_tuple {};

  struct Instruction_new_tuple_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      new_rule,
      seps,
      tuple_rule,
      seps,
      pegtl::one< '(' >,
      seps,
      t_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > {};
  
  // Umbrella Instruction Rule(s)
  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_define_rule>      , Instruction_define_rule       >,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>  , Instruction_assignment_rule   >,
      pegtl::seq< pegtl::at<Instruction_op_rule>          , Instruction_op_rule           >,
      pegtl::seq< pegtl::at<Instruction_load_rule>        , Instruction_load_rule         >,
      pegtl::seq< pegtl::at<Instruction_store_rule>       , Instruction_store_rule        >,
      pegtl::seq< pegtl::at<Instruction_length_rule>      , Instruction_length_rule       >,
      pegtl::seq< pegtl::at<Instruction_call_rule>        , Instruction_call_rule         >,
      pegtl::seq< pegtl::at<Instruction_call_assign_rule> , Instruction_call_assign_rule  >,
      pegtl::seq< pegtl::at<Instruction_new_array_rule>   , Instruction_new_array_rule    >,
      pegtl::seq< pegtl::at<Instruction_new_tuple_rule>   , Instruction_new_tuple_rule    >
    > {};

  struct Instructions_rule:
    pegtl::plus<
      pegtl::seq<
        seps,
        Instruction_rule,
        seps
      >
    > {};
  
  /*
   * te rules
   */

  // te_br_label_rule

  // te_br_t_rule

  // te_return_rule

  // te_return_t_rule

  // Umbrella te Rule(s)
  struct te_rule : 
    pegtl::sor<
      pegtl::seq< pegtl::at<te_br_label_rule>             , te_br_label_rule              >,
      pegtl::seq< pegtl::at<te_br_t_rule>                 , te_br_t_rule                  >,
      pegtl::seq< pegtl::at<te_return_rule>               , te_return_rule                >,
      pegtl::seq< pegtl::at<te_return_t_rule>             , te_return_t_rule              >
    > {};

  /* 
   * Basic Blocks
   */
  
  struct Basic_Block_rule:
    pegtl::seq<
      seps,
      Instruction_label_rule, 
      seps, 
      Instructions_rule,
      seps,
      te_rule,
      seps
    > {};

  struct Basic_Blocks_rule: 
    pegtl::plus<
      pegtl::seq<
        seps,
        Basic_Block_rule,
        seps
      >
    > {};

  /* 
   * Function Rule 
   */
  
  // Function Components 

  struct str_define : TAOCPP_PEGTL_STRING( "define" ) {};

  struct function_type : 
    T_rule {};
  
  struct function_name :
    label_rule {};

  struct function_arguments_rule :
    pegtl::sor<
      pegtl::seq<
        type_rule,
        seps,
        variable_rule,
        pegtl::star<
          pegtl::seq<
            pegtl::one< ',' >,
            seps,
            type_rule,
            seps,
            variable_rule
          >
        >
      >,
      seps
    > {};

  // Function Rule

  struct Function_rule:
    pegtl::seq<
      seps,
      str_define,
      seps,
      function_type,
      seps,
      function_name,
      seps,
      pegtl::one< '(' >,
      seps,
      function_arguments_rule,
      seps,
      pegtl::one< ')' >,
      seps,
      pegtl::one< '{' >,
      seps,
      Basic_Blocks_rule,
      seps,
      pegtl::one< '}' >,
      seps
    > {};

  struct Functions_rule: // covers multiple functions
    pegtl::plus<
      pegtl::seq<
        seps,
        Function_rule,
        seps
      >
    > {};

  struct grammer :
    pegtl::must< 
      Functions_rule
    > {};

//================================= ACTIONS =================================
  /* 
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < function_name > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "function_name (no end)\n";
      auto newF = new Function();
      newF->name = in.string();
      p.functions.push_back(newF);
    }
  };


  Program parse_file (char *fileName){

    /* 
     * Check the grammar for some possible issues.
     */
    pegtl::analyze< grammar >();

    /*
     * Parse.
     */   
    file_input< > fileInput(fileName);
    Program p;
    parse< grammar, action >(fileInput, p);

    return p;
  }
}