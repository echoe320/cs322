!============== CHANGELOG ==============
1. register to register i.e. rdi += rax
2. single register i.e. rdi-- or rdi++
3. Arithmetic operations in memory
    1. register to mem i.e. rdi -= mem rsp 8
    2. mem to register
4. added second arg arithmetic for rsp and numbers
5. shift operations
6. comparison operations
7. Call operations
    1. Runtime operations i.e. print, input, allocate, tensor-error
    2. bigger call_rule
8. add t_rule


# Todo:
## Parser:
### Structure
- Registers
- Memory
- Label
- Instructions
    - return
    - assignment
    - arithmetic
    - shift
    - conditional jumps
    - LEA
    - calls
    
### General
- make sure every pegtl::seq has seps in between each R

### Structs we don't need action rules for:
- any of the str_ structs
- name
- t_rule
- argRegister
- number
- 

### Structs we DO need action rules for:
- all register_xxx_rules
- register_rsp_rule
- label
- mem_rule
- function name
- argument_number



Code_generation:
