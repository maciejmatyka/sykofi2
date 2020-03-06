

#define C_DRAW   0x10000000              /* test cell inside system only */
#define C_BND    0x00000001 						/* boundary cells 							*/
#define C_IN     0x00000002             /* input wall 									*/
#define C_INCOR  0x00000004             /* IN adjacent to NOSLP					*/
#define C_OUT    0x00000008             /* out wall 										*/
#define C_FRSLP  0x00000010             /* free slip 										*/
#define C_NOSLP  0x00000020             /* no slip cell 								*/
#define C_EMPBND 0x00000040             /* off from calculation 				*/
#define C_EMP    0x00000080             /* empty cell (no fluid inside) */
#define C_FULL   0x00000100             /* full, no surface cell 				*/
#define C_SUR    0x00000200             /* full, surface cell 					*/
#define C_OB     0x00000400             /* adiacent to boundary cell 		*/
#define C_COR    0x00000800             /* corner cell 									*/
#define C_URON   0x00001000             /* upper right hand corner touch bnd cell */

#define S_N     0x08000000   /* surface cells          */
#define S_S     0x04000000   /* North, South, East, West */
#define S_W     0x02000000   
#define S_E     0x01000000
#define S_WE    (S_W|S_E)
#define S_NS    (S_N|S_S)
#define S_SW    (S_S|S_W)
#define S_NW    (S_N|S_W)
#define S_NE    (S_N|S_E)
#define S_SE    (S_S|S_E)
#define S_SWE   (S_SW|S_E)
#define S_NSW   (S_NS|S_W)
#define S_NWE   (S_NW|S_E)
#define S_NSE   (S_NS|S_E)
#define S_NSWE  (S_NS|S_WE)

