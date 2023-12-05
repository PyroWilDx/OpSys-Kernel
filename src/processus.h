#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <inttypes.h>

#define PROC_MAX_NAME_LENGTH 32
#define PROC_NB_REG_IMPORTANT 5
#define PROC_PILE_EXEC_SIZE 512

#define PROC_TABLE_SIZE 256

#define MAX_PRIORITY 16

typedef enum _EtatProcessus
{
    ETAT_ELU,
    ETAT_ACTIVABLE,
    ETAT_ENDORMI,
    ETAT_ZOMBIE
} EtatProcessus;

typedef struct _Processus
{
    int32_t pid;
    char nom[PROC_MAX_NAME_LENGTH];
    EtatProcessus etat;
    int32_t zone_sauv_reg[PROC_NB_REG_IMPORTANT];
    int32_t pile_exec[PROC_PILE_EXEC_SIZE];
    uint32_t wake_time;
    uint32_t base_priority;
    uint32_t curr_priority;
    struct _Processus *suiv;
} Processus;

typedef struct _ProcLSC
{
    Processus *head;
    Processus *queue;
} ProcLSC;

extern Processus *tab_proc[PROC_TABLE_SIZE];
extern uint32_t cpt_proc;
extern Processus *actif;
extern ProcLSC activable_LSC;
extern ProcLSC endormi_LSC;
extern ProcLSC zombie_LSC;

extern void ctx_sw(int32_t *zone0, int32_t *zone1);

void init_processus(void);

void init_idle(void);

int32_t cree_processus(void (*code)(void), char *nom);

int32_t cree_processus_priotity(void (*code)(void), char *nom, uint32_t priority);

Processus *pop_head_LSC(ProcLSC *LSC);

void insert_queue_LSC(Processus *proc, ProcLSC *LSC);

Processus *pop_head_ac_proc();

void insert_queue_ac_proc(Processus *proc);

void insert_priority_ac_proc(Processus *proc);

Processus *pop_head_sleep_proc();

void insert_sleep_proc(Processus *proc);

Processus *pop_head_zombie_proc();

void insert_queue_zombie_proc(Processus *proc);

void ordonnance(void);

int32_t mon_pid(void);

char *mon_nom(void);

uint32_t mon_base_prio(void);

uint32_t mon_prio(void);

void dors(uint32_t nb_secs);

void fin_processus(void);

void affiche_etats(void);

#endif
