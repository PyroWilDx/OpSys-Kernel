#include <cpu.h>
#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "ecran.h"
#include "temps.h"
#include "processus.h"
#include "processus_code.h"

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1)
    {
        res = 1;
    }
    else
    {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    reset_ecran();

    init_traitant_IT(32, &traitant_IT_32);

    masque_IRQ(0, false);

    // sti();

    init_idle();

    // for (int i = 1; i < 10; i++) {
    //     char nom[PROC_MAX_NAME_LENGTH];
    //     sprintf(nom, "proc%d", i);
    //     int32_t pid = cree_processus(&proc_gen, nom);
    //     if (pid == -1) printf("Failed to create processus : %s.\n", nom);
    //     else printf("Created processus : %s.\n", nom);
    // }

    // Endormissement
    int32_t pid1 = cree_processus(&proc1, "proc1");
    if (pid1 == -1)
        printf("Failed to create a process.\n");
    int32_t pid2 = cree_processus(&proc2, "proc2");
    if (pid2 == -1)
        printf("Failed to create a process.\n");
    int32_t pid3 = cree_processus(&proc3, "proc3");
    if (pid3 == -1)
        printf("Failed to create a process.\n");
    int32_t pid4 = cree_processus(&proc4, "proc4");
    if (pid4 == -1)
        printf("Failed to create a process.\n");
    int32_t pid5 = cree_processus(&proc5, "proc5");
    if (pid5 == -1)
        printf("Failed to create a process.\n");
    int32_t pid6 = cree_processus(&proc6, "proc6");
    if (pid6 == -1)
        printf("Failed to create a process.\n");

    // Création dynamique
    // int32_t pid_creator = cree_processus(&proc_creator, "procCreator");
    // if (pid_creator == -1) printf("Failed to create a process.\n");

    // Donut
    // int32_t pid_donut = cree_processus(&proc_donut, "procDonut");
    // if (pid_donut == -1) printf("Failed to create a process.\n");

    // Infini
    // int32_t pid1 = cree_processus(&proc1, "proc1");
    // if (pid1 == -1) printf("Failed to create a process.\n");
    // int32_t pid2 = cree_processus(&proc2, "proc2");
    // if (pid2 == -1) printf("Failed to create a process.\n");
    // int32_t pid_infinite = cree_processus(&proc_infinite, "procInfinite");
    // if (pid_infinite == -1) printf("Failed to create a process.\n");

    // Priorité
    // for (int i = 1; i < 4; i++) {
    //     char nom[PROC_MAX_NAME_LENGTH];
    //     sprintf(nom, "proc%d", i);
    //     int32_t pid = cree_processus_priotity(&proc_gen, nom, i * 2);
    //     if (pid == -1) printf("Failed to create processus : %s.\n", nom);
    // }

    idle();

    // for (int i = 2; i < 102; i++) {
    //     if (i % 2 == 0) {
    //         printf("%d ", i);
    //     }
    //     if (i % 10 == 0) {
    //         printf("\n");
    //     }
    // }

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
