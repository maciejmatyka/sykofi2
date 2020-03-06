

#include "fluid.h"




void AllocTables(void)
{

    if
	(
	 !(Flag = Alloc2TableI(imax, jmax)) |
	 !(Un1 = Alloc2TableD(imax, jmax)) |
!(Vn1 = Alloc2TableD(imax, jmax)) |
	 !(U = Alloc2TableD(imax, jmax)) |
	 !(V = Alloc2TableD(imax, jmax)) |
	 !(P = Alloc2TableD(imax, jmax)) |
	 !(D = Alloc2TableD(imax, jmax)) |
	 !(R = Alloc2TableD(imax, jmax)) |
	 !(_Flag = Alloc2TableI(imax, jmax)) |
	 !(_Un1 = Alloc2TableD(imax, jmax)) |
	 !(_Vn1 = Alloc2TableD(imax, jmax)) |
	 !(_U = Alloc2TableD(imax, jmax)) |
	 !(_V = Alloc2TableD(imax, jmax)) |
	 !(_P = Alloc2TableD(imax, jmax)) |
	 !(_D = Alloc2TableD(imax, jmax)) | 
	 !(_R = Alloc2TableD(imax, jmax)) |

	 !(TS_ANY = Alloc2TableI(imax, jmax)) |
	 !(TS_UKNK = Alloc2TableD(imax, jmax)) |
	 !(TS_VKNK = Alloc2TableD(imax, jmax)) 
				 
	 ) 
	
	{	/* then ... */
		printf("No memory to alloc tables.\n");
		exit(1);
    }

}