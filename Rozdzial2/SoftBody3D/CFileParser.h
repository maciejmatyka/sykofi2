

#ifndef __CFILEPARSER_H_
#define __CFILEPARSER_H_

#include <stdio.h>
#include "iFileTypes.h"

/**
 * @short Parser Class. Not "State of the art" code, but it works and loads .asc files
 * into SoftBody structure.
 *
 *
 * @author Maciej Matyka
 * http://panoramix.ift.uni.wroc.pl/~maq
 *
 */


class CFileParser
{
public:

	CFileParser(char *_cFilename, int _iType) : cFilename(_cFilename), iType(_iType)
	{}

	CFileParser()
	{}
	
	
	~CFileParser(){}

	/**
	 * read points positions from .asc file                                                                     
	 */

	void parsePoints(void);


	/**
	 * read faces from .asc file  
	 */

	void parseFaces(void);

	/**
	 * specific I/O function                                                                     
	 */

	int parseIntValue(FILE *fp, char *);

	/**
	 * specific I/O function                                                                     
	 */

	void testEOF(FILE *fp);

	/**
	 * specific I/O function                                                                     
	 */

	void mfscanf(FILE *fp,char workbuf2[],char workbuf[]);
	
	/**
	 * specific I/O function                                                                     
	 */

	void seekWord(FILE *fp, char *word);

	/**
	 * dump the object into .asc file                                                                     
	 */

	void Dump3DSASC(char *);


private:
	char	*cFilename;
	int		iType;			// file type, like F_LWO
	FILE	*fp;			// file pointer

public:
	int		iNump;			// points number
	int		iNumf;			// faces number
	float	*fPoints;		// points
	int		*fFaces;	// faces


};


#endif