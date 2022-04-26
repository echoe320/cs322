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

#include <L1.h>
#include <L2.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L1 {

  /* 
   * Data required to parse
   */ 
  std::vector<Item> parsed_items;

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

    struct function_name :
    label {};

  struct argument_number :
    number {};

  struct local_number :
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

  //===================== jacobie defined =====================
  

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

  //? do we need to differentiate this? if not, we can abstract argRegister to register
  struct argRegister :
    pegtl::sor<
      register_rdi_rule,
      register_rsi_rule,
      register_rdx_rule,
      register_rcx_rule,
      register_r8_rule,
      register_r9_rule
    > {};

  //Result registers

  struct str_rax : TAOCPP_PEGTL_STRING( "rax" ) {};
  struct register_rax_rule : str_rax {};

  //Caller save -> also includes rdi, rsi, rdx, rcx, r8, r9, rax

  struct str_r10 : TAOCPP_PEGTL_STRING( "r10" ) {};
  struct str_r11 : TAOCPP_PEGTL_STRING( "r11" ) {};

  struct register_r10_rule : str_r10 {};
  struct register_r11_rule : str_r11 {};
  
  //Callee save -> also includes 
  
  struct str_r12 : TAOCPP_PEGTL_STRING( "r12" ) {};
  struct str_r13 : TAOCPP_PEGTL_STRING( "r13" ) {};
  struct str_r14 : TAOCPP_PEGTL_STRING( "r14" ) {};
  struct str_r15 : TAOCPP_PEGTL_STRING( "r15" ) {};
  struct str_rbp : TAOCPP_PEGTL_STRING( "rbp" ) {};
  struct str_rbx : TAOCPP_PEGTL_STRING( "rbx" ) {};

  struct register_r12_rule : str_r12 {};
  struct register_r13_rule : str_r13 {};
  struct register_r14_rule : str_r14 {};
  struct register_r15_rule : str_r15 {};
  struct register_rbp_rule : str_rbp {};
  struct register_rbx_rule : str_rbx {};

  //Registers
  struct str_rsp : TAOCPP_PEGTL_STRING( "rsp" ) {};
  struct register_rsp_rule : str_rsp {};

  struct register_rule :
    pegtl::sor<
      argRegister,
      register_rax_rule,
      register_rbx_rule,
      register_rbp_rule,
      register_r10_rule,
      register_r11_rule,
      register_r12_rule,
      register_r13_rule,
      register_r14_rule,
      register_r15_rule
    > {};

  struct t_rule :
    pegtl::sor<
      register_rule,
      register_rsp_rule,
      number
    > {};

  // mem rule aka memory offsets
  struct str_mem : TAOCPP_PEGTL_STRING( "mem" ) {};

  struct mem_rule : 
    pegtl::seq<
      str_mem,
      seps,
      pegtl::sor<register_rule, register_rsp_rule>,
      seps,
      pegtl::digit 
    > {};

  //struct register_rdi_rule:
  //     str_rdi {};
    
  // struct register_rax_rule:
  //     str_rax {};
   
  // struct register_rule:
  //   pegtl::sor<
  //     register_rdi_rule,
  //     register_rax_rule
  //   > {};

  //------------------------ Arithmetic operations ------------------------

  struct str_plusEqual : TAOCPP_PEGTL_STRING( "+=" ) {};
  struct str_minusEqual : TAOCPP_PEGTL_STRING( "-=" ) {};
  struct str_multEqual : TAOCPP_PEGTL_STRING( "*=" ) {};
  struct str_bitAND : TAOCPP_PEGTL_STRING( "&=" ) {};
  struct str_inc : TAOCPP_PEGTL_STRING( "++" ) {};
  struct str_dec : TAOCPP_PEGTL_STRING( "--" ) {};
  
  struct aop_rule :
    pegtl::sor<
      str_plusEqual,
      str_minusEqual,
      str_multEqual,
      str_bitAND
    > {};

  struct crement_rule :
    pegtl::sor<
      str_inc, 
      str_dec
    > {};

  //------------------------ Shift Operations ------------------------
  struct str_leftShift : TAOCPP_PEGTL_STRING( "<<=" ) {}; //!DO SOMETHIGN ABOUT THIS"
  struct str_rightShift : TAOCPP_PEGTL_STRING( ">>=" ) {};

  struct sop_rule : 
    pegtl::sor<
      str_leftShift,
      str_rightShift
    > {};
  
  //------------------------ Comparison operations ------------------------
  struct str_less : TAOCPP_PEGTL_STRING( "<" ) {};
  struct str_lessEqual : TAOCPP_PEGTL_STRING( "<=" ) {};
  struct str_equal : TAOCPP_PEGTL_STRING( "=" ) {};

  struct cmp_rule :
    pegtl::sor<
      str_less,
      str_lessEqual,
      str_equal
    > {};
    
  //------------------------ Conditional Jumps ------------------------------
  struct str_cjump : TAOCPP_PEGTL_STRING ( "cjump" ) {};
  struct str_goto : TAOCPP_PEGTL_STRING ( "goto" ) {};


  //------------------------ LEA operation ------------------------------

  struct str_at : TAOCPP_PEGTL_STRING ( "@" ) {};

  //------------------------ Call operations ------------------------------

  // Runtime operations
  // includes: print, input, allocate, tensor-error
  struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_allocate : TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_tensor_error : TAOCPP_PEGTL_STRING( "tensor-error" ) {};
  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};

  struct runtime_op_rule :
    pegtl::sor<
      str_print,
      str_input,
      str_allocate,
      str_tensor_error
    > {};

  //------------------------ Instruction rules ------------------------
  struct Instruction_return_rule :
    pegtl::seq<
      str_return
    > { };

  struct Instruction_assignment_rule : // mem rsp 0 <- rdi
  //1. register <- register
  //2. mem to reg
  //3. reg to mem
  //4. assign number
  //5. assign comparison
    pegtl::seq<
      pegtl::sor<
        register_rule,
        mem_rule
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        register_rule,
        mem_rule,
        number,
        label
      >
    > {};

  struct Instruction_arithmetic_rule :
    pegtl::seq<
      pegtl::sor<
        register_rule,
        mem_rule
      >,
      seps,
      aop_rule,
      seps,
      t_rule
    > {};

  struct Instruction_shift_rule :
    pegtl::seq<
      register_rule,
      seps,
      sop_rule,
      seps,
      pegtl::sor<
        register_rcx_rule, 
        number
      > //? why is sx only rcx?
    > {};

  struct Instruction_cmp_rule :
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      t_rule, 
      seps, 
      cmp_rule, 
      seps, 
      t_rule
    > {};

  struct Instruction_cjump_rule :
    pegtl::seq<
      str_cjump,
      seps,
      t_rule,
      seps,
      cmp_rule,
      seps,
      t_rule,
      seps,
      label
    > {};
    
  struct Instruction_call_rule :
    pegtl::seq<
      str_call,
      seps,
      pegtl::sor<
        register_rule,
        label,
        runtime_op_rule
      >,
      seps,
      number
    > {};

  struct Instruction_crement_rule :
    pegtl::seq<
      register_rule,
      seps,
      crement_rule
    > {};

  struct Instruction_LEA_rule :
    pegtl::seq<
      register_rule,
      seps,
      register_rule,
      seps,
      register_rule,
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
      pegtl::seq< pegtl::at<Instruction_assignment_rule>  , Instruction_assignment_rule   >,
      pegtl::seq< pegtl::at<Instruction_arithmetic_rule>  , Instruction_arithmetic_rule   >,
      pegtl::seq< pegtl::at<Instruction_crement_rule>     , Instruction_crement_rule      >,
      pegtl::seq< pegtl::at<Instruction_shift_rule>       , Instruction_shift_rule        >,
      pegtl::seq< pegtl::at<Instruction_cjump_rule>       , Instruction_cjump_rule        >,
      pegtl::seq< pegtl::at<Instruction_LEA_rule>         , Instruction_LEA_rule          >,
      pegtl::seq< pegtl::at<Instruction_call_rule>        , Instruction_call_rule         >
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
      local_number,
      seps,
      Instructions_rule,
      seps,
      pegtl::one< ')' >
    > {};

  struct Functions_rule: // covers multiple functions
    pegtl::plus<
      seps,
      Function_rule,
      seps
    > {};

  struct entry_point_rule:
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
    > {};

  struct grammar : 
    pegtl::must< 
      entry_point_rule
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
      auto newF = new Function();
      newF->name = in.string();
      p.functions.push_back(newF);
    }
  };

  template<> struct action < argument_number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      currentF->arguments = std::stoll(in.string());
    }
  };

  template<> struct action < local_number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      currentF->locals = std::stoll(in.string());
    }
  };

  template<> struct action < number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = false;
      i.isMem = false;
      i.isNum = true;
      i.offset = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < str_return > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_ret();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Label_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = false;
      i.isMem = false;
      i.isNum = false;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };
  
  // Register Actions all
  template<> struct action < register_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isMem = false;
      i.isNum = false;
      i.Register = in.string();
      parsed_items.push_back(i);
    }
  };

  // // Register Actions rsp
  // template<> struct action < register_rsp_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.isNum = false;
  //     i.Register = in.string();
  //     parsed_items.push_back(i);
  //   }
  // };

  // // Register Actions rsp
  // template<> struct action < register_rcx_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.isNum = false;
  //     i.Register = in.string();
  //     parsed_items.push_back(i);
  //   }
  // };

  // Register Actions -> push
  // template<> struct action < register_rdi_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.Register = rdi;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rsi_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = rsi;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rdx_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.Register = rdx;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rcx_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.Register = rcx;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r8_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r8;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r9_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r9;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rax_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = rax;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rbx_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = rbx;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rdi_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = rdi;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rbp_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = rbp;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r10_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r10;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r11_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r11;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r12_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r12;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r13_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r13;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r14_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r14;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_r15_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = r15;
  //     parsed_items.push_back(i);
  //   }
  // };

  // template<> struct action < register_rsp_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = true;
  //     i.isMem = false;
  //     i.r = rsp;
  //     parsed_items.push_back(i);
  //   }
  // };

  //Mem action
  // template<> struct action < mem_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     Item i;
  //     i.isARegister = false;
  //     i.isMem = true;
  //     i.isNum = false;
  //     i.Register = in[2].string();
  //     i.offset = in[4].string();
  //     parsed_items.push_back(i);
  //   }
  // };
  // // aop_rule -> push
  // template<> struct action < aop_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     operation i;
  //     i.op = in.string();
  //     parsed_items.push_back(i);
  //   }
  // };

  // // crement_rule -> push
  // template<> struct action < crement_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     operation i;
  //     i.op = in.string();
  //     parsed_items.push_back(i);
  //   }
  // }; 

  // // sop_rule -> push
  // template<> struct action < sop_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     operation i;
  //     i.op = in.string();
  //     parsed_items.push_back(i);
  //   }
  // };  

  // // cmp_rule -> push
  // template<> struct action < cmp_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     operation i;
  //     i.op = in.string();
  //     parsed_items.push_back(i);
  //   }
  // };  

  // // runtime_op_rule -> push
  // template<> struct action < runtime_op_rule > {
  //   template< typename Input >
  //   static void apply( const Input & in, Program & p){
  //     operation i;
  //     i.op = in.string();
  //     parsed_items.push_back(i);
  //   }
  // };

  // Instruction Actions -> pop
  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_assignment();
      i->src = parsed_items.back(); //mem
      parsed_items.pop_back();
      i->dst = parsed_items.back();
      parsed_items.pop_back();

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  // // Arithmetic actions
  // template<> struct action < Instruction_arithmetic_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_arithmetic();
  //     i->src = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->op = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->dst = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };

  // //crement rule
  // template<> struct action < Instruction_crement_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_crement();
  //     i->op = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->dst = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };

  // // Shift actions
  // template<> struct action < Instruction_shift_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_shift();
  //     i->src = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->op = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->dst = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };

  // // cmp actions

  // template<> struct action < Instruction_cmp_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_cmp();
  //     i->arg2 = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->op = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->arg1 = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->dst = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };

  // // cjump actions
  // template<> struct action < Instruction_cjump_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_cjump();
  //     i->label = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->arg2 = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->op = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->arg1 = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };

  // // lea action
  // template<> struct action < Instruction_LEA_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_lea();
  //     i->multiple = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->arg2 = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->arg1 = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->dst = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };

  // //call action
  // template<> struct action < Instruction_call_rule > {
  //   template< typename Input >
	// static void apply( const Input & in, Program & p){

  //     /* 
  //      * Fetch the current function.
  //      */ 
  //     auto currentF = p.functions.back();

  //     /* 
  //      * Create the instruction.
  //      */ 
  //     auto i = new Instruction_calls();
  //     i->N = parsed_items.back();
  //     parsed_items.pop_back();
  //     i->u = parsed_items.back();
  //     parsed_items.pop_back();

  //     /* 
  //      * Add the just-created instruction to the current function.
  //      */ 
  //     currentF->instructions.push_back(i);
  //   }
  // };



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
