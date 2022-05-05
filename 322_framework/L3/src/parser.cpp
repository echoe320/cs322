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
#include <map>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L3.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

extern bool is_debug;
namespace L3
{
  /*
   * Data required to parse
   */
  std::vector<Item *> parsed_items = {};
  std::vector<Item *> list_of_args = {}; 
  std::vector<Item *> parameter_list = {}; 

  /*
   *map from string to registerid
   */

  /*
   * Grammar rules from now on.
   */
  struct name : pegtl::seq<
                    pegtl::plus<
                        pegtl::sor<
                            pegtl::alpha,
                            pegtl::one<'_'>>>,
                    pegtl::star<
                        pegtl::sor<
                            pegtl::alpha,
                            pegtl::one<'_'>,
                            pegtl::digit>>>
  {
  };

  /*
   * Keywords.
   */
  struct str_return : TAOCPP_PEGTL_STRING("return") {};
  struct str_load : TAOCPP_PEGTL_STRING("load") {};
  struct str_store : TAOCPP_PEGTL_STRING("store") {};
  struct str_call : TAOCPP_PEGTL_STRING("call") {};
  struct str_br : TAOCPP_PEGTL_STRING("br") {};
  struct str_arrow : TAOCPP_PEGTL_STRING("<-") {};

  struct comment : pegtl::disable<
                       TAOCPP_PEGTL_STRING("//"),
                       pegtl::until<pegtl::eolf>> {};

  struct label : pegtl::seq<
                     pegtl::one<':'>,
                     name> {};
  struct seps : pegtl::star<
                    pegtl::sor<
                        pegtl::ascii::space,
                        comment>> {};
  struct number : pegtl::seq<
                      pegtl::opt<
                          pegtl::sor<
                              pegtl::one<'-'>,
                              pegtl::one<'+'>>>,
                      pegtl::plus<
                          pegtl::digit>> {};
  struct variable : pegtl::seq<
                             pegtl::one<'%'>,
                             name> {};
  struct function_name : label {};

  struct number_rule : number {};

  struct variable_rule : variable {};
  // struct parameter_rule :variable {};
  struct variables_rule :   pegtl::sor<
                                pegtl::seq<
                                    variable,
                                    pegtl::star<
                                        pegtl::seq<
                                            seps,
                                            pegtl::one<','>,
                                            seps,
                                            variable,
                                            seps
                                                >
                                            >
                                        >,
                                variable,
                                seps
                                > {};
  struct args_rule :   pegtl::sor<
                                pegtl::seq<
                                    pegtl::sor<variable_rule, number_rule>,
                                    pegtl::star<
                                        pegtl::seq<
                                            pegtl::one<','>,
                                            pegtl::sor<seps, pegtl::eol>,
                                            pegtl::sor<variable_rule, number_rule>
                                                >
                                            >
                                        >,
                                seps
                                    > {};

  struct Label_rule : label {};
  
  struct op_rule : pegtl::sor<TAOCPP_PEGTL_STRING("<<"), 
                            TAOCPP_PEGTL_STRING(">>"),
                            pegtl::one<'+'>, 
                            pegtl::one<'-'>, 
                            pegtl::one<'*'>,
                            pegtl::one<'&'>
                            >
  {
  };

  struct Instruction_return_rule : pegtl::seq<
                                       str_return> {};
  struct Instruction_return_t_rule : pegtl::seq<
                                       str_return,
                                       seps,
                                       pegtl::sor<variable_rule, 
                                                  number_rule>> {};

  struct Instruction_assignment_rule : pegtl::seq<
                                           variable_rule,
                                           seps,
                                           str_arrow,
                                           seps,
                                           pegtl::sor<
                                               number_rule,
                                               Label_rule,
                                               variable_rule>>
  {
  };

  struct Instruction_math_rule : pegtl::seq<
                                      variable_rule,
                                      seps,
                                      str_arrow,
                                      seps, 
                                      pegtl::sor<variable_rule, number_rule>,
                                      seps,
                                      op_rule,
                                      seps,
                                      pegtl::sor<number_rule, variable_rule>>
  {
  };

  struct Instruction_load_rule : pegtl::seq<
                                     variable_rule,
                                     seps,
                                     str_arrow,
                                     seps,
                                     str_load,
                                     seps,
                                     variable_rule>
  {
  };

  struct Instruction_store_rule : pegtl::seq<
                                      str_store,
                                      seps,
                                      variable_rule,
                                      seps,
                                      str_arrow,
                                      seps,
                                      pegtl::sor<
                                          number_rule,
                                          Label_rule,
                                          variable_rule>>
  {
  };

  struct compare_op_rule : pegtl::sor<
                               TAOCPP_PEGTL_STRING(">="),
                               TAOCPP_PEGTL_STRING("<="),
                               TAOCPP_PEGTL_STRING("<"),
                               TAOCPP_PEGTL_STRING(">"),
                               TAOCPP_PEGTL_STRING("=")>
  {
  };

  struct Instruction_compare_rule : pegtl::seq<
                                        variable_rule,
                                        seps,
                                        str_arrow,
                                        seps,
                                        pegtl::sor<number_rule, variable_rule>,
                                        seps,
                                        compare_op_rule,
                                        seps,
                                        pegtl::sor<number_rule, variable_rule>>
  {
  };
  /*
  call
  */
  struct call_string_rule : pegtl::sor<
                                TAOCPP_PEGTL_STRING("print"), 
                                TAOCPP_PEGTL_STRING("allocate"), 
                                TAOCPP_PEGTL_STRING("input"), 
                                TAOCPP_PEGTL_STRING("tensor-error")> {};
  struct Instruction_call_rule : pegtl::seq<
                                     str_call,
                                     seps,
                                     pegtl::sor<
                                         Label_rule,
                                         variable_rule,
                                         call_string_rule>,
                                     seps,
                                     TAOCPP_PEGTL_STRING("("),
                                     pegtl::sor<seps, pegtl::eol>,
                                     args_rule,
                                     pegtl::sor<seps, pegtl::eol>,
                                     TAOCPP_PEGTL_STRING(")")>
  {
  };
  struct Instruction_call_assignment_rule : pegtl::seq<
                                    variable_rule, 
                                    seps, 
                                    str_arrow, 
                                    seps, 
                                    str_call,
                                    seps,
                                     pegtl::sor<
                                         Label_rule,
                                         variable_rule,
                                         call_string_rule>,
                                    seps,
                                     TAOCPP_PEGTL_STRING("("),
                                     pegtl::sor<seps, pegtl::eol>,
                                     args_rule,
                                     pegtl::sor<seps, pegtl::eol>,
                                     TAOCPP_PEGTL_STRING(")")>
  {
  };

  struct Instruction_br_label_rule : pegtl::seq<
                                     str_br,
                                     seps,
                                     Label_rule>
  {
  };
  struct Instruction_br_t_rule : pegtl::seq<
                                     str_br,
                                     seps,
                                     pegtl::sor<variable_rule, number_rule>,
                                     seps, 
                                     Label_rule>
  {
  };
  struct Instruction_label_rule : label
  {
  };
  struct Instruction_rule : pegtl::sor<
                                pegtl::seq<pegtl::at<Instruction_label_rule>, Instruction_label_rule>,
                                pegtl::seq<pegtl::at<Instruction_br_label_rule>, Instruction_br_label_rule>,
                                pegtl::seq<pegtl::at<Instruction_br_t_rule>, Instruction_br_t_rule>,
                                pegtl::seq<pegtl::at<Instruction_return_t_rule>, Instruction_return_t_rule>,
                                pegtl::seq<pegtl::at<Instruction_return_rule>, Instruction_return_rule>,
                                pegtl::seq<pegtl::at<Instruction_compare_rule>, Instruction_compare_rule>,
                                pegtl::seq<pegtl::at<Instruction_math_rule>, Instruction_math_rule>,
                                pegtl::seq<pegtl::at<Instruction_load_rule>, Instruction_load_rule>,
                                pegtl::seq<pegtl::at<Instruction_store_rule>, Instruction_store_rule>,
                                pegtl::seq<pegtl::at<Instruction_call_rule>, Instruction_call_rule>,
                                pegtl::seq<pegtl::at<Instruction_call_assignment_rule>, Instruction_call_assignment_rule>,
                                pegtl::seq<pegtl::at<Instruction_assignment_rule>, Instruction_assignment_rule>>
  {
  };

  struct Instructions_rule : pegtl::plus<
                                 pegtl::seq<
                                     seps,
                                     Instruction_rule,
                                     seps>>
  {
  };

  struct Function_rule : pegtl::seq<
                             TAOCPP_PEGTL_STRING("define"),
                             seps,
                             function_name,
                             seps,
                             TAOCPP_PEGTL_STRING("("),
                             seps,
                             variables_rule,
                             seps,
                             TAOCPP_PEGTL_STRING(")"),
                             seps,
                             TAOCPP_PEGTL_STRING("{"),
                             seps,
                             Instructions_rule,
                             seps,
                            TAOCPP_PEGTL_STRING("}")>
  {
  };

  struct Functions_rule : pegtl::plus<
                              seps,
                              Function_rule,
                              seps>
  {
  };

  struct grammar : pegtl::must<
                       Functions_rule>
  {
  };
  /*
   * Actions attached to grammar rules.
   */
  template <typename Rule>
  struct action : pegtl::nothing<Rule>
  {
  };

  template <>
  struct action<Function_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug) cout << "firing Function_rule" << endl;
    }
  };

  // template <>
  // struct action<parameter_rule>
  // {
  //   template <typename Input>
  //   static void apply(const Input &in, Program &p)
  //   {
  //     auto currentF = p.functions.back();
  //     //parameter_list.push_back(in.string());
  //     Variable *v = currentF->newVariable(in.string());
  //     currentF->arguments.push_back(v);
  //   }
  // };

  template <>
  struct action<function_name>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug) cout << "new function: " << in.string() << endl;
      auto newF = new Function();
      newF->name = in.string();
      newF->isMain = in.string() == ":main";
      parameter_list = {};
      p.functions.push_back(newF);
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
  struct action<Label_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      Label *i = new Label(in.string());
      parsed_items.push_back(i);   
    }
  };

  template <>
  struct action<variable_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing variable_rule: " << in.string() << endl;
      auto currentF = p.functions.back();
      std::string var_name = in.string(); 
      Variable *i = currentF->newVariable(var_name);
      parsed_items.push_back(i);
    }
  };    

template <>
  struct action<variables_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing variables_rule: " << in.string() << endl;
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
  struct action<Instruction_math_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_math_rule: " << in.string() << endl;
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
  struct action<Instruction_call_assignment_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_call_assignment_rule: " << in.string() << endl;
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

  template <>
  struct action<compare_op_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      // if (is_debug)
      //   cout << "firing compare_op_rule: " << in.string() << endl;
      Operation *i = new Operation(in.string());
      parsed_items.push_back(i);
    }
  };

  template <>
  struct action<Instruction_compare_rule>
  {
    template <typename Input>
    static void apply(const Input &in, Program &p)
    {
      if (is_debug)
        cout << "firing Instruction_compare_rule: " << in.string() << endl;
      auto currentF = p.functions.back();

      auto i = new Instruction_compare();
      i->oprand2 = parsed_items.back();
      parsed_items.pop_back();
      i->op = parsed_items.back();
      parsed_items.pop_back();
      i->oprand1 = parsed_items.back();
      parsed_items.pop_back();
      i->dst = dynamic_cast<Variable*>(parsed_items.back());
      parsed_items.pop_back();
      if(is_debug) cout << i->toString() << endl;
      currentF->instructions.push_back(i);
    }
  };

  Program parse_file(char *fileName)
  {

    /*
     * Check the grammar for some possible issues.
     */
    pegtl::analyze<grammar>();

    /*
     * Parse.
     */
    file_input<> fileInput(fileName);
    Program p;
    parse<grammar, action>(fileInput, p);

    return p;
  }

}
