#define DEF_CMD(name, len_of_name, num, cpu_code) \
    CMD_##name = num,

enum Commands
{
    #include "com.h"
};


#undef DEF_CMD


enum Registers
{
    AX_CODE = 1,
    BX_CODE,
    CX_CODE,
    DX_CODE,
};


struct Arg
{
    unsigned mem : 1;
    unsigned plus : 1;
    unsigned num : 1;
    unsigned reg : 1;
    unsigned reg_num : 4;
    int number;
};


#include "D:\Downloads\TX\TXLib.h"
#include "my_calloc_free.h"


#define INFO __LINE__, __PRETTY_FUNCTION__, __FILE__

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else
    #define ON_DEBUG(code)  ;
#endif
