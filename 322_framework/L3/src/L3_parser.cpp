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
#include <map> //optional

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L3_parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
// using namespace std;

bool shouldPrint = false;
extern bool is_debug;
namespace L3 {
  /*
   * Data required to parse
   */
  
  std::vector<Item *> parsed_items = {};
  std::vector<Item *> list_of_args = {}; 
  std::vector<Item *> parameter_list = {}; 

  /*
   * Keywords.
   */
  struct str_return : TAOCPP_PEGTL_STRING("return") {};
  struct str_arrow : TAOCPP_PEGTL_STRING("<-") {};

  /*
   * Basics
   */

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
  
  struct var :
    pegtl::seq<
      pegtl::one<'%'>,
      name
    > {};
  
  struct var_rule:
    var {};

  struct str_load : TAOCPP_PEGTL_STRING("load") {};
  struct str_store : TAOCPP_PEGTL_STRING("store") {};
  struct str_br : TAOCPP_PEGTL_STRING("br") {};

  struct vars_rule :
    pegtl::sor<
      pegtl::seq<
        pegtl::sor<
          var
        >,
        pegtl::star<
          pegtl::seq<
            seps,
            pegtl::one<','>,
            seps,
            pegtl::sor<
              var
            >
          >
        >
      >,
      seps
    > {};
  
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

  /* Instruction_assignment_rule */
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

  struct args_rule :
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

  struct Instruction_load_rule : pegtl::seq<
                                     var_rule,
                                     seps,
                                     str_arrow,
                                     seps,
                                     str_load,
                                     seps,
                                     var_rule>
  {
  };

  struct Instruction_store_rule : pegtl::seq<
                                      str_store,
                                      seps,
                                      var_rule,
                                      seps,
                                      str_arrow,
                                      seps,
                                      pegtl::sor<
                                          number_rule,
                                          label_rule,
                                          var_rule>>
  {
  };

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

  struct Instruction_label_rule :
    pegtl::seq<
      seps,
      label,
      seps
    > {};

  struct Instruction_br_label_rule : pegtl::seq<
                                     str_br,
                                     seps,
                                     label_rule>
  {
  };
  struct Instruction_br_t_rule : pegtl::seq<
                                     str_br,
                                     seps,
                                     pegtl::sor<var_rule, number_rule>,
                                     seps, 
                                     label_rule>
  {
  };

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
  
  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_op_rule>          , Instruction_op_rule           >,
      pegtl::seq< pegtl::at<Instruction_load_rule>        , Instruction_load_rule         >,
      pegtl::seq< pegtl::at<Instruction_store_rule>       , Instruction_store_rule        >,
      pegtl::seq< pegtl::at<Instruction_call_rule>        , Instruction_call_rule         >,
      pegtl::seq< pegtl::at<Instruction_call_assign_rule> , Instruction_call_assign_rule  >,
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
   * Function Rule 
   */
  
  // Function Components 

  struct str_define : TAOCPP_PEGTL_STRING( "define" ) {};
  
  struct function_name :
    label_rule {};

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

  template<> struct action < function_name > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (shouldPrint) cout << "function_name action started\n";
      auto newF = new Function();
      // auto newF = p.functions.back();
      // if (in.string() == ":main") currentF->isMain = true;
      newF->name = in.string();
      p.functions.push_back(newF);
      if (shouldPrint) cout << "function_name action ended\n";
    }
  };

  template <>
  struct action<Instruction_return_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_ret: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_ret_not();
      i->op = dynamic_cast<Operation*>(parsed_items.back()); 
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };
  template <>
  struct action<Instruction_return_t_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_ret_t: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_ret_t();
      i->arg = parsed_items.back(); 
      parsed_items.pop_back();
      i->op = dynamic_cast<Operation*>(parsed_items.back()); 
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
      if(is_debug) cout << i->toString() << endl;
    }
  };
  template <>
  struct action<label_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      Label *i = new Label(in.string());
      parsed_items.push_back(i);   
    }
  };

  template <>
  struct action<var_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing var_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      std::string var_name = in.string(); 
      Variable *i = currentF->newVariable(var_name);
      parsed_items.push_back(i);
    }
  };    

template <>
  struct action<vars_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing vars_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      std::string vars = in.string(); 
      while(vars.find(',') != vars.npos){
          int n = vars.find(','); 
          //eliminate any space in 0-n
          std::string temp = vars.substr(0, n); 
          temp.erase(std::remove_if(temp.begin(), temp.end(), [](unsigned char x){return std::isspace(x);}), temp.end()); 
          if(temp.size() == 0) continue;
          else if(temp[0] == '%'){
            Variable *i = currentF->newVariable(temp);
            currentF->variables[temp] = i;
            currentF->arguments.push_back(i);
          }
          else {
              Number* i = new Number(std::stoll(temp)); 
              currentF->arguments.push_back(i);
          }
          vars = vars.substr(n+1);
      }
      vars.erase(std::remove_if(vars.begin(), vars.end(), [](unsigned char x){return std::isspace(x);}), vars.end()); 
      if(is_debug) cout << "vars after parsed: " << vars << " size: " << vars.size() << endl;
      if(vars.size() == 0) return ;
      else if(vars[0] == '%'){
          Variable* i = currentF->newVariable(vars); 
          currentF->variables[vars] = i; 
          currentF->arguments.push_back(i);
      }
      else {
         Number* i = new Number(std::stoll(vars)); 
        currentF->arguments.push_back(i);
      }
    }
  };
template <>
  struct action<args_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing args_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      std::string args = in.string(); 
      if(args.size() == 0) return ;
      while(args.find(',') != args.npos){
          int n = args.find(','); 
          //eliminate any space in 0-n
          std::string temp = args.substr(0, n); 
          temp.erase(std::remove_if(temp.begin(), temp.end(), [](unsigned char x){return std::isspace(x);}), temp.end()); 
          if(is_debug) cout << "args: " << temp << endl;
          if(temp[0] == '%'){
            Variable *i = currentF->newVariable(temp);
            currentF->variables[temp] = i;
            list_of_args.push_back(i);
          }
          else {
              Number* i = new Number(std::stoll(temp)); 
              list_of_args.push_back(i);
          }
          args = args.substr(n+1);
      }
      if(is_debug) cout << "args after parsed: " << args << endl;
      args.erase(std::remove_if(args.begin(), args.end(), [](unsigned char x){return std::isspace(x);}), args.end()); 
      if(args[0] == '%'){
          Variable* i = currentF->newVariable(args); 
          currentF->variables[args] = i; 
          list_of_args.push_back(i);
      }
      else {
         Number* i = new Number(std::stoll(args)); 
        list_of_args.push_back(i);
      }
    }
  };
  // action for + - & * << >>
  template <>
  struct action<op_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing op_rule: " << in.string() << endl;
      Operation *i = new Operation(in.string());
      parsed_items.push_back(i);
    }
  };
  template <>
  struct action<str_return>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing op_rule: " << in.string() << endl;
      Operation *i = new Operation(in.string());
      parsed_items.push_back(i);
    }
  };
  template <>
  struct action<str_br>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing op_rule: " << in.string() << endl;
      Operation *i = new Operation(in.string());
      parsed_items.push_back(i);
    }
  };
  template <>
  struct action<str_load>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing op_rule: " << in.string() << endl;
      Operation *i = new Operation(in.string());
      parsed_items.push_back(i);
    }
  };
  template <>
  struct action<str_store>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing op_rule: " << in.string() << endl;
      Operation *i = new Operation(in.string());
      parsed_items.push_back(i);
    }
  };
  // action when value is a number
  template <>
  struct action<number_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing number_rule: " << in.string() << endl;
      Number *i = new Number(std::stoll(in.string()));
      parsed_items.push_back(i);
    }
  };

  //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              action when value is a print, allocate, input, tensor-error
  template <>
  struct action<call_string_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing call_string_rule: " << in.string() << endl;
      String *i = new String(in.string());
      parsed_items.push_back(i);
    }
  };

  // action for :label
  template <>
  struct action<Instruction_label_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_label_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_label();
      Label *item = new Label(in.string());
      i->label = item;
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };
  // action for var <- t op t
  template <>
  struct action<Instruction_op_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_op_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_math();
      i->oprand2 = parsed_items.back();
      parsed_items.pop_back();
      i->op = parsed_items.back();
      parsed_items.pop_back();
      i->oprand1 = parsed_items.back();
      parsed_items.pop_back();
      i->dst = dynamic_cast<Variable*>(parsed_items.back());
      if(dynamic_cast<Variable*>(parsed_items.back()) == nullptr) cout << "NULL" <<endl;
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };
  /*
   call actions
  */
  // action for call callee (args)
  template <>
  struct action<Instruction_call_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_call_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_call_noassign();
      reverse(list_of_args.begin(), list_of_args.end());
      while(!list_of_args.empty()) {
          i->args.push_back(list_of_args.back());
          list_of_args.pop_back();
          parsed_items.pop_back();
      }
      i->callee = parsed_items.back();
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };

  template <>
  struct action<Instruction_call_assign_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_call_assign_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_call_assignment();
      for(Item* item : list_of_args) {
         i->args.push_back(item);
         parsed_items.pop_back();
      }
      list_of_args = {};
      i->callee = parsed_items.back();
      parsed_items.pop_back();
      i->dst = dynamic_cast<Variable*>(parsed_items.back());;
       parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  // action for br label
  template <>
  struct action<Instruction_br_label_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_br_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_br_label();
      i->label = dynamic_cast<Label*>(parsed_items.back());;
      parsed_items.pop_back();
      i->op = dynamic_cast<Operation*>(parsed_items.back());;
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
      if(is_debug) cout << i->toString() << endl;
    }
  };
  // action for br t label
  template <>
  struct action<Instruction_br_t_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_br_t_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      auto i = new Instruction_br_t();
      i->label = dynamic_cast<Label*>(parsed_items.back());;
      parsed_items.pop_back();
      i->condition = parsed_items.back();
      parsed_items.pop_back();
      i->op = dynamic_cast<Operation*>(parsed_items.back());;
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
      if(is_debug) cout << i->toString() << endl;
    }
  };

  template <>
  struct action<Instruction_assignment_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_assignment_rule: " << in.string() << endl;
      auto currentF = p.functions.back();

      auto i = new Instruction_assignment();
      i->src = parsed_items.back();
      parsed_items.pop_back();
      i->dst = dynamic_cast<Variable*>(parsed_items.back());
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      /*
       * Add the just-created instruction to the current function.
       */
      currentF->instructions.push_back(i);
    }
  };

  template <>
  struct action<Instruction_load_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_load_rule: " << in.string() << endl;

      auto currentF = p.functions.back();

      auto i = new Instruction_load();
      i->src = dynamic_cast<Variable*>(parsed_items.back());
      parsed_items.pop_back();
      i->op = dynamic_cast<Operation*>(parsed_items.back()); 
      parsed_items.pop_back();
      i->dst = dynamic_cast<Variable*>(parsed_items.back());
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };
  template <>
  struct action<Instruction_store_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_store_rule: " << in.string() << endl;

      auto currentF = p.functions.back();

      auto i = new Instruction_store();
      i->src = parsed_items.back();
      parsed_items.pop_back();
      i->dst = parsed_items.back();
      parsed_items.pop_back();
      i->op = dynamic_cast<Operation*>(parsed_items.back()); 
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };


  Program parse_file(char *fileName) {
    /*
     * Check the grammar for some possible issues.
     */
    pegtl::analyze<grammar>();

    /*
     * Parse.
     */
    file_input< > fileInput(fileName);
    Program p;
    parse<grammar, action>(fileInput, p);

    return p;
  }
}
