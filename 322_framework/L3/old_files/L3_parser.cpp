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
#include <regex>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L3_parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L3 {
  bool shouldPrint = true;
  // var_type vType_curr;

  /* 
   * Data required to parse
   */ 

  std::vector<Item *> parsed_items;
  std::vector<std::vector<Item *>> vec_of_args;
  std::vector<var_type> vType_curr;

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

  // INCLUDE VARS_RULE
  struct vars_rule :
    pegtl::sor<
      pegtl::seq<
        pegtl::sor<
          var_rule
        >,
        pegtl::star<
          pegtl::seq<
            pegtl::one<','>,
            seps,
            pegtl::sor<
              var_rule
            >
          >
        >
      >,
      seps
    > {};
  
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

  // // type/T rules
  // struct str_int64 : TAOCPP_PEGTL_STRING( "int64" ) {};
  // struct str_brackets : TAOCPP_PEGTL_STRING( "[]" ) {};
  // struct str_tuple : TAOCPP_PEGTL_STRING( "tuple" ) {};
  // struct str_code : TAOCPP_PEGTL_STRING( "code" ) {};
  // struct str_void : TAOCPP_PEGTL_STRING( "void" ) {};

  // struct type_rule :
  //   pegtl::sor<
  //     pegtl::seq<
  //       str_int64, 
  //       pegtl::star<
  //         str_brackets
  //       >
  //     >,
  //     str_tuple, 
  //     str_code
  //   > {};
  
  // struct T_rule :
  //   pegtl::sor<
  //     type_rule,
  //     str_void
  //   > {};

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

  // struct Instruction_define_rule :
  //   pegtl::seq<
  //     seps,
  //     type_rule,
  //     seps,
  //     var_rule,
  //     seps
  //   > {};

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
  struct str_load : TAOCPP_PEGTL_STRING( "load" ) {};

  struct load_rule : str_load {};

  // struct indices_rule : 
  //   pegtl::plus<
  //     pegtl::seq<
  //       pegtl::one< '[' >,
  //       pegtl::sor<
  //         t_rule
  //       >, 
  //       pegtl::one< ']' >
  //     >
  //   >{};

  struct Instruction_load_rule :
    pegtl::seq<
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      load_rule,
      seps,
      var_rule, // indices_rule,
      seps
    > {};

  // Instruction_store_rule
  struct str_store : TAOCPP_PEGTL_STRING( "store" ) {};

  struct store_rule : str_store {};

  struct Instruction_store_rule :
    pegtl::seq<
      seps,
      store_rule,
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      s_rule,
      seps
    > {};

  // Instruction_length_rule

  // struct str_length : TAOCPP_PEGTL_STRING( "length" ) {};

  // struct length_rule : str_length {};

  // struct Instruction_length_rule :
  //   pegtl::seq<
  //     seps,
  //     var_rule,
  //     seps,
  //     str_arrow,
  //     seps,
  //     length_rule,
  //     seps,
  //     var_rule,
  //     seps,
  //     t_rule,
  //     seps
  //   > {};

  // Instruction_call_rule

  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_allocate : TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_tensor_error : TAOCPP_PEGTL_STRING( "tensor-error" ) {};

  struct call_rule : str_call {};
  struct print_rule : str_print {};
  struct allocate_rule : str_allocate {};
  struct input_rule : str_input {};
  struct tensor_error_rule : str_tensor_error {};

  struct callee :
    pegtl::sor<
      u_rule,
      print_rule,
      allocate_rule,
      input_rule,
      tensor_error_rule
    > {};

  struct callee_rule : callee {};

  struct arguments_rule :
    pegtl::sor<
      pegtl::seq<
        pegtl::sor<
          t_rule
        >,
        pegtl::star<
          pegtl::seq<
            pegtl::one<','>,
            seps,
            pegtl::sor<
              t_rule
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

  // struct str_new : TAOCPP_PEGTL_STRING( "new" ) {};
  // struct str_array : TAOCPP_PEGTL_STRING( "Array" ) {};

  // struct new_rule : str_new {};
  // struct array_rule : str_array {};

  // struct Instruction_new_array_rule :
  //   pegtl::seq<
  //     seps,
  //     var_rule,
  //     seps,
  //     str_arrow,
  //     seps,
  //     new_rule,
  //     seps,
  //     array_rule,
  //     seps,
  //     pegtl::one< '(' >,
  //     seps,
  //     arguments_rule,
  //     seps,
  //     pegtl::one< ')' >,
  //     seps
  //   > {};

  // // Instruction_new_tuple_rule

  // // struct str_tuple : TAOCPP_PEGTL_STRING( "Tuple" ) {};

  // struct tuple_rule : str_tuple {};

  // struct Instruction_new_tuple_rule :
  //   pegtl::seq<
  //     seps,
  //     var_rule,
  //     seps,
  //     str_arrow,
  //     seps,
  //     new_rule,
  //     seps,
  //     tuple_rule,
  //     seps,
  //     pegtl::one< '(' >,
  //     seps,
  //     t_rule,
  //     seps,
  //     pegtl::one< ')' >,
  //     seps
  //   > {};

  /*
   * Instruction br rules
   */

  // Instruction_br_label_rule

  struct str_br : TAOCPP_PEGTL_STRING( "br" ) {};

  struct br_rule : str_br {};

  struct Instruction_br_label_rule :
    pegtl::seq<
      seps,
      br_rule,
      seps,
      label_rule,
      seps
    > {};

  // Instruction_br_t_rule

  struct Instruction_br_t_rule :
    pegtl::seq<
      seps,
      br_rule,
      seps,
      t_rule,
      seps,
      label_rule,
      seps,
      label_rule,
      seps
    > {};

  // Instruction_return_rule

  struct Instruction_return_rule :
    pegtl::seq<
      seps,
      str_return,
      seps
    > {};

  // Instruction_return_t_rule

  struct Instruction_return_t_rule :
    pegtl::seq<
      seps,
      str_return,
      seps,
      t_rule,
      seps
    > {};
  
  // Umbrella Instruction Rule(s)
  struct Instruction_rule:
    pegtl::sor<
      // pegtl::seq< pegtl::at<Instruction_define_rule>      , Instruction_define_rule       >,
      pegtl::seq< pegtl::at<Instruction_op_rule>          , Instruction_op_rule           >,
      pegtl::seq< pegtl::at<Instruction_load_rule>        , Instruction_load_rule         >,
      pegtl::seq< pegtl::at<Instruction_store_rule>       , Instruction_store_rule        >,
      // pegtl::seq< pegtl::at<Instruction_length_rule>      , Instruction_length_rule       >,
      pegtl::seq< pegtl::at<Instruction_call_rule>        , Instruction_call_rule         >,
      pegtl::seq< pegtl::at<Instruction_call_assign_rule> , Instruction_call_assign_rule  >,
      // pegtl::seq< pegtl::at<Instruction_new_array_rule>   , Instruction_new_array_rule    >,
      // pegtl::seq< pegtl::at<Instruction_new_tuple_rule>   , Instruction_new_tuple_rule    >,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>  , Instruction_assignment_rule   >,
      pegtl::seq< pegtl::at<Instruction_br_label_rule>             , Instruction_br_label_rule              >,
      pegtl::seq< pegtl::at<Instruction_br_t_rule>                 , Instruction_br_t_rule                  >,
      pegtl::seq< pegtl::at<Instruction_return_t_rule>             , Instruction_return_t_rule              >,
      pegtl::seq< pegtl::at<Instruction_return_rule>               , Instruction_return_rule                >

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

  // Instruction_br_label_rule

  // struct str_br : TAOCPP_PEGTL_STRING( "br" ) {};

  // struct br_rule : str_br {};

  // struct Instruction_br_label_rule :
  //   pegtl::seq<
  //     seps,
  //     br_rule,
  //     seps,
  //     label_rule,
  //     seps
  //   > {};

  // // Instruction_br_t_rule

  // struct Instruction_br_t_rule :
  //   pegtl::seq<
  //     seps,
  //     br_rule,
  //     seps,
  //     t_rule,
  //     seps,
  //     label_rule,
  //     seps,
  //     label_rule,
  //     seps
  //   > {};

  // // Instruction_return_rule

  // struct Instruction_return_rule :
  //   pegtl::seq<
  //     seps,
  //     str_return,
  //     seps
  //   > {};

  // // Instruction_return_t_rule

  // struct Instruction_return_t_rule :
  //   pegtl::seq<
  //     seps,
  //     str_return,
  //     seps,
  //     t_rule,
  //     seps
  // > {};

  // Umbrella te Rule(s)
  // struct te_rule : 
  //   pegtl::sor<
  //     pegtl::seq< pegtl::at<Instruction_br_label_rule>             , Instruction_br_label_rule              >,
  //     pegtl::seq< pegtl::at<Instruction_br_t_rule>                 , Instruction_br_t_rule                  >,
  //     pegtl::seq< pegtl::at<Instruction_return_t_rule>             , Instruction_return_t_rule              >,
  //     pegtl::seq< pegtl::at<Instruction_return_rule>               , Instruction_return_rule                >
  //   > {};

  /* 
   * Basic Blocks
   */
  
  // struct Basic_Block_rule:
  //   pegtl::seq<
  //     seps,
  //     Instruction_label_rule, 
  //     seps, 
  //     Instructions_rule,
  //     seps,
  //     te_rule,
  //     seps
  //   > {};

  // struct Basic_Blocks_rule: 
  //   pegtl::plus<
  //     pegtl::seq<
  //       seps,
  //       Basic_Block_rule,
  //       seps
  //     >
  //   > {};

  /* 
   * Function Rule 
   */
  
  // Function Components 

  struct str_define : TAOCPP_PEGTL_STRING( "define" ) {};

  // struct function_type : 
  //   T_rule {};
  
  struct function_name :
    label_rule {};

  // struct function_arguments_rule :
  //   pegtl::sor<
  //     pegtl::seq<
  //       type_rule,
  //       seps,
  //       var_rule,
  //       pegtl::star<
  //         pegtl::seq<
  //           pegtl::one< ',' >,
  //           seps,
  //           type_rule,
  //           seps,
  //           var_rule
  //         >
  //       >
  //     >,
  //     seps
  //   > {};

  // Function Rule

  struct Function_rule:
    pegtl::seq<
      seps,
      str_define,
      seps,
      function_name,
      seps,
      pegtl::one< '(' >,
      seps,
      vars_rule,
      seps,
      pegtl::one< ')' >,
      seps,
      pegtl::one< '{' >,
      seps,
      Instructions_rule,
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

  struct grammar :
    pegtl::must< 
      Functions_rule
    > {};

//================================= ACTIONS =================================
  /* 
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  // template<> struct action < function_type > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){
  //     if (shouldPrint) cout << "function_type (no end)\n";
  //     auto newF = new Function();
  //     if (in.string() == "int64") 
  //     {
  //       newF->type = func_type::int64_f;
  //     } else if (std::regex_match (in.string(), std::regex("(int64[])(.*)")))
  //     {
  //       newF->type = func_type::int64_arr_f;
  //     } else if (in.string() == "tuple")
  //     {
  //       newF->type = func_type::tup_f;
  //     } else if (in.string() == "code")
  //     {
  //       newF->type = func_type::code_f;
  //     } else if (in.string() == "void")
  //     {
  //       newF->type = func_type::v_f;
  //     }
      
  //     p.functions.push_back(newF);
  //   }
  // };

  template<> struct action < function_name > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "function_name action started\n";
      auto currentF = p.functions.back();
      // if (in.string() == ":main") currentF->isMain = true;
      currentF->name = in.string();
    }
  };

  // Item rule actions

  template<> struct action < number_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "number_rule started\n";
      Number* n = new Number(std::stol(in.string()));
      // std::cout << n << "\n";
      parsed_items.push_back(n);
      if (shouldPrint) cout << "number_rule ended\n";
    }
  };

  template<> struct action < label_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "label_rule started\n";
      Label* l = new Label(in.string());
      parsed_items.push_back(l);
      if (shouldPrint) cout << "label_rule ended\n";
    }
  };

  // template<> struct action < type_rule > { // need to edit this to scan for two parts
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     if (shouldPrint) cout << "type_rule started\n";
  //     if (in.string() == "int64") 
  //     {
  //       vType_curr.push_back(var_type::int64);
  //     } else if (std::regex_match (in.string(), std::regex("(int64[])(.*)")))
  //     {
  //       vType_curr.push_back(var_type::int64_arr);
  //     } else if (in.string() == "tuple")
  //     {
  //       vType_curr.push_back(var_type::tup);
  //     } else if (in.string() == "code")
  //     {
  //       vType_curr.push_back(var_type::code);
  //     }
  //     if (shouldPrint) cout << "type_rule ended\n";
  //   }
  // };

  template<> struct action < var_rule > { // need to edit this to scan for two parts
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "var started\n";
      var_type vtype_temp = vType_curr.back();
      vType_curr.pop_back();
      Variable* v = new Variable(vtype_temp, in.string());
      parsed_items.push_back(v);

      auto currentF = p.functions.back();
      currentF->existing_vars[in.string()] = v;
      if (shouldPrint) cout << "var ended\n";
    }
  };

  /* Operation rule actions */

  template<> struct action < plus_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "plus_rule started\n";
      Operation* op = new Operation(op_add);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "plus_rule ended\n";
    }
  };

  template<> struct action < minus_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "minus_rule started\n";
      Operation* op = new Operation(op_minus);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "minus_rule ended\n";
    }
  };

  template<> struct action < mult_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "mult_rule started\n";
      Operation* op = new Operation(op_multiply);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "mult_rule ended\n";
    }
  };

  template<> struct action < bitAND_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "bitAND_rule started\n";
      Operation* op = new Operation(op_AND);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "bitAND_rule ended\n";
    }
  };

  // sop_rule -> push
  template<> struct action < Lshift_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Lshift_rule started\n";
      Operation* op = new Operation(op_lshift);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "Lshift_rule ended\n";
    }
  };

  template<> struct action < Rshift_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Rshift_rule started\n";
      Operation* op = new Operation(op_rshift);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "Rshift_rule ended\n";
    }
  };

  template<> struct action < lessEq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "lessEq_rule started\n";
      Operation* op = new Operation(cmp_lesseq);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "lessEq_rule ended\n";
    }
  };
  
  template<> struct action < greatEq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "greatEq_rule started\n";
      Operation* op = new Operation(cmp_greateq);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "greatEq_rule ended\n";
    }
  };

  template<> struct action < less_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "less_rule started\n";
      Operation* op = new Operation(cmp_less);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "less_rule ended\n";
    }
  };
  
  template<> struct action < great_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "great_rule started\n";
      Operation* op = new Operation(cmp_great);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "great_rule ended\n";
    }
  };

  template<> struct action < equal_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "equal_rule started\n";
      Operation* op = new Operation(cmp_equals);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "equal_rule ended\n";
    }
  };

  /* Instruction rule actions */

  // template<> struct action < Instruction_define_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){
  //     if (shouldPrint) cout << "Instruction_define_rule started\n";

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();
  //     auto b = currentF->basicblocks.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto var = parsed_items.back();
  //     parsed_items.pop_back();
  //     auto i = new Instruction_def(var);

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     if (shouldPrint) cout << "Instruction_define_rule ended\n";
  //   }
  // };

  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_assignment_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_assignment(dst, src);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_assignment_rule ended\n";
    }
  };

  // Arithmetic actions
  template<> struct action < Instruction_op_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_op_rule started\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto a2 = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto a1 = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_op(dst, a1, op, a2);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_op_rule ended\n";
    }
  };

  template<> struct action < Instruction_load_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_load_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_load(dst, src);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_load_rule ended\n";
    }
  };

  template<> struct action < Instruction_store_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_store_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_store(dst, src);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_store_rule ended\n";
    }
  };

  // template<> struct action < Instruction_length_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){
  //     if (shouldPrint) cout << "Instruction_length_rule started\n";

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();
  //     auto b = currentF->basicblocks.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto dim = parsed_items.back();
  //     parsed_items.pop_back();
  //     auto src = parsed_items.back();
  //     parsed_items.pop_back();
  //     auto dst = parsed_items.back();
  //     parsed_items.pop_back();
  //     auto i = new Instruction_length(dst, src, dim);

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     if (shouldPrint) cout << "Instruction_length_rule ended\n";
  //   }
  // };

  template<> struct action < print_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "print_rule started\n";
      Callee* cc = new Callee(cc_print);
      parsed_items.push_back(cc);
      if (shouldPrint) std::cout << "print_rule ended\n";
    }
  };

  template<> struct action < allocate_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "allocate_rule started\n";
      Callee* cc = new Callee(cc_allocate);
      parsed_items.push_back(cc);
      if (shouldPrint) std::cout << "allocate_rule ended\n";
    }
  };

  template<> struct action < input_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "input_rule started\n";
      Callee* cc = new Callee(cc_input);
      parsed_items.push_back(cc);
      if (shouldPrint) std::cout << "input_rule ended\n";
    }
  };

  template<> struct action < tensor_error_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "tensor_error_rule started\n";
      Callee* cc = new Callee(cc_tensor_error);
      parsed_items.push_back(cc);
      if (shouldPrint) std::cout << "tensor_error_rule ended\n";
    }
  };

  template<> struct action < vars_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "arguments_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();
      // auto b = currentF->basicblocks.back();

      std::vector<std::string> v;
      std::string s = in.string();
      int l = s.length();
      int c = count(s.begin(), s.end(), ' ');
      remove(s.begin(), s.end(), ' ');
      s.resize(l - c);

      std::stringstream ss(s);
  
      while (ss.good()) {
          std::string substr;
          std::getline(ss, substr, ',');
          v.push_back(substr);
      }

      std::vector<Item *> parsed_args;

      for (auto i : v) {
        if (std::regex_match(i, std::regex("(%)(.*)"))) {
          Variable * v_temp = currentF->existing_vars[i];
          parsed_args.push_back(v_temp);
        } else {
          Number* n_temp = new Number(std::stol(i));
          parsed_args.push_back(n_temp);
        }
      }

      vec_of_args.push_back(parsed_args);

      if (shouldPrint) cout << "arguments_rule ended\n";
    }
  };

  template<> struct action < arguments_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "arguments_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();
      // auto b = currentF->basicblocks.back();

      std::vector<std::string> v;
      std::string s = in.string();
      int l = s.length();
      int c = count(s.begin(), s.end(), ' ');
      remove(s.begin(), s.end(), ' ');
      s.resize(l - c);

      std::stringstream ss(s);
  
      while (ss.good()) {
          std::string substr;
          std::getline(ss, substr, ',');
          v.push_back(substr);
      }

      std::vector<Item *> parsed_args;

      for (auto i : v) {
        if (std::regex_match(i, std::regex("(%)(.*)"))) {
          Variable * v_temp = currentF->existing_vars[i];
          parsed_args.push_back(v_temp);
        } else {
          Number* n_temp = new Number(std::stol(i));
          parsed_args.push_back(n_temp);
        }
      }

      vec_of_args.push_back(parsed_args);

      if (shouldPrint) cout << "arguments_rule ended\n";
    }
  };

  template<> struct action < Instruction_call_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_call_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto vec_arg = vec_of_args.back();
      vec_of_args.pop_back();
      auto call = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_call(call, vec_arg);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_call_rule ended\n";
    }
  };

  template<> struct action < Instruction_call_assign_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_call_assign_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto vec_arg = vec_of_args.back();
      vec_of_args.pop_back();
      auto call = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_call_assign(dst, call, vec_arg);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_call_assign_rule ended\n";
    }
  };

  //te rules

  template<> struct action < Instruction_return_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_return_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_return();
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */
      if (shouldPrint) cout << "Instruction_return_rule ended\n";
    }
  };

  template<> struct action < Instruction_return_t_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_return_t_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto arg = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_return_t(arg);
      currentF->instructions.push_back(i);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      if (shouldPrint) cout << "Instruction_return_t_rule ended\n";
    }
  };

  template<> struct action < Instruction_label_rule > { 
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "var started\n";
      auto currentF = p.functions.back();
      // Basic_Block* b = new Basic_Block();
      auto lab = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_label(lab);
      // auto lab_temp = dynamic_cast<Label *>(lab);
      // b->label = lab_temp;
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "var ended\n";
    }
  };

  template<> struct action < Instruction_br_label_rule > { 
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_br_label_rule started\n";
      auto currentF = p.functions.back();

      auto lab = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_br_label(lab);
      currentF->instructions.push_back(i);

      if (shouldPrint) cout << "Instruction_br_label_rule ended\n";
    }
  };

  template<> struct action < Instruction_br_t_rule > { 
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_br_t_rule started\n";
      auto currentF = p.functions.back();

      auto lab = parsed_items.back();
      parsed_items.pop_back();
      auto t = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_br_t(t, lab);
      currentF->instructions.push_back(i);

      if (shouldPrint) cout << "Instruction_br_t_rule ended\n";
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