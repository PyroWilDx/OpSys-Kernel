#ifndef __TEMPS_H__
#define __TEMPS_H__

#include <inttypes.h>

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

extern uint8_t cpt_tic;

extern uint8_t seconde;
extern uint8_t minute;
extern uint8_t heure;
extern uint32_t total_secs;

extern void traitant_IT_32();

uint8_t small_strlen(const char *);

void ecrit_temps(const char *str, uint8_t len);

void tic_PIT();

void init_traitant_IT(uint32_t num_IT, void (*traitant)());

void masque_IRQ(uint32_t num_IRQ, _Bool masque);

uint32_t nbr_secondes();

#endif
