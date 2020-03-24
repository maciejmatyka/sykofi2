#include <cstdlib>
#include <cstdio>

/*
 * Take screenshot with glReadPixels and save to a file in PPM format.
 * -   filename: file path to save to, without extension
 * -   width: screen width in pixels
 * -   height: screen height in pixels
 * -   pixels: intermediate buffer to avoid repeated mallocs across multiple calls.
 *     Contents of this buffer do not matter. May be NULL, in which case it is initialized.
 *     You must free it when you won't be calling this function anymore.
 * */
static void screenshot_ppm(const char *filename, unsigned int width,
						           unsigned int height, GLubyte **pixels) 
{
   
       size_t i, j, k, cur;
       const size_t format_nchannels = 3;
       FILE *f = fopen(filename, "w");
       fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
       *pixels = (GLubyte*) realloc(*pixels, format_nchannels * sizeof(GLubyte) * width * height);
       glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, *pixels);
       for (i = 0; i < height; i++) 
	 {
		
		        for (j = 0; j < width; j++) 
		  {
			 
			             cur = format_nchannels * ((height - i - 1) * width + j);
			             fprintf(f, "%3d %3d %3d ", (*pixels)[cur], (*pixels)[cur + 1], (*pixels)[cur + 2]);
		  }
		
		        fprintf(f, "\n");
	 }
   
       fclose(f);
}
