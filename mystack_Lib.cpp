#include <stdio.h>
#include <assert.h>
#include <string.h>


typedef int Elem_t;

const int SIZE_OF_STACK = 10;

const int POISON = -666;

const int NAME_LEN = 100;

const char canary ='$';


#define INFORMFDUMP __FILE__, __PRETTY_FUNCTION__, __LINE__

#define STACK(x) Stack_t x;               \
                 StackConstructor(&x, #x)

#define DUMP(x, reason) Dump ((x), (x)->name, __LINE__, __PRETTY_FUNCTION__, (reason))

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else
    #define ON_DEBUG
#endif


struct Stack_t
{
    char canary_begin;

    int size_of_fulled_stack;
    char name[NAME_LEN];
    Elem_t data[SIZE_OF_STACK];
    char errno;

    char canary_end;
};


void Dump (const Stack_t* stk_adr)
{
    printf ("\t" "size = %d" "\n", stk_adr->size_of_fulled_stack);
    printf ("\t" "data[%d] = %#x" "\n\t{\n", SIZE_OF_STACK, &stk_adr->data[0]);
    for (int i = 0; i < SIZE_OF_STACK; ++i)
    {
        if (stk_adr->data[i] != POISON)
            printf ("\t\t*[%d] = %d\n\n", i, stk_adr->data[i]);
        else
            printf ("\t\t [%d] = %d [POISON]\n\n", i, stk_adr->data[i]);
    }
    printf ("\t}\n}\n\n");
}

void StackConstructor (Stack_t* stk_adr, const char* name)
{
    strncpy (stk_adr->name, name, NAME_LEN);

    for (int i = 0; i < SIZE_OF_STACK; ++i)
        stk_adr->data[i] = POISON;

    stk_adr->size_of_fulled_stack = 0;

    stk_adr->canary_begin = canary;
    stk_adr->canary_end = canary;
}

void StackPush (Stack_t* stk_adr, Elem_t t)
{
    assert (stk_adr->size_of_fulled_stack < SIZE_OF_STACK);

    stk_adr->data[stk_adr->size_of_fulled_stack] = t;

    ++stk_adr->size_of_fulled_stack;
}

Elem_t  StackPop (Stack_t* stk_adr)
{
    assert (stk_adr->size_of_fulled_stack > 0);

    --stk_adr->size_of_fulled_stack;

    Elem_t t = stk_adr->data[stk_adr->size_of_fulled_stack];

    stk_adr->data[stk_adr->size_of_fulled_stack] = POISON;

    return t;
}

void StackDestruct (Stack_t* stk_adr)
{
        for (int i = 0; i < SIZE_OF_STACK; ++i)
            stk_adr->data[i] = POISON;
        stk_adr->size_of_fulled_stack = 0;
}

bool stk_ok (Stack_t* stk_adr)
{
    if ((stk_adr->canary_begin != '$') or (stk_adr->canary_end != '$'))
        return false;

    if (stk_adr->size_of_fulled_stack < 0)
        return false;

    return true;
}
