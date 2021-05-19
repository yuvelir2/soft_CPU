#ifndef MY_CALLOC_FREE_H_INCLUDED
#define MY_CALLOC_FREE_H_INCLUDED


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void* my_calloc (FILE* f, size_t kol, size_t size_of_elem, int line, const char* func, const char* file)
{
    void* ptr = calloc (kol, size_of_elem);

    if (ptr == NULL)
    {
        fprintf (f, "Not enough memory accessible\n");
    }
    else
    {
        fprintf (f, "%p -- alloc new given pointer on   %3d line %s func %s\n\n", ptr, line, strrchr (file, '\\'), func);
    }
    return ptr;
}

void my_free (FILE* f, void* mem, int line, const char* func, const char* file)
{
    fprintf (f,     "%p -- call to free this block from %3d line %s func %s\n", mem, line, strrchr (file, '\\'), func);

    free (mem);
    return;
}


#endif // MY_CALLOC_FREE_H_INCLUDED
