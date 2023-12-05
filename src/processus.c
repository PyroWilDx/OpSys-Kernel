#include <stdio.h>
#include <cpu.h>
#include <string.h>
#include <tinyalloc.h>
#include "processus.h"
#include "processus_code.h"
#include "temps.h"

Processus *tab_proc[PROC_TABLE_SIZE] = {};
uint32_t cpt_proc = 0;

Processus *actif = NULL;

ProcLSC activable_LSC = {};
ProcLSC endormi_LSC = {};
ProcLSC zombie_LSC = {};

void init_processus(void)
{
    for (uint32_t i = 0; i < PROC_TABLE_SIZE; i++)
    {
        tab_proc[i] = NULL;
    }
    activable_LSC.head = NULL;
    activable_LSC.queue = NULL;
    endormi_LSC.head = NULL;
    endormi_LSC.queue = NULL;
    zombie_LSC.head = NULL;
    zombie_LSC.queue = NULL;
}

void init_idle(void)
{
    tab_proc[cpt_proc] = malloc(sizeof(Processus));
    Processus *curr_proc = tab_proc[cpt_proc];
    curr_proc->pid = cpt_proc;
    strcpy(curr_proc->nom, "Idle");
    curr_proc->etat = ETAT_ELU;
    curr_proc->wake_time = 0;
    curr_proc->suiv = NULL;
    curr_proc->base_priority = MAX_PRIORITY;
    curr_proc->curr_priority = MAX_PRIORITY;

    actif = curr_proc;
    cpt_proc++;
}

int32_t cree_processus(void (*code)(void), char *nom)
{
    Processus *zombie_proc = pop_head_zombie_proc();

    if (cpt_proc >= PROC_TABLE_SIZE && zombie_proc == NULL)
        return -1;

    Processus *curr_proc;
    if (zombie_proc == NULL)
    {
        tab_proc[cpt_proc] = malloc(sizeof(Processus));
        curr_proc = tab_proc[cpt_proc];
    }
    else
    {
        curr_proc = zombie_proc;
    }
    curr_proc->pid = cpt_proc;
    strcpy(curr_proc->nom, nom);
    curr_proc->etat = ETAT_ACTIVABLE;
    curr_proc->zone_sauv_reg[1] = (int32_t) & (curr_proc->pile_exec[PROC_PILE_EXEC_SIZE - 2]);
    curr_proc->pile_exec[PROC_PILE_EXEC_SIZE - 1] = (int32_t)fin_processus;
    curr_proc->pile_exec[PROC_PILE_EXEC_SIZE - 2] = (int32_t)code;
    curr_proc->wake_time = 0;
    curr_proc->base_priority = 1;
    curr_proc->curr_priority = 1;
    curr_proc->suiv = NULL;

    insert_queue_ac_proc(curr_proc);
    cpt_proc++;

    return curr_proc->pid;
}

int32_t cree_processus_priotity(void (*code)(void), char *nom, uint32_t priority)
{
    Processus *zombie_proc = pop_head_zombie_proc();

    if (cpt_proc >= PROC_TABLE_SIZE && zombie_proc == NULL)
        return -1;

    Processus *curr_proc;
    if (zombie_proc == NULL)
    {
        tab_proc[cpt_proc] = malloc(sizeof(Processus));
        curr_proc = tab_proc[cpt_proc];
    }
    else
    {
        curr_proc = zombie_proc;
    }
    curr_proc->pid = cpt_proc;
    strcpy(curr_proc->nom, nom);
    curr_proc->etat = ETAT_ACTIVABLE;
    curr_proc->zone_sauv_reg[1] = (int32_t) & (curr_proc->pile_exec[PROC_PILE_EXEC_SIZE - 2]);
    curr_proc->pile_exec[PROC_PILE_EXEC_SIZE - 1] = (int32_t)fin_processus;
    curr_proc->pile_exec[PROC_PILE_EXEC_SIZE - 2] = (int32_t)code;
    curr_proc->wake_time = 0;
    curr_proc->base_priority = priority;
    curr_proc->curr_priority = priority;
    curr_proc->suiv = NULL;

    insert_queue_ac_proc(curr_proc);
    cpt_proc++;

    return curr_proc->pid;
}

Processus *pop_head_LSC(ProcLSC *LSC)
{
    Processus *last_head = LSC->head;
    if (last_head == NULL)
        return NULL;
    LSC->head = last_head->suiv;
    if (LSC->head == NULL)
        LSC->queue = NULL;
    return last_head;
}

void insert_queue_LSC(Processus *proc, ProcLSC *LSC)
{
    proc->suiv = NULL;

    Processus *last_queue = LSC->queue;

    if (proc == last_queue)
        return;

    LSC->queue = proc;

    if (last_queue != NULL)
    {
        last_queue->suiv = proc;
    }
    else
    {
        LSC->head = proc;
    }
}

Processus *pop_head_ac_proc()
{
    activable_LSC.head->etat = ETAT_ELU;
    return pop_head_LSC(&activable_LSC);
}

void insert_queue_ac_proc(Processus *proc)
{
    proc->etat = ETAT_ACTIVABLE;
    insert_queue_LSC(proc, &activable_LSC);
}

void insert_priority_ac_proc(Processus *proc)
{
    proc->etat = ETAT_ACTIVABLE;

    Processus *tmp = activable_LSC.head;

    // No Element
    if (tmp == NULL)
    {
        proc->suiv = NULL;
        activable_LSC.head = proc;
        activable_LSC.queue = proc;
        return;
    }

    // First Element
    if (tmp->curr_priority >= proc->curr_priority)
    {
        proc->suiv = activable_LSC.head;
        activable_LSC.head = proc;
        return;
    }

    // Middle Element
    while (tmp->suiv != NULL)
    {
        if (tmp->suiv->curr_priority >= proc->curr_priority)
        {
            proc->suiv = tmp->suiv;
            tmp->suiv = proc;
            return;
        }
        tmp = tmp->suiv;
    }

    // Last Element
    proc->suiv = NULL;
    tmp->suiv = proc;
    activable_LSC.queue = proc;
}

Processus *pop_head_sleep_proc()
{
    endormi_LSC.head->etat = ETAT_ACTIVABLE;
    return pop_head_LSC(&endormi_LSC);
}

void insert_sleep_proc(Processus *proc)
{
    proc->etat = ETAT_ENDORMI;

    Processus *tmp = endormi_LSC.head;

    // No Element
    if (tmp == NULL)
    {
        proc->suiv = NULL;
        endormi_LSC.head = proc;
        endormi_LSC.queue = proc;
        return;
    }

    // First Element
    if (tmp->wake_time >= proc->wake_time)
    {
        proc->suiv = endormi_LSC.head;
        endormi_LSC.head = proc;
        return;
    }

    // Middle Element
    while (tmp->suiv != NULL)
    {
        if (tmp->suiv->wake_time >= proc->wake_time)
        {
            proc->suiv = tmp->suiv;
            tmp->suiv = proc;
            return;
        }
        tmp = tmp->suiv;
    }

    // Last Element
    proc->suiv = NULL;
    tmp->suiv = proc;
    endormi_LSC.queue = proc;
}

Processus *pop_head_zombie_proc()
{
    return pop_head_LSC(&zombie_LSC);
}

void insert_queue_zombie_proc(Processus *proc)
{
    proc->etat = ETAT_ZOMBIE;
    insert_queue_LSC(proc, &zombie_LSC);
}

void ordonnance(void)
{
    Processus *tmp_endormi = endormi_LSC.head;
    while (tmp_endormi != NULL && tmp_endormi->wake_time <= nbr_secondes())
    {
        insert_queue_ac_proc(pop_head_sleep_proc());
        tmp_endormi = tmp_endormi->suiv;
    }

    Processus *curr_proc = actif;
    curr_proc->curr_priority++;
    if (curr_proc->wake_time <= nbr_secondes())
    {
        insert_priority_ac_proc(curr_proc);
    }
    Processus *tmp_ac = activable_LSC.head;
    while (tmp_ac != NULL)
    {
        if (tmp_ac->curr_priority >= MAX_PRIORITY)
        {
            tmp_ac->curr_priority = tmp_ac->base_priority;
        }
        tmp_ac = tmp_ac->suiv;
    }
    Processus *next_proc = pop_head_ac_proc();
    actif = next_proc;
    ctx_sw(curr_proc->zone_sauv_reg, next_proc->zone_sauv_reg);
}

int32_t mon_pid(void)
{
    return actif->pid;
}

char *mon_nom(void)
{
    return actif->nom;
}

uint32_t mon_base_prio(void)
{
    return actif->base_priority;
}

uint32_t mon_prio(void)
{
    return actif->curr_priority;
}

void dors(uint32_t nb_secs)
{
    Processus *sleep_proc = actif;
    sleep_proc->wake_time = nbr_secondes() + nb_secs;
    insert_sleep_proc(sleep_proc);

    ordonnance();
}

void fin_processus(void)
{
    Processus *killed_proc = actif;
    killed_proc->wake_time = -1; // Pour ne pas le ré-inserer dans la liste des processus actifs
    insert_queue_zombie_proc(killed_proc);

    ordonnance();
}

void affiche_etats(void)
{
    printf("=================\n");
    for (uint32_t i = 0; i < PROC_TABLE_SIZE; i++)
    {
        Processus *curr_proc = tab_proc[i];
        if (curr_proc != NULL)
        {
            printf("Name=%s | Pid=%i | État=%d | basePrio=%d | Prio=%d\n",
                   curr_proc->nom, curr_proc->pid, curr_proc->etat,
                   curr_proc->base_priority, curr_proc->curr_priority);
        }
    }
    printf("=================\n");
}
