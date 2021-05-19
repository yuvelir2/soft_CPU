//!----------------------------------------------------------------------------
//!DEF_CMD (name, len_of_name, enum number, cpu_code)
//!----------------------------------------------------------------------------


DEF_CMD(PUSH, 4, 1,
    {
        Arg arg_set = {};

        memmove (&arg_set, buff, sizeof (Arg));

        buff += sizeof (Arg);

        if (arg_set.mem == 1)
        {
            StackPush (&stk, mem[reg[arg_set.reg_num] + arg_set.number]);
        }
        else
        {
            StackPush (&stk, reg[arg_set.reg_num] + arg_set.number * 100);
        }
        skip_probs (&buff);

       printf("PUSH:\n");
       Dump (&stk);
    })

DEF_CMD(POP, 3, 2,
    {
        Arg arg_set = {};

        memmove (&arg_set, buff, sizeof (Arg));

        buff += sizeof (Arg);

        if (arg_set.mem == 1)
        {
            mem[reg[arg_set.reg_num] + arg_set.number] = StackPop (&stk);
        }
        else if (arg_set.reg == 1)
        {
            reg[arg_set.reg_num] = StackPop (&stk);
        }
        else
        {
            Elem_t t = StackPop (&stk);
        }

    printf ("POP:\n");
       Dump (&stk);
    })

DEF_CMD(ADD, 3, 3,
    {
        StackPush (&stk, StackPop (&stk) + StackPop (&stk));

        printf ("ADD:\n");
        Dump (&stk);
    })

DEF_CMD(SUB, 3, 4,
    {
        Elem_t t = StackPop (&stk);
        StackPush (&stk, StackPop (&stk) - t);

       Dump (&stk);
    })

DEF_CMD(MUL, 3, 5,
    {
        StackPush (&stk, StackPop (&stk) * StackPop (&stk) / 100);
    })

DEF_CMD(DIV, 3, 6,
    {
        Elem_t t = StackPop (&stk);
        StackPush (&stk, StackPop (&stk) * 100 / t);
    })

DEF_CMD(IN, 2, 7,
    {
        float t = 0;

        scanf ("%f", &t);

        StackPush (&stk, (int) (t * 100));
    })

DEF_CMD(OUT, 3, 8,
    {
        int t = StackPop (&stk);
        $i
        printf ("Rez: mul100: %d, normal:%.2f\n", t, (float) t / 100);
        $d
    })

DEF_CMD(SQRT, 4, 9,
    {
        Elem_t t = (Elem_t) (sqrt (StackPop (&stk)) * 10);
        assert (t >= 0);
        StackPush (&stk, t);
    })

DEF_CMD(SIN, 3, 20,
    {
        printf ("SIN: ");
        StackPush (&stk, (Elem_t) (sin ((double) StackPop (&stk) / 100) * 100));
    })

DEF_CMD(END, 3, 50,
    {
        printf ("!!!EEEENDDD!!!\n");
        return;
    })

DEF_CMD(JMP, 3, 90,
    {
        int adr = *(int*) buff;

        buff = buff_beg + adr;
    })

DEF_CMD(JA, 2, 91,
    {
        if (StackPop (&stk) > StackPop (&stk))
        {
            int adr = *(int*) buff;

            buff = buff_beg + adr;
        }
        else
        {
            buff += sizeof (int);
        }
    })

DEF_CMD(JAE, 3, 92,
    {
        if (StackPop (&stk) >= StackPop (&stk))
        {
            int adr = *(int*) buff;

            buff = buff_beg + adr;
        }
        else
        {
            buff += sizeof (int);
        }
    })

DEF_CMD(JB, 2,93,
    {
        if (StackPop (&stk) < StackPop (&stk))
        {
            int adr = *(int*) buff;

            buff = buff_beg + adr;
        }
        else
        {
            buff += sizeof (int);
        }
    })

DEF_CMD(JBE, 3, 94,
    {
        if (StackPop (&stk) <= StackPop (&stk))
        {
            int adr = *(int*) buff;

            buff = buff_beg + adr;
        }
        else
        {
            buff += sizeof (int);
        }
    })

DEF_CMD(JE, 2, 95,
    {
        if (StackPop (&stk) == StackPop (&stk))
        {
            int adr = *(int*) buff;

            buff = buff_beg + adr;
        }
        else
        {
            buff += sizeof (int);
        }
    })

DEF_CMD(JNE, 3, 96,
    {
        if (StackPop (&stk) != StackPop (&stk))
        {
            int adr = *(int*) buff;

            buff = buff_beg + adr;
        }
        else
        {
            buff += sizeof (int);
        }
    })

DEF_CMD(CALL, 4, 97,
    {
        int adr = *(int*) buff;

        buff += sizeof (int);

        StackPush (&stk_call, (int) (buff - buff_beg));
        printf ("Return to %d\n",(int) (buff - buff_beg));

        buff = buff_beg + adr;
        printf ("Now jump to %d\n", adr);

    })

DEF_CMD(RET, 3, 42,
    {
        buff = StackPop (&stk_call) + buff_beg;
    })
