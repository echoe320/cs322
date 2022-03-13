/*
 * SUGGESTION FROM THE CC TEAM:
 * double check the order of actions that are fired.
 * You can do this in (at least) two ways:
 * 1) by using gdb adding breakpoints to actions
 * 2) by adding printing statements in each action
 *
 * For 2), we suggest writing the code to make it straightforward to enable/disable all of them 
 * (e.g., assuming shouldIPrint is a global variable
 *    if (shouldIPrint) std::cerr << "MY OUTPUT" << std::endl;
 * )
 */
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

#include <L2.h>
#include <L2_parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L2 {
  bool shouldPrint = false;

  /* 
   * Data required to parse
   */ 
  std::vector<Item *> parsed_items;

  /* 
   * Grammar rules from now on.
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

  // denoted as 'N' on L1 language slide
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
  
  struct label :
    pegtl::seq<
      pegtl::one<':'>,
      name
    > {};

  struct var :
    pegtl::seq<
      pegtl::one<'%'>,
      name
    > {};

  struct function_name :
    label {};

  struct argument_number :
    number {};

  struct comment : 
    pegtl::disable< 
      TAOCPP_PEGTL_STRING( "//" ), 
      pegtl::until< pegtl::eolf > 
    > {};

  struct Label_rule:
    label {};

  struct seps : 
    pegtl::star< 
      pegtl::sor< 
        pegtl::ascii::space, 
        comment 
      > 
    > {};
  
  /* 
   * Keywords.
   */
  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
  struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {}; //!DO SOMETHIGN ABOUT THIS"
  
  //Argument registers
  
  struct str_rdi : TAOCPP_PEGTL_STRING( "rdi" ) {};
  struct str_rsi : TAOCPP_PEGTL_STRING( "rsi" ) {};
  struct str_rdx : TAOCPP_PEGTL_STRING( "rdx" ) {};
  struct str_rcx : TAOCPP_PEGTL_STRING( "rcx" ) {};
  struct str_r8 : TAOCPP_PEGTL_STRING( "r8" ) {};
  struct str_r9 : TAOCPP_PEGTL_STRING( "r9" ) {};

  struct register_rdi_rule : str_rdi {};
  struct register_rsi_rule : str_rsi {};
  struct register_rdx_rule : str_rdx {};
  struct register_rcx_rule : str_rcx {};
  struct register_r8_rule : str_r8 {};
  struct register_r9_rule : str_r9 {};


  //Special registers
  struct str_rax : TAOCPP_PEGTL_STRING( "rax" ) {};
  struct str_rsp : TAOCPP_PEGTL_STRING( "rsp" ) {};

  struct register_rax_rule : str_rax {};
  struct register_rsp_rule : str_rsp {};

  struct w_rule :
    pegtl::sor<
      register_rdi_rule,
      register_rsi_rule,
      register_rdx_rule,
      register_rcx_rule,
      register_r8_rule,
      register_r9_rule,
      register_rax_rule,
      var
    > {};
  
  struct x_rule :
    pegtl::sor<
      w_rule,
      register_rsp_rule
    > {};

  struct t_rule :
    pegtl::sor<
      x_rule,
      number
    > {};

  // mem rule aka memory offsets
  struct str_mem : TAOCPP_PEGTL_STRING( "mem" ) {};

  struct offset : number {};

  struct mem_rule : 
    pegtl::seq<
      str_mem,
      seps,
      x_rule,
      seps,
      number
    > {};

  //------------------------ Arithmetic operations ------------------------

  struct str_plusEqual : TAOCPP_PEGTL_STRING( "+=" ) {};
  struct str_minusEqual : TAOCPP_PEGTL_STRING( "-=" ) {};
  struct str_multEqual : TAOCPP_PEGTL_STRING( "*=" ) {};
  struct str_bitAND : TAOCPP_PEGTL_STRING( "&=" ) {};
  struct str_inc : TAOCPP_PEGTL_STRING( "++" ) {};
  struct str_dec : TAOCPP_PEGTL_STRING( "--" ) {};

  struct plusEq_rule : str_plusEqual {};
  struct minusEq_rule : str_minusEqual {};
  struct multEq_rule : str_multEqual {};
  struct bitAND_rule : str_bitAND {};
  struct inc_rule : str_inc {};
  struct dec_rule : str_dec {};

  
  struct aop_rule :
    pegtl::sor<
      plusEq_rule,
      minusEq_rule,
      multEq_rule,
      bitAND_rule
    > {};

  struct crement_rule :
    pegtl::sor<
      inc_rule,
      dec_rule
    > {};

  //------------------------ Shift Operations ------------------------
  struct str_leftShift : TAOCPP_PEGTL_STRING( "<<=" ) {};
  struct str_rightShift : TAOCPP_PEGTL_STRING( ">>=" ) {};

  struct Lshift_rule : str_leftShift {};
  struct Rshift_rule : str_rightShift {};

  struct sop_rule : 
    pegtl::sor<
      Lshift_rule,
      Rshift_rule
    > {};
  
  //------------------------ Comparison operations ------------------------
  struct str_less : TAOCPP_PEGTL_STRING( "<" ) {};
  struct str_lessEqual : TAOCPP_PEGTL_STRING( "<=" ) {};
  struct str_equal : TAOCPP_PEGTL_STRING( "=" ) {};

  struct less_rule : str_less {};
  struct lessEq_rule : str_lessEqual {};
  struct equal_rule : str_equal {};

  struct cmp_rule :
    pegtl::sor<
      pegtl::seq< pegtl::at<lessEq_rule>, lessEq_rule>,
      pegtl::seq< pegtl::at<less_rule  >, less_rule  >,
      pegtl::seq< pegtl::at<equal_rule >, equal_rule >
    > {};
    
  //------------------------ Conditional Jumps ------------------------------
  struct str_cjump : TAOCPP_PEGTL_STRING ( "cjump" ) {};
  struct str_goto : TAOCPP_PEGTL_STRING ( "goto" ) {};


  //------------------------ Stack-Arg operation ------------------------------
  struct str_stackarg : TAOCPP_PEGTL_STRING ( "stack-arg" ) {};

  //------------------------ Call operations ------------------------------

  // Runtime operations
  // includes: print, input, allocate, tensor-error
  struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_allocate : TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_tensor_error : TAOCPP_PEGTL_STRING( "tensor-error" ) {};
  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};

  struct rt_print_rule : str_print {};
  struct rt_input_rule : str_input {};
  struct rt_allocate_rule : str_allocate {};
  struct rt_tensor_error_rule : str_tensor_error {};

  struct runtime_op_rule :
    pegtl::sor<
      rt_print_rule,
      rt_input_rule,
      rt_allocate_rule,
      rt_tensor_error_rule
    > {};

  //------------------------ Instruction rules ------------------------
  struct Instruction_return_rule :
    pegtl::seq<
      seps,
      str_return,
      seps
    > { };

  struct Instruction_assignment_rule : 
  //1. register <- register
  //2. mem to reg
  //3. reg to mem
  //4. assign number
  //5. assign comparison
  //6. mem <- ret_label
    pegtl::seq<
      seps,
      pegtl::sor<
        w_rule,
        mem_rule
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        w_rule,
        mem_rule,
        number,
        Label_rule
      >,
      seps
    > {};

  struct Instruction_arithmetic_rule :
    pegtl::seq<
      seps,
      pegtl::sor<
        w_rule,
        mem_rule
      >,
      seps,
      aop_rule,
      seps,
      pegtl::sor<t_rule, mem_rule>,
      seps
    > {};

  struct Instruction_shift_rule :
    pegtl::seq<
      seps,
      w_rule,
      seps,
      sop_rule,
      seps,
      pegtl::sor<
        w_rule, 
        number
      >,
      seps
    > {};

  struct Instruction_cmp_rule :
    pegtl::seq<
      seps,
      w_rule,
      seps,
      str_arrow,
      seps,
      t_rule,
      seps,
      cmp_rule,
      seps,
      t_rule,
      seps
    > {};

  struct Instruction_cjump_rule :
    pegtl::seq<
      seps,
      str_cjump,
      seps,
      t_rule,
      seps,
      cmp_rule,
      seps,
      t_rule,
      seps,
      Label_rule,
      seps
    > {};
    
  struct Instruction_call_rule :
    pegtl::seq<
      seps,
      str_call,
      seps,
      pegtl::sor<
        w_rule,
        Label_rule
        // runtime_op_rule
      >,
      seps,
      number,
      seps
    > {};

  struct Instruction_runtime_rule :
    pegtl::seq<
      seps,
      str_call,
      seps,
      runtime_op_rule,
      seps,
      number,
      seps
    > {};

  struct Instruction_crement_rule :
    pegtl::seq<
      seps,
      w_rule,
      seps,
      crement_rule,
      seps
    > {};

  struct Instruction_LEA_rule :
    pegtl::seq<
      seps,
      w_rule,
      seps,
      pegtl::one<'@'>,
      seps,
      w_rule,
      seps,
      w_rule,
      seps,
      number,
      seps
    > {};

  struct Instruction_label_rule :
    pegtl::seq<
      seps,
      Label_rule,
      seps
    > {};

  struct Instruction_goto_rule :
    pegtl::seq<
      seps,
      str_goto,
      seps,
      Label_rule,
      seps
    > {};

  struct Instruction_stackarg_rule :
    pegtl::seq<
      seps,
      w_rule,
      seps,
      str_arrow,
      seps,
      str_stackarg,
      seps,
      number
    > {};

  //============================ Umbrella Instruction Rule(s) ============================
  //1. return
  //2. assignment
  //3. arithmetic
  //4. shift
  //5. cjump
  //6. LEA
  //7. call
  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_return_rule>      , Instruction_return_rule       >,
      pegtl::seq< pegtl::at<Instruction_cjump_rule>       , Instruction_cjump_rule        >,
      pegtl::seq< pegtl::at<Instruction_cmp_rule>         , Instruction_cmp_rule          >,
      pegtl::seq< pegtl::at<Instruction_stackarg_rule>    , Instruction_stackarg_rule     >,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>  , Instruction_assignment_rule   >,
      pegtl::seq< pegtl::at<Instruction_arithmetic_rule>  , Instruction_arithmetic_rule   >,
      pegtl::seq< pegtl::at<Instruction_crement_rule>     , Instruction_crement_rule      >,
      pegtl::seq< pegtl::at<Instruction_shift_rule>       , Instruction_shift_rule        >,
      pegtl::seq< pegtl::at<Instruction_LEA_rule>         , Instruction_LEA_rule          >,
      pegtl::seq< pegtl::at<Instruction_runtime_rule>     , Instruction_runtime_rule      >,
      pegtl::seq< pegtl::at<Instruction_call_rule>        , Instruction_call_rule         >,
      pegtl::seq< pegtl::at<Instruction_goto_rule>        , Instruction_goto_rule         >,
      pegtl::seq< pegtl::at<Instruction_label_rule>       , Instruction_label_rule        >
    > {};

  struct Instructions_rule:
    pegtl::plus<
      pegtl::seq<
        seps,
        Instruction_rule,
        seps
      >
    > {};

  struct Function_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      function_name,
      seps,
      argument_number,
      seps,
      Instructions_rule,
      seps,
      pegtl::one< ')' >,
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

  struct entry_point_rule:
    pegtl::sor<
      pegtl::seq<
        seps,
        pegtl::one< '(' >,
        seps,
        label,
        seps,
        Functions_rule,
        seps,
        pegtl::one< ')' >,
        seps
      >

    > {};

  struct grammar : 
    pegtl::must<
      entry_point_rule
    > {};
    // pegtl::sor< 
    //   pegtl::seq< pegtl::at<entry_point_rule>, 
    //   entry_point_rule>,
    //   pegtl::seq< pegtl::at<Function_rule>, 
    //   Function_rule>
    // > {};

  struct function_file_grammer :
    pegtl::must< 
      Function_rule
    > {};
//================================= ACTIONS =================================
  /* 
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < label > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (p.entryPointLabel.empty()){
        p.entryPointLabel = in.string();
      } else {
        abort();
      }
    }
  };

  template<> struct action < function_name > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "function_name (no end)\n";
      auto newF = new Function();
      newF->name = in.string();
      p.functions.push_back(newF);
    }
  };

  template<> struct action < argument_number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "argument_number (no end)\n";
      auto currentF = p.functions.back();
      currentF->arguments = std::stoll(in.string());
    }
  };

  template<> struct action < number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "number started\n";
      Number* n = new Number(std::stoi(in.string()));
      // std::cout << n << "\n";
      parsed_items.push_back(n);
      if (shouldPrint) cout << "number ended\n";
    }
  };

  template<> struct action < Label_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "label_rule started\n";
      Label* l = new Label(in.string());
      parsed_items.push_back(l);
      if (shouldPrint) cout << "label_rule ended\n";
    }
  };

  // Register actions
  template<> struct action < register_rdi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_rdi_rule started\n";
      Register* regi = new Register(rdi);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_rdi_rule ended\n";
    }
  };

  template<> struct action < register_rsi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_rsi_rule started\n";
      Register* regi = new Register(rsi);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_rsi_rule ended\n";
    }
  };

  template<> struct action < register_rdx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_rdx_rule started\n";
      Register* regi = new Register(rdx);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_rdx_rule ended\n";
    }
  };

  template<> struct action < register_rcx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_rcx_rule started\n";
      Register* regi = new Register(rcx);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_rcx_rule ended\n";
    }
  };

  template<> struct action < register_r8_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_r8_rule started\n";
      Register* regi = new Register(r8);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_r8_rule ended\n";
    }
  };

  template<> struct action < register_r9_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_r9_rule started\n";
      Register* regi = new Register(r9);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_r9_rule ended\n";
    }
  };

  template<> struct action < register_rax_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_rax_rule started\n";
      Register* regi = new Register(rax);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_rax_rule ended\n";
    }
  };

  template<> struct action < var > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "var started\n";
      Variable* v = new Variable(in.string());
      parsed_items.push_back(v);
      if (shouldPrint) cout << "var ended\n";
    }
  };

  template<> struct action < register_rsp_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "register_rsp_rule started\n";
      Register* regi = new Register(rsp);
      parsed_items.push_back(regi);
      if (shouldPrint) cout << "register_rsp_rule ended\n";
    }
  };

  //Mem action
  template<> struct action < mem_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "mem_rule started\n";
      auto os = parsed_items.back();
      parsed_items.pop_back();
      auto rv = parsed_items.back();
      parsed_items.pop_back();
      Memory* mem = new Memory(rv, os);
      if (shouldPrint) cout << "mem_rule ended\n";

      parsed_items.push_back(mem);
    }
  };
  
  // aop_rule -> push
  template<> struct action < plusEq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "plusEq_rule started\n";
      Operation* op = new Operation(op_add);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "plusEq_rule ended\n";
    }
  };

  template<> struct action < minusEq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "minusEq_rule started\n";
      Operation* op = new Operation(op_minus);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "minnusEq_rule ended\n";
    }
  };

  template<> struct action < multEq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "plusEq_rule started\n";
      Operation* op = new Operation(op_multiply);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "multEq_rule ended\n";
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

  // crement_rule -> push
  template<> struct action < inc_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "inc_rule started\n";
      Operation* op = new Operation(op_inc);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "inc_rule ended\n";
    }
  };

  template<> struct action < dec_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "dec_rule started\n";
      Operation* op = new Operation(op_dec);
      parsed_items.push_back(op);
      if (shouldPrint) cout << "dec_rule ended\n";
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

  // cmp_rule -> push
  template<> struct action < less_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "cmp_less_rule started\n";
      Operation* op = new Operation(cmp_less);      
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "cmp_less_rule ended\n";
    }
  };

  template<> struct action < lessEq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "cmp_lessEq_rule started\n";
      Operation* op = new Operation(cmp_lesseq);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "cmp_lessEq_rule ended\n";
    }
  };

  template<> struct action < equal_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "cmp_equal_rule started\n";
      Operation* op = new Operation(cmp_equals);
      parsed_items.push_back(op);
      if (shouldPrint) std::cout << "cmp_equal_rule ended\n";
    }
  };

  // runtime_op_rule -> push
  template<> struct action < rt_print_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "rt_print_rule started\n";
      Runtime* rt = new Runtime(rt_print);
      parsed_items.push_back(rt);
      if (shouldPrint) std::cout << "rt_print_rule ended\n";
    }
  };

  template<> struct action < rt_input_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "rt_input_rule started\n";
      Runtime* rt = new Runtime(rt_input);
      parsed_items.push_back(rt);
      if (shouldPrint) std::cout << "rt_input_rule ended\n";
    }
  };

  template<> struct action < rt_allocate_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "rt_allocate_rule started\n";
      Runtime* rt = new Runtime(rt_allocate);
      parsed_items.push_back(rt);
      if (shouldPrint) std::cout << "rt_allocate_rule ended\n";
    }
  };

  template<> struct action < rt_tensor_error_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "rt_tensor_error_rule started\n";
      Runtime* rt = new Runtime(rt_tensor_error);
      parsed_items.push_back(rt);
      if (shouldPrint) std::cout << "rt_tensor_error_rule ended\n";
    }
  };

  // Instruction Actions -> pop
  template<> struct action < Instruction_return_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "return instruction started\n";
      auto currentF = p.functions.back();
      auto i = new Instruction_ret();
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "return instruction ended\n";
    }
  };

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
      auto i = new Instruction_assignment(src, dst);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_assignment_rule ended\n";
    }
  };

  // Arithmetic actions
  template<> struct action < Instruction_arithmetic_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_arithmetic_rule started\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_arithmetic(src, dst, op);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_arithmetic_rule ended\n";
    }
  };

  //crement rule
  template<> struct action < Instruction_crement_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_crement_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_crement(dst, op);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_crement_rule ended\n";
    }
  };

  // Shift actions
  template<> struct action < Instruction_shift_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_shift_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_shift(src, dst, op);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_shift_rule ended\n";
    }
  };

  // cmp actions

  template<> struct action < Instruction_cmp_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_cmp_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */
      auto arg2 = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto arg1 = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_cmp(dst, arg1, arg2, op);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_cmp_rule ended\n";
    }
  };

  // cjump actions
  template<> struct action < Instruction_cjump_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) std::cout << "Instruction_cjump_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto label = parsed_items.back();
      parsed_items.pop_back();
      auto arg2 = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto arg1 = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_cjump(arg1, arg2, label, op);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) std::cout << "Instruction_cjump_rule ended\n";
    }
  };

  // lea action
  template<> struct action < Instruction_LEA_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_LEA_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto multiple = parsed_items.back();
      parsed_items.pop_back();
      auto arg2 = parsed_items.back();
      parsed_items.pop_back();
      auto arg1 = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_lea(dst, arg1, arg2, multiple);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_LEA_rule ended\n";
    }
  };

  // lea action
  template<> struct action < Instruction_label_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_label_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      // cout << "This: " << parsed_items.back().labelName << endl;
      auto label = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_label(label);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_label_rule ended\n";
    }
  };
  
  // goto action
  template<> struct action < Instruction_goto_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_goto_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto label = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_goto(label);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_goto_rule ended\n";
    }
  };

  // stackarg action
  template<> struct action < Instruction_stackarg_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "Instruction_stackarg_rule started\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto M = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_stackarg(dst, M);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_stackarg_rule ended\n";
    }
  };

  //call action
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
      auto N = parsed_items.back();
      parsed_items.pop_back();
      auto u = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_calls(u, N);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_call_rule ended\n";
    }
  };

  //RUNTIME call action
  template<> struct action < Instruction_runtime_rule > {
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
      auto N = parsed_items.back();
      parsed_items.pop_back();
      auto runtime = parsed_items.back();
      parsed_items.pop_back();
      auto i = new Instruction_runtime(runtime, N);

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      if (shouldPrint) cout << "Instruction_call_rule ended\n";
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

  Program parse_function_file (char *fileName){

    /* 
     * Check the grammar for some possible issues.
     */
    if (shouldPrint) std::cout << "checking grammar for possible issues" << std::endl;
    pegtl::analyze< function_file_grammer >();

    /*
     * Parse.
     */
    if (shouldPrint) std::cout << "begin parsing" << std::endl;
    file_input< > fileInput(fileName);
    Program p;
    parse< function_file_grammer, action >(fileInput, p);
    if (shouldPrint) std::cout << "end parsing" << std::endl;

    return p;
  }

  Program parse_spill_file (char *fileName){

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