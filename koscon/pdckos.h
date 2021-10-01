/* PDCurses */

#include <curspriv.h>
#include <string.h>

/*----------------------------------------------------------------------
 *  MEMORY MODEL SUPPORT:
 *
 *  MODELS
 *    TINY    cs,ds,ss all in 1 segment (not enough memory!)
 *    SMALL   cs:1 segment, ds:1 segment
 *    MEDIUM  cs:many segments, ds:1 segment
 *    COMPACT cs:1 segment, ds:many segments
 *    LARGE   cs:many segments, ds:many segments
 *    HUGE    cs:many segments, ds:segments > 64K
 */

#ifdef __TINY__
# define SMALL 1
#endif
#ifdef __SMALL__
# define SMALL 1
#endif
#ifdef __MEDIUM__
# define MEDIUM 1
#endif
#ifdef __COMPACT__
# define COMPACT 1
#endif
#ifdef __LARGE__
# define LARGE 1
#endif
#ifdef __HUGE__
# define HUGE 1
#endif

#include <sys/ksys.h>

extern short pdc_curstoreal[16];
extern int pdc_adapter;
extern int pdc_scrnmode;
extern int pdc_font;
extern bool pdc_direct_video;
extern bool pdc_bogus_adapter;
extern unsigned pdc_video_seg;
extern unsigned pdc_video_ofs;

