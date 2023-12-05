#include <cpu.h>
#include <debug.h>
#include <string.h>
#include "temps.h"
#include "ecran.h"
#include "segment.h"
#include "processus.h"

#define ADDR_0_TABLE 0x1000
#define PORT_CMD_HORLOGE 0x43
#define PORT_DATA_HORLOGE 0x40
#define PORT_DATA_IRQ 0x21

uint8_t cpt_tic = 0;

uint8_t seconde = 0;
uint8_t minute = 0;
uint8_t heure = 0;

uint32_t total_secs = 0;

uint8_t small_strlen(const char *str)
{
    uint8_t len = 0;
    char *tmp_str = (char *)str;
    while (*tmp_str != '\0')
    {
        len++;
        tmp_str++;
    }
    return len;
}

void ecrit_temps(const char *str, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        ecrit_car(MAX_LIG, MAX_COL - len + i + 1, str[i],
                  0xF, 0x0, 0);
    }
}

void tic_PIT()
{
    outb(0x20, 0x20);

    cpt_tic++;

    if (cpt_tic == CLOCKFREQ)
    {
        total_secs++;
        seconde++;

        if (seconde % 1 == 0)
            affiche_etats();

        if (seconde == 60)
        {
            minute++;
            seconde = 0;
            if (minute == 60)
            {
                heure++;
                minute = 0;
            }
        }
        char str_time[10];
        sprintf(str_time, "%02u:%02u:%02u", heure, minute, seconde);
        ecrit_temps(str_time, small_strlen(str_time));
        cpt_tic = 0;
    }

    ordonnance();
}

void init_traitant_IT(uint32_t num_IT, void (*traitant)())
{
    uint32_t *ptr_table = (uint32_t *)ADDR_0_TABLE;
    ptr_table += (num_IT * 2);

    uint32_t val_addr = (uint32_t)traitant;
    uint32_t val1 = (KERNEL_CS << 16) | (val_addr & 0x0000FFFF);
    uint32_t val2 = (val_addr & 0xFFFF0000) | 0x8E00;

    *ptr_table = val1;
    ptr_table++;
    *ptr_table = val2;

    uint16_t freq = QUARTZ / CLOCKFREQ;
    outb(0x34, PORT_CMD_HORLOGE);
    outb(freq & 0xFF, PORT_DATA_HORLOGE);
    outb(freq >> 8, PORT_DATA_HORLOGE);

    const char str_time[] = "00:00:00";
    ecrit_temps(str_time, small_strlen(str_time));
}

void masque_IRQ(uint32_t num_IRQ, _Bool masque)
{
    uint8_t mask_val = inb(PORT_DATA_IRQ);
    mask_val = mask_val & 0b11111110;
    uint8_t modif = 0b1 << num_IRQ;
    if (!masque)
    {
        modif = ~modif;
    }
    if (!masque)
        mask_val = mask_val & modif;
    else
        mask_val = mask_val | modif;
    outb(mask_val, PORT_DATA_IRQ);
}

uint32_t nbr_secondes()
{
    return total_secs;
}
