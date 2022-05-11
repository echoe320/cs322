#include <architecture.h>
#include <string> 
#include <unordered_map>
#include <algorithm>
#include <map>

using namespace std;

namespace L2 {
    unordered_map<Architecture::RegisterID, string> register_to_string = 
    { 
        {Architecture::rax, "rax"}, 
        {Architecture::rbx, "rbx"}, 
        {Architecture::rcx, "rcx"}, 
        {Architecture::rdx, "rdx"}, 
        {Architecture::rdi, "rdi"}, 
        {Architecture::rsi, "rsi"}, 
        {Architecture::rbp, "rbp"}, 
        {Architecture::r8, "r8"}, 
        {Architecture::r9, "r9"}, 
        {Architecture::r10, "r10"}, 
        {Architecture::r11, "r11"}, 
        {Architecture::r12, "r12"}, 
        {Architecture::r13, "r13"}, 
        {Architecture::r14, "r14"}, 
        {Architecture::r15, "r15"}, 
        {Architecture::rsp, "rsp"}
    }; 
    vector<Architecture::RegisterID> argument_regs = {
        Architecture::rdi, 
        Architecture::rsi, 
        Architecture::rdx,
        Architecture::rcx,
        Architecture::r8,
        Architecture::r9 };
    vector<Architecture::RegisterID> caller_saved_regs = {
        Architecture::rdi, 
        Architecture::rsi, 
        Architecture::rdx,
        Architecture::rcx,
        Architecture::r8,
        Architecture::r9,
        Architecture::r10,
        Architecture::r11,
        Architecture::rax
         };
    vector<Architecture::RegisterID> callee_saved_regs = {
        Architecture::rbp, 
        Architecture::rbx, 
        Architecture::r12,
        Architecture::r13,
        Architecture::r14,
        Architecture::r15
         };

    std::map<Architecture::RegisterID, Architecture::Color> r_to_color = {
        {Architecture::rax, Architecture::red}, 
        {Architecture::rbx, Architecture::orange}, 
        {Architecture::rcx, Architecture::yellow}, 
        {Architecture::rdx, Architecture::green}, 
        {Architecture::rdi, Architecture::blue}, 
        {Architecture::rsi, Architecture::purple}, 
        {Architecture::rbp, Architecture::pink}, 
        {Architecture::r8, Architecture::black}, 
        {Architecture::r9, Architecture::white}, 
        {Architecture::r10, Architecture::grey}, 
        {Architecture::r11, Architecture::brown}, 
        {Architecture::r12, Architecture::lime}, 
        {Architecture::r13, Architecture::bronze}, 
        {Architecture::r14, Architecture::golden}, 
        {Architecture::r15, Architecture::silver}
    };
    std::map<Architecture::Color, Architecture::RegisterID> color_to_r = {
        {Architecture::red, Architecture::rax}, 
        {Architecture::orange, Architecture::rbx}, 
        {Architecture::yellow, Architecture::rcx}, 
        {Architecture::green, Architecture::rdx}, 
        {Architecture::blue, Architecture::rdi}, 
        {Architecture::purple, Architecture::rsi}, 
        {Architecture::pink, Architecture::rbp}, 
        {Architecture::black, Architecture::r8}, 
        {Architecture::white, Architecture::r9}, 
        {Architecture::grey, Architecture::r10}, 
        {Architecture::brown, Architecture::r11}, 
        {Architecture::lime, Architecture::r12}, 
        {Architecture::bronze, Architecture::r13}, 
        {Architecture::golden, Architecture::r14}, 
        {Architecture::silver, Architecture::r15}
    };
    std::string Architecture::fromRegisterToString(Architecture::RegisterID r) {
        return register_to_string[r]; 
    }
    Architecture::Color Architecture::fromRegisterToColor(Architecture::RegisterID r){
        return r_to_color[r];
    }
    Architecture::RegisterID Architecture::fromColorToRegister(Architecture::Color c){
        return color_to_r[c];
    }
    bool Architecture::isArgument(Architecture::RegisterID r){
        if(find(argument_regs.begin(), argument_regs.end(), r) != argument_regs.end()) return true; 
        return false;
    }
    bool Architecture::isCallerSaved(Architecture::RegisterID r){
        if(find(caller_saved_regs.begin(), caller_saved_regs.end(), r) != caller_saved_regs.end()) return true; 
        return false;
    } 
    bool Architecture::isCalleeSaved(Architecture::RegisterID r){
        if(find(callee_saved_regs.begin(), callee_saved_regs.end(), r) != callee_saved_regs.end()) return true; 
        return false;
    }      
    bool Architecture::isGeneralPurpose(Architecture::RegisterID r){
        if(find(argument_regs.begin(), argument_regs.end(), r) != argument_regs.end()) return true; 
        if(find(callee_saved_regs.begin(), callee_saved_regs.end(), r) != callee_saved_regs.end()) return true; 
        return false;
    }
    vector<Architecture::RegisterID> Architecture::get_caller_saved_regs() {
        return caller_saved_regs;
    }
    vector<Architecture::RegisterID> Architecture::get_callee_saved_regs() {
        return callee_saved_regs;
    }
    vector<Architecture::RegisterID> Architecture::get_argument_regs() {
        return argument_regs;
    }
}