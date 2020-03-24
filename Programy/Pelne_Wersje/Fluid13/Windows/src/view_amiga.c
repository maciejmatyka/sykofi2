
#include <stdio.h>
#include <stdlib.h>
#include <graphics/gfx.h>
#include <intuition/intuition.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/exec.h>

#include "view.h"

struct IntuitionBase *IntuitionBase;
struct GraphicsBase *GraphicsBase;
struct Window *window1;
struct Screen *screen1;

struct TagItem screentags[] = {
    SA_Left, 0,
    SA_Top, 0,
    SA_Width, 640,
    SA_Height, 512,
    SA_Depth, 3,

    SA_DisplayID, 0,
/*	SA_DisplayID, HIRES|LACE,*/

    SA_AutoScroll, TRUE,
    SA_Type, CUSTOMSCREEN,
    SA_ShowTitle, FALSE,
    SA_Title, (ULONG) "Output View",
/*	SA_Draggable, TRUE,*/
/*	SA_Quiet, TRUE,*/
/*	SA_MinimizeISG, TRUE,*/

    TAG_DONE
};

struct TagItem windowtags[] = {
    WA_Left, 20,
    WA_Top, 20,
    WA_Width, 0,
    WA_Height, 0,
    WA_Flags,
    WFLG_RMBTRAP | WFLG_ACTIVATE | WFLG_GIMMEZEROZERO | WFLG_BORDERLESS,
    WA_ScreenTitle, (ULONG) "Output View",

    WA_CustomScreen, 0,

    TAG_DONE
};

void V_ClearRect(int x0, int y0, int x1, int y1)
{
    EraseRect(&screen1->RastPort, (LONG) x0, (LONG) y0, (LONG) x1,
	      (LONG) y1);
}

void V_Line(int x0, int y0, int x1, int y1)
{
    SetAPen(&screen1->RastPort, 1);	/* black color */

    Move(&screen1->RastPort, (LONG) x0, (LONG) y0);
    Draw(&screen1->RastPort, (LONG) x1, (LONG) y1);
}

void V_Point(int x, int y)
{
    SetAPen(&screen1->RastPort, 2);	/* grey color */
    WritePixel(&screen1->RastPort, (LONG) x, (LONG) y);
}

int V_CheckExitTrue(void)
{
    int a;
    a = window1->MouseX;
    return ((a <= 1));
}


void V_OpenView(int sizex, int sizey)
{
    sizex = 320;
    sizey = 256;


    if (!
	(IntuitionBase =
	 (struct IntuitionBase *) OpenLibrary("intuition.library", 37))) {
	printf("Cannot open intuition.library.\n");
	exit(1);
    }

    if (!
	(GraphicsBase =
	 (struct GraphicsBase *) OpenLibrary("graphics.library", 37))) {
	printf("Cannot open graphics.library.\n");
	exit(1);
    }

    if (sizey == 200)
	sizey = 256;

    screentags[2].ti_Data = sizex;	/* x output size */
    screentags[3].ti_Data = sizey;	/* y output size */

    if (sizex > 320 || sizey > 256)
	screentags[5].ti_Data = HIRES | LACE;	/* set to HIRES|LACE screen */


    if (!(screen1 = OpenScreenTagList(0, screentags))) {
	printf("Cannot open screen.\n");
	exit(1);
    }


    windowtags[2].ti_Data = sizex;	/* x output size */
    windowtags[3].ti_Data = sizey;	/* y output size */
    windowtags[6].ti_Data = (ULONG) screen1;


    if (!(window1 = OpenWindowTagList(0, windowtags))) {
	printf("Cannot open Output View.\n");
	exit(1);
    }


    SetRGB32(&screen1->ViewPort, 0, 180 << 24, 180 << 24, 180 << 24);	/* background */
    SetRGB32(&screen1->ViewPort, 1, 170 << 24, 170 << 24, 170 << 24);	/* mesh */
    SetRGB32(&screen1->ViewPort, 2, 120 << 24, 120 << 24, 130 << 24);	/* particles */


}

void V_CloseView(void)
{
    CloseWindow(window1);
    CloseScreen(screen1);
    CloseLibrary((struct Library *) IntuitionBase);
    CloseLibrary((struct Library *) GraphicsBase);
}
