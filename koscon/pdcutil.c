/* PDCurses */

#include "pdckos.h"

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
    _ksys_speaker_play(0xFFFFFFFF);
}

void PDC_napms(int ms)
{
    _ksys_delay(ms/10);
}

const char *PDC_sysname(void)
{
    return "KolibriOS"; 
}