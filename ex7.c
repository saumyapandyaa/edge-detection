/**
 * Exercise 7 - Convolutions and Edge detection!
 *
 * Please read the comments below carefully, they describe your task in detail.
 * Any clarifications and corrections will be posted to Piazza so please keep
 * an eye on the forum!
 *
 * Starter code:  Mustafa Quraish, 2020
 */

#include "imgUtils.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Image dimensions
#define SIZEX 512
#define SIZEY 512

// Kernel size
#define K_SIZE 3

// Declaration of the X and Y kernel matrices.
double GX[K_SIZE][K_SIZE] = {
    {1, 0, -1},
    {2, 0, -2},
    {1, 0, -1},
};

double GY[K_SIZE][K_SIZE] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1},
};

/*****************************************************************************/

double convolve(unsigned char inp[SIZEY][SIZEX], int px, int py,
                double kernel[K_SIZE][K_SIZE])
{
  /**
   * Perform the convolution of the given kernel and the image with (px, py) as
   * the center pixel. In essense, the center of the kernel is placed at the
   * given pixel, and you compute weighted sum (refer to tha handout for an
   * example) and return it.
   *
   * If (px, py) is one of the pixels at the edge of the screen (and thus part
   * of the kernel is leaking outside the image), you may assume that those
   * pixels are black. In other words, you should pretend that all pixels
   * outside the image are just black for the sake of computation.
   *
   
   * Note: In this case, the weighted sum is also an integer because of GX and
   *       GY. However, in general the kernels can have floating point values,
   *       so doing it this way will let you generalize your solution much more
   *       easily if you wish to do so.
   */
   int half=K_SIZE/2;
   double sum=0;   
  // printf("px = %d, py= %d\n\n",px,py);
   for(int y=(py-half), kernel_y=0; y<(py+half+1);y++){// && kernel_y<SIZEY
	   for(int x=(px-half),kernel_x=0;x<(px+half+1);x++){// && kernel_x<SIZEX
		  // printf("x: %d , y= %d\n", x,y);
		  // printf("sum=%f\n",sum);
		  // printf("kernel[%d][%d]*inp[%d][%d] =  %f*%d  =  %f\n",kernel_y,kernel_x,y,x,kernel[kernel_y][kernel_x],inp[y][x],kernel[kernel_y][kernel_x]*inp[y][x]);
			if(x>=0 && x<SIZEX && y>=0 && y<SIZEY){
				sum+=(kernel[kernel_y][kernel_x]*inp[y][x]);
			}
		   kernel_x++;
	   }
	   kernel_y++;
	  // printf("\n--------\n\n");
   }
    return sum; // Return with the correct value
}

void sobel(unsigned char inp[SIZEY][SIZEX], unsigned char out[SIZEY][SIZEX])
{
  /**
   * Given an input image, perform the Sobel edge detection algorithm and store
   * the output in `out`.  Remember that this boils down to:
   *
   * for pixel in input:
   *    g_x = convolve GX at pixel
   *    g_y = convolve GY at pixel
   *    set corresponding output pixel to sqrt(g_x^2 + g_y^2)
   *
   * Note: The expression sqrt(g_x^2 + g_y^2) may result in values greater than
   *       255. If the value is greater, just set it equal to 255. Round down
   *       in case of floating point values.
   */
	for(int y=0;y<SIZEY;y++){
		for(int x=0;x<SIZEX;x++){
			double g_x=convolve(inp,x,y,GX);
			double g_y=convolve(inp,x,y,GY);
			//printf("g_x = %f,  g_y = %f\n", g_x, g_y);
			if((sqrt((g_x*g_x) + (g_y*g_y)))>255){
				out[y][x]=255;
			}
			else{
				out[y][x]=floor(sqrt((g_x*g_x) + (g_y*g_y)));
			}
			//printf("out[%d][%d]=%d\n", y,x,out[y][x]);
		}
	}
    return; // Update `out` before returning
}

/*****************************************************************************/

#ifndef __testing__ // You know the drill, don't remove this

int main()
{
  /* Feel free to change the contents INSIDE the main function to help test */

  // Initialize the 2D-arrays that will store the values of the pixels
  unsigned char data[SIZEX][SIZEY];
  unsigned char edge[SIZEX][SIZEY];

  // We will store the values of the input image
  readPGM("input.pgm", &data[0][0]);

  // Test convolve()
  double res = convolve(data, 0, 0, GY);
  if (res != -132.0)
  {
    printf("Error: convolve() test 1 output incorrect.\n");
    printf("Expected: %f,  Yours: %f\n", -132.0, res);
    exit(1);
  }

  // Pixel somewhere in the middle
  if (convolve(data, 324, 218, GX) != -21.0)
  {
    printf("Error: convolve() test 2 output incorrect.\n");
    printf("Expected: %f,  Yours: %f\n", -21.0, res);
    exit(1);
  }
  printf("- convolve() output was correct.\n");
/*  double x1=convolve(data, 0,96,GX);
 double x2=convolve(data, 0,96,GY);
 printf("x1= %f ,x2= %f \n", x1, x2); */
  // Call the sobel function
  sobel(data, edge);


  // Write the values to the output image
  writePGM("output.pgm", &edge[0][0]);

  // Compare to the expected image
  readPGM("expected.pgm", &data[0][0]);

  for (int i = 0; i < SIZEX; i++)
  {
    for (int j = 0; j < SIZEY; j++)
    {
      if (data[j][i] != edge[j][i])
      {
        printf("Error: Pixel (x=%d, y=%d) has the wrong colour.\n", i, j);
        printf("Expected: %u,  Yours: %u\n", data[j][i], edge[j][i]);
        exit(1);
      }
    }
  }

  printf("- sobel() output was correct.\n");
  return 0;
}

#endif
