#include <cpu.h>
#include <string.h>
#include "ecran.h"

#define ADDR_0_ECRAN 0xB8000
#define PORT_CMD_CURSEUR 0x3D4
#define PORT_DATA_CURSEUR 0x3D5

uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    return (uint16_t *)(ADDR_0_ECRAN + 2 * ((MAX_COL + 1) * lig + col));
}

void ecrit_car(uint32_t lig, uint32_t col, char c,
               uint8_t couleur, uint8_t couleur_fond,
               _Bool clignote)
{
    uint16_t *ptr_coord_mem = ptr_mem(lig, col);
    couleur = couleur & 0b1111; // on prend les 4 premiers bits (faible) pour la couleur du caractère
    couleur_fond = couleur_fond & 0b111; // on prend les 3 premiers bits (faible) pour la couleur du fond
    *ptr_coord_mem = (clignote << 15) + (couleur_fond << 12) +
                     (couleur << 8) + c;
}

uint16_t get_empty_car()
{
    return (0b0 << 15) + (0b000 << 12) + (0b1111 << 8) + ' ';
}

uint16_t *get_max_addr()
{
    return ptr_mem(MAX_LIG, MAX_COL);
}

void rempli_ecran_from_to(uint16_t *start_addr, uint16_t *end_addr, uint16_t val)
{
    for (uint16_t *ptr_coord_mem = start_addr; ptr_coord_mem < end_addr; ptr_coord_mem++)
    {
        *ptr_coord_mem = val;
    }
}

void efface_ecran(void)
{
    rempli_ecran_from_to(ptr_mem(0, 0), get_max_addr(), get_empty_car());
}

uint32_t curseur_lig = 0;
uint32_t curseur_col = 0;

void place_curseur(uint32_t lig, uint32_t col)
{
    uint16_t curseur_pos = (MAX_COL + 1) * lig + col;

    outb(0x0F, PORT_CMD_CURSEUR);
    outb(curseur_pos & 0b11111111, PORT_DATA_CURSEUR);
    outb(0x0E, PORT_CMD_CURSEUR);
    outb(curseur_pos >> 8, PORT_DATA_CURSEUR);

    curseur_lig = lig;
    curseur_col = col;
}

void traite_car(char c)
{
    switch (c)
    {
    case '\b':
        place_curseur(curseur_lig, (curseur_col > 0) ? curseur_col - 1 : 0);
        break;

    case '\t':
        uint8_t taille_tabulation = 8;
        place_curseur(curseur_lig,
                      (curseur_col < MAX_COL - taille_tabulation) ? curseur_col + taille_tabulation : MAX_COL);
        break;

    case '\n':
        place_curseur(curseur_lig + 1, 0);
        break;

    case '\f':
        reset_ecran();
        break;

    case '\r':
        place_curseur(curseur_lig, 0);
        break;

    default:
        ecrit_car(curseur_lig, curseur_col, c,
                  0xF, 0x0, 0);
        _Bool fin_lig = (curseur_col == MAX_COL);
        place_curseur((fin_lig) ? curseur_lig + 1 : curseur_lig,
                      (fin_lig) ? 0 : curseur_col + 1);
        break;
    }
}

void defilement(void)
{
    memmove(ptr_mem(0, 0), ptr_mem(1, 0),
            2 * (MAX_LIG * (MAX_COL + 1)) - (10 * 2));
    rempli_ecran_from_to(ptr_mem(MAX_LIG, 0), get_max_addr() - 10, get_empty_car());
    place_curseur(curseur_lig - 1, curseur_col);
}

void console_putbytes(const char *str, int len)
{
    for (int i = 0; i < len; i++)
    {
        traite_car(str[i]);
        if (curseur_lig > MAX_LIG)
        {
            defilement();
        }
    }
}

void reset_ecran()
{
    efface_ecran();
    place_curseur(0, 0);
}
