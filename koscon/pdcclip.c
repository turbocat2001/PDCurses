/* PDCurses */

#include "curses.h"
#include "pdckos.h"
#include "stddef.h"

#include <stdlib.h>
#include <string.h>

/*man-start**************************************************************

clipboard
---------

### Synopsis

    int PDC_getclipboard(char **contents, long *length);
    int PDC_setclipboard(const char *contents, long length);
    int PDC_freeclipboard(char *contents);
    int PDC_clearclipboard(void);

### Description

   PDC_getclipboard() gets the textual contents of the system's
   clipboard. This function returns the contents of the clipboard in the
   contents argument. It is the responsibility of the caller to free the
   memory returned, via PDC_freeclipboard(). The length of the clipboard
   contents is returned in the length argument.

   PDC_setclipboard copies the supplied text into the system's
   clipboard, emptying the clipboard prior to the copy.

   PDC_clearclipboard() clears the internal clipboard.

### Return Values

    indicator of success/failure of call.
    PDC_CLIP_SUCCESS        the call was successful
    PDC_CLIP_MEMORY_ERROR   unable to allocate sufficient memory for
                            the clipboard contents
    PDC_CLIP_EMPTY          the clipboard contains no text
    PDC_CLIP_ACCESS_ERROR   no clipboard support

### Portability
                             X/Open  ncurses  NetBSD
    PDC_getclipboard            -       -       -
    PDC_setclipboard            -       -       -
    PDC_freeclipboard           -       -       -
    PDC_clearclipboard          -       -       -

**man-end****************************************************************/

#define _DATA(type, addr, offset) *((type*)((uint8_t*)addr+offset))
#define _TEXT_BASE 12

int PDC_getclipboard(char **contents, long *length)
{
    int clip_num = 0;
    long len = 0;
    char *full_content = NULL;

    PDC_LOG(("PDC_getclipboard() - called\n"));

    clip_num = _ksys_clip_num(); 

    if(clip_num < 1){
        return PDC_CLIP_ACCESS_ERROR;
    }

    full_content =_ksys_clip_get(clip_num-1);
    if(full_content==(char*)1 || full_content==(char*)-1){
        return PDC_CLIP_ACCESS_ERROR;
    }

    if(len<=0){
        free(full_content);
        return PDC_CLIP_EMPTY;
    }

    if(_DATA(int, full_content, 0) <= 0 || 
       _DATA(int, full_content, 4) != KSYS_CLIP_TEXT || 
       _DATA(int, full_content, 8) != KSYS_CLIP_CP866 
    ){
       free(full_content);
       return PDC_CLIP_EMPTY;
    }

    *contents = full_content + _TEXT_BASE;
    *length   = len;

    return PDC_CLIP_SUCCESS;
}

int PDC_setclipboard(const char *contents, long length)
{
    char *tmp_buff = NULL;
    long full_len  = 0;

    PDC_LOG(("PDC_setclipboard() - called\n"));
    if(_ksys_clip_pop()){
        return PDC_CLIP_ACCESS_ERROR;
    }

    full_len = length + _TEXT_BASE + 1;

    tmp_buff = calloc(full_len, sizeof(char));
    if(!tmp_buff){
        return PDC_CLIP_MEMORY_ERROR;
    }

    _DATA(int, tmp_buff, 0) = full_len;
    _DATA(int, tmp_buff, 4) = KSYS_CLIP_TEXT;
    _DATA(int, tmp_buff, 8) = KSYS_CLIP_CP866;

    strncpy(tmp_buff, contents, length);

    if(_ksys_clip_set(full_len, tmp_buff)){
        free(tmp_buff);
        return PDC_CLIP_ACCESS_ERROR;
    }
    free(tmp_buff); 
    return PDC_CLIP_SUCCESS;
}

int PDC_freeclipboard(char *contents)
{
    PDC_LOG(("PDC_freeclipboard() - called\n"));
    free(contents);
    return PDC_CLIP_SUCCESS;
}

int PDC_clearclipboard(void)
{
    PDC_LOG(("PDC_clearclipboard() - called\n"));
    /* STUB */
    return PDC_CLIP_SUCCESS;
}