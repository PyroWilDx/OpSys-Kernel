#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "processus_code.h"
#include "processus.h"
#include "temps.h"

void idle(void)
{
    for (;;)
    {
        sti();
        hlt();
        cli();
    }
}

void proc_gen(void)
{
    for (;;)
    {
        printf("[%s] Pid=%i | basePrio=%d | Prio=%d\n", mon_nom(), mon_pid(), mon_base_prio(), mon_prio());
        sti();
        hlt();
        cli();
    }
}

void proc1(void)
{
    for (uint32_t i = 0; i < 30; i++)
    {
        printf("[Temps = %u] Processus = %s | Pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(1);
    }
    printf("Self-Kill: Processus = %s | Pid = %i\n", mon_nom(), mon_pid());
    fin_processus();
}

void proc2(void)
{
    for (;;)
    {
        printf("[Temps = %u] Processus = %s | Pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(2);
    }
}

void proc3(void)
{
    for (uint32_t i = 0; i < 2; i++)
    {
        printf("[Temps = %u] Processus = %s | Pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(3);
    }
    printf("Self-Kill: Processus = %s | Pid = %i\n", mon_nom(), mon_pid());
    fin_processus();
}

void proc4(void)
{
    for (;;)
    {
        printf("[Temps = %u] Processus = %s | Pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(4);
    }
}

void proc5(void)
{
    for (uint32_t i = 0; i < 6; i++)
    {
        printf("[Temps = %u] Processus = %s | Pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(5);
    }
    printf("Self-Kill: Processus = %s | Pid = %i\n", mon_nom(), mon_pid());
    fin_processus();
}

void proc6(void)
{
    for (;;)
    {
        printf("[Temps = %u] Processus = %s | Pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(6);
    }
}

void proc_creator(void)
{
    cree_processus(&proc3, "proc3Created1");
    cree_processus(&proc3, "proc3Created2");
    cree_processus(&proc3, "proc3Created3");
    cree_processus(&proc3, "proc3Created4");
    cree_processus(&proc3, "proc3Created5");
    cree_processus(&proc3, "proc3Created6");
    cree_processus(&proc3, "proc3Created7");
    cree_processus(&proc3, "proc3Created8");
    cree_processus(&proc3, "proc3Created9");
    cree_processus(&proc3, "proc3Created10");
    cree_processus(&proc3, "proc3Created11");
    if (cree_processus(&proc3, "proc3Created12") == -1)
        printf("Merde\n");

    dors(16);

    cree_processus(&proc3, "proc3Created13");
    cree_processus(&proc3, "proc3Created14");
    cree_processus(&proc3, "proc3Created15");
    cree_processus(&proc3, "proc3Created16");
    cree_processus(&proc3, "proc3Created17");
    cree_processus(&proc3, "proc3Created18");

    dors(12);

    cree_processus(&proc6, "proc6Created1");
    cree_processus(&proc6, "proc6Created2");
    cree_processus(&proc6, "proc6Created3");
    cree_processus(&proc6, "proc6Created4");
    cree_processus(&proc6, "proc6Created5");
    cree_processus(&proc6, "proc6Created6");
    cree_processus(&proc6, "proc6Created7");
    cree_processus(&proc6, "proc6Created8");
    cree_processus(&proc6, "proc6Created9");
    cree_processus(&proc6, "proc6Created10");
    cree_processus(&proc6, "proc6Created11");

    dors(16);
}

double abs(double x)
{
    if (x < 0)
        return -x;
    return x;
}

double cos(double x)
{
    double t, s;
    int p;
    p = 0;
    s = 1.0;
    t = 1.0;
    while (abs(t / s) > 0.1)
    {
        p++;
        t = (-t * x * x) / ((2 * p - 1) * (2 * p));
        s += t;
    }
    return s;
}

double sin(double x)
{
    double res = 0, pow = x, fact = 1;
    for (int i = 0; i < 10; ++i)
    {
        res += pow / fact;
        pow *= -1 * x * x;
        fact *= (2 * (i + 1)) * (2 * (i + 1) + 1);
    }

    return res;
}

void proc_donut(void)
{
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    (void)z;
    char b[1760];
    for (;;)
    {
        memset(b, 32, 1760);
        memset(z, 0, 7040);
        for (j = 0; j < 6.28; j += 0.07)
        {
            for (i = 0; i < 6.28; i += 0.02)
            {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y = 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        for (k = 0; k < 1761; k++)
        {
            printf("%c", k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
    }
}

void proc_infinite(void)
{
    dors(6);
    uint32_t done = 0;
    for (;;)
    {
        uint32_t secs = nbr_secondes();
        if (secs % 2 == 0)
        {
            if (done == 0)
            {
                printf("Infinite\n");
                done = 1;
            }
        }
        if (secs % 2 == 1)
        {
            done = 0;
        }
        sti();
        hlt();
        cli();
    }
}
