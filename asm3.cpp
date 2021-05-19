//Q: size_of_binary_buff


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <ctype.h>

#include "enum.h"


const size_t Max_label_kol = 10;
const size_t Max_label_size = 20;
const size_t Size_of_bb = 1000;
const int Jmp = 90;


struct label
{
    char name [Max_label_size];
    int adr;
};


char* Read_buff (FILE* f_mem);
void parse_arg (char** buff, char** curr);
bool find_jump_by_label (char label_name[Max_label_size], label label_arr[Max_label_kol], int* adr);
void Dump_label_arr (label* label_arr);
void skip_probs (char** buff);
void coming_through_buff (char* buff, char* curr, label* label_arr);
char* buff_to_asm (FILE* f_mem, char* buff);
void Write_in_file (char* binary_buff);


int main ()
{
    FILE* f_mem = fopen ("mem_info.txt", "w");

    char* buff = Read_buff (f_mem);

    printf ("%s$$ asm done! $$\n", buff);

    char* binary_buff = buff_to_asm (f_mem, buff);

    my_free (f_mem, buff, INFO);

    Write_in_file (binary_buff);

    my_free (f_mem, binary_buff, INFO);

    return 0;
}


char* Read_buff (FILE* f_mem)
{
    char file_name[100] = "";
    int size_of_file = 0;

    #ifndef DEBUG
        scanf ("%s", file_name);
    #endif

    ON_DEBUG(strcpy (file_name, "input.txt"));

    FILE* fin = fopen (file_name, "r");

    fseek (fin, 0, SEEK_END) == 0;
    size_of_file = ftell (fin);
    rewind (fin);

    char* buff = (char*) my_calloc (f_mem, size_of_file, sizeof (char), INFO);

    fread (buff, sizeof (char), size_of_file, fin);

    fclose (fin);

    return buff;
}

void parse_arg (char** buff, char** curr)
{
    Arg arg_set = {};

    if (**buff == '[')
    {
        arg_set.mem = 1;

        *buff += 1;//eaten [
    }

    if (*(*buff + 1) == 'x')
    {
        arg_set.reg = 1;

        switch (**buff)
        {
            case 'a':
                arg_set.reg_num = AX_CODE;
                break;

            case 'b':
                arg_set.reg_num = BX_CODE;
                break;

            case 'c':
                arg_set.reg_num = CX_CODE;
                break;

            case 'd':
                arg_set.reg_num = DX_CODE;
                break;
        }

        *buff += 2;//eaten reg
    }

    if (**buff == '+')
    {
        arg_set.plus = 1;

        *buff += 1;
    }

    if (isdigit(**buff) || (**buff == '-'))
    {
        arg_set.num = 1;

        int len_of_number = 0;

        sscanf (*buff, "%d%n", &arg_set.number, &len_of_number);

        *buff += len_of_number;
    }

    if ((arg_set.mem == 1) && (**buff == ']'))
    {
        *buff += 1;
    }

    skip_probs (buff);

    memmove (*curr, &arg_set, sizeof (Arg));

    *curr += sizeof (Arg);

    return;
}

bool find_jump_by_label (char label_name[Max_label_size], label label_arr[Max_label_kol], int* adr)
{
    for (int i = 0; i < Max_label_kol; ++i)
    {
        if (strncmp (label_arr[i].name, label_name, strlen (label_name)) == 0)
        {
            *adr = label_arr[i].adr;

            return true;
        }
    }

    *adr = 0;
    return false;
}

void Dump_label_arr (label* label_arr)
{
    for (int i = 0; i < Max_label_kol; ++i)
    {
        if (strcmp (label_arr[i].name, "") != 0)
            printf ("Метка %s ведет на %d ячейку\n", label_arr[i].name, label_arr[i].adr);
    }
}

//! Skips spaces, new line and comments

void skip_probs (char** buff)
{
    while (**buff == ' ' || **buff == '\n' || **buff == '\r')
        *buff += 1;

    if (**buff == '/')
    {
        while (**buff != '\n')
            *buff += 1;
    }
    *buff++;

    return;
}

void coming_through_buff (char* buff, char* curr, label* label_arr)
{
    char* bb_beg = curr;

    int step = 0;
    int free = 0;//index of free place for new label

    while (*buff != '\0')
    {
        #define DEF_CMD(name, len_of_name, num, cpu_code)               \
                                                                        \
            if (strncmp (buff, #name, len_of_name) == 0)                \
            {                                                           \
                *curr = num;                                            \
                curr++;                                                 \
                buff += len_of_name;                                    \
                skip_probs (&buff);                                     \
                                                                        \
                if ((num == CMD_PUSH) || (num == CMD_POP))              \
                {                                                       \
                    parse_arg (&buff, &curr);                           \
                }                                                       \
                else if (num >= Jmp)                                    \
                {                                                       \
                    char label_name[Max_label_size] = "";               \
                                                                        \
                    sscanf (buff, "%s%n", label_name, &step);           \
                                                                        \
                    buff += step;                                       \
                                                                        \
                    int adr = 0;                                        \
                                                                        \
                    find_jump_by_label (label_name, label_arr, &adr);   \
                                                                        \
                    *(int*) curr = adr;                                 \
                    printf ("Записали адрес %d метки %s в бин буф\n", adr, label_name);      \
                                                                        \
                    curr += sizeof (int);                               \
                }                                                       \
            }

        if (*buff == ':')
        {
            buff++;

            sscanf (buff, "%s%n", label_arr[free].name, &step);
            label_arr[free].adr = curr - bb_beg;

            free++;

            buff += step;
        }

        #include "com.h"

        skip_probs (&buff);
    }

    #undef DEF_CMD

    return;
}

char* buff_to_asm (FILE* f_mem, char* buff)
{
    char* binary_buff = (char*) my_calloc (f_mem, Size_of_bb, sizeof (char), __LINE__, __PRETTY_FUNCTION__, __FILE__);

    label* label_arr = (label*) my_calloc (f_mem, Max_label_kol, sizeof (label), __LINE__, __PRETTY_FUNCTION__, __FILE__);

    coming_through_buff (buff, binary_buff, label_arr);

    Dump_label_arr (label_arr);

    coming_through_buff (buff, binary_buff, label_arr);

    my_free (f_mem, label_arr, __LINE__, __PRETTY_FUNCTION__, __FILE__);

    return binary_buff;
}

void Write_in_file (char* binary_buff)
{
    FILE* fout = fopen ("output.cpu", "wb");

    fwrite (binary_buff, sizeof (char), Size_of_bb, fout);

    fclose (fout);
}
