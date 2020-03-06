
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "jpeg.h"
#include "ijl.h" // intel jpeg library header file.


/*
Intel Corporation has released a number of extremely useful libraries and tools as part
of their performance suite.  These tools are designed to assist developers in taking
advantage of their advanced processors.

One excellent tool is the Intel Jpeg Library which provides a single small DLL which
will rapidly compress and decompress Jpeg images, a common graphics file format.

The following code snippet provides a static wrapper class for this library.  The 
only thing you need besides this code snippet is IJL.H, IJL.LIB, IJL.DLL which can
be found at the following URL.


http://www-cs.intel.com/support/performancetools/libraries/ijl/index.htm

When you see the quantity of code required just to compress or decompress a JPEG image
using IJL I think you will see the value of the following simplified wrapper layer.

John W. Ratcliff
jratcliff@verant.com

*/

//** JPEG.CPP, the code used to wrap IJL

//############################################################################
//##                                                                        ##
//##  JPEG.CPP                                                                 ##
//##                                                                        ##
//##  Wrapper class to load a jpeg from a block of memory.                  ##
//##                                                                        ##
//##  OpenSourced 2/4/2000 by John W. Ratcliff                                ##
//##                                                                        ##
//##  No warranty expressed or implied.  Released as part of the triangle   ##
//##  throughput testbed project.                                           ##
//############################################################################
//##                                                                        ##
//##  Contact John W. Ratcliff at jratcliff@verant.com                      ##
//############################################################################
// read image into this buffer.
void * Jpeg::ReadImage(int &width,
                       int &height,
                       int &nchannels,
                       const void *buffer,
                       int sizebytes)
{
  JPEG_CORE_PROPERTIES jcprops;

  if ( ijlInit(&jcprops) != IJL_OK )
  {
    ijlFree(&jcprops);
    return 0;
  }

  jcprops.JPGBytes = (unsigned char *) buffer;
  jcprops.JPGSizeBytes = sizebytes;
  jcprops.jquality = 100;

  if ( ijlRead(&jcprops,IJL_JBUFF_READPARAMS) != IJL_OK )
  {
    ijlFree(&jcprops);
    return 0;
  }

  width  = jcprops.JPGWidth;
  height = jcprops.JPGHeight;
  IJLIOTYPE mode;

  mode = IJL_JBUFF_READWHOLEIMAGE;
  nchannels = jcprops.JPGChannels;
  unsigned char * pixbuff = new unsigned char[width*height*nchannels];
  if ( !pixbuff )
  {
    ijlFree(&jcprops);
    return 0;
  }

  jcprops.DIBWidth  = width;
  jcprops.DIBHeight = height;
  jcprops.DIBChannels = nchannels;
  jcprops.DIBPadBytes = 0;
  jcprops.DIBBytes = (unsigned char *)pixbuff;

  if ( jcprops.JPGChannels == 3 )
  {
    jcprops.DIBColor = IJL_RGB;
    jcprops.JPGColor = IJL_YCBCR;
    jcprops.JPGSubsampling = IJL_411;
    jcprops.DIBSubsampling = (IJL_DIBSUBSAMPLING) 0;
  }
  else
  {
    jcprops.DIBColor = IJL_G;
    jcprops.JPGColor = IJL_G;
    jcprops.JPGSubsampling = (IJL_JPGSUBSAMPLING) 0;
    jcprops.DIBSubsampling = (IJL_DIBSUBSAMPLING) 0;
  }

  if ( ijlRead(&jcprops, mode) != IJL_OK )
  {
    ijlFree(&jcprops);
    return 0;
  }

  if ( ijlFree(&jcprops) != IJL_OK ) return 0;

  return (void *)pixbuff;
}


void * Jpeg::Compress(const void *source,
                      int width,
                      int height,
                      int bpp,
                      int &len,
                      int quality)
{
  JPEG_CORE_PROPERTIES jcprops;

  if ( ijlInit(&jcprops) != IJL_OK )
  {
    ijlFree(&jcprops);
    return false;
  }

  jcprops.DIBWidth    = width;
  jcprops.DIBHeight   = height;
  jcprops.JPGWidth    = width;
  jcprops.JPGHeight   = height;
  jcprops.DIBBytes    = (unsigned char *) source;
  jcprops.DIBPadBytes = 0;
  jcprops.DIBChannels = bpp;
  jcprops.JPGChannels = bpp;

  if ( bpp == 3 )
  {
    jcprops.DIBColor = IJL_RGB;
    jcprops.JPGColor = IJL_YCBCR;
    jcprops.JPGSubsampling = IJL_411;
    jcprops.DIBSubsampling = (IJL_DIBSUBSAMPLING) 0;
  }
  else
  {
    jcprops.DIBColor = IJL_G;
    jcprops.JPGColor = IJL_G;
    jcprops.JPGSubsampling = (IJL_JPGSUBSAMPLING) 0;
    jcprops.DIBSubsampling = (IJL_DIBSUBSAMPLING) 0;
  }

  int size = width*height*bpp;

  unsigned char * buffer = new unsigned char[size];

  jcprops.JPGSizeBytes = size;
  jcprops.JPGBytes     = buffer;

  jcprops.jquality = quality;


  if ( ijlWrite(&jcprops,IJL_JBUFF_WRITEWHOLEIMAGE) != IJL_OK )
  {
    ijlFree(&jcprops);
    delete buffer;
    return 0;
  }


  if ( ijlFree(&jcprops) != IJL_OK )
  {
    delete buffer;
    return 0;
  }

  len = jcprops.JPGSizeBytes;
  return buffer;
}
