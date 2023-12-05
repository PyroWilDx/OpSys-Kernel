#ifndef __ECRAN_H__
#define __ECRAN_H__

#include <inttypes.h>

#define MAX_LIG 24
#define MAX_COL 79

/*
Renvoie un pointeur sur la case mémoire correspondant aux coordonnées fournies.
*/
uint16_t *ptr_mem(uint32_t lig, uint32_t col);

/*
Écrit le caractère aux coordonnées spécifiées.
*/
void ecrit_car(uint32_t lig, uint32_t col, char c,
               uint8_t couleur_c, uint8_t couleur_fond,
               _Bool clignote);

uint16_t get_empty_car();

uint16_t *get_max_addr();

void rempli_ecran_from_to(uint16_t *start_addr, uint16_t *end_addr, uint16_t val);

void efface_ecran(void);

/*
Place le curseur à la position donnée.
*/
void place_curseur(uint32_t lig, uint32_t col);

/*
Traite un caractère donné.
*/
void traite_car(char c);

/*
Fait remonter d’une ligne l’affichage de l'écran.
*/
void defilement(void);

void console_putbytes(const char *str, int len);

void reset_ecran();

#endif
