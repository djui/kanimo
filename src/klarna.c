/*
  Design decisions:
   * No float: Extra initialization space
   * No math: Extra include header file space
 */
#include <stdio.h>
#include <math.h>
#include <sysexits.h>

static const int FPS =  50; // Frames per second
static const int F   =   1; // Frame amount
static const int A   =   3; // Anti-Aliasing value
static const int W   =  80; // Output width
static const int H   =  40; // Output height
static const int B   =   1; // Circle border size
static const int K[] = {' ', '.', '.', ')', '.', '\\', '/', '@'}; // Pixel "brightness"
static const int C[] = {   // Circles' data, leaves in background to foreground order
// x, y, r
   0, 14, 48, // < yellow
  74,  8, 41, // > yellow
   0,  0, 52, // < green
  77, 19, 40, // > green
  11, 35, 33, // < blue
  46,  4, 18, // > blue
   0, 25, 49, // < cyan
  65,  8, 27, // > cyan
  20,  3, 41, // < red
  94, 24, 49, // > red
  11, 45, 37, // < purple
  46,  5, 20, // > purple
  43,  5, 20, // < black
  78, 34, 30  // > black
};
static const int colors[] = {3, 2, 4, 6, 1, 5, 7}; // yellow, green, blue, cyan, red, purple, black

int main(int argc, char *argv[])
{
  int f; // Frame counter
  int x; // X-Axis value
  int y; // Y-Axis value
  int i; // Circle counter
  int t; // Fill or Border toggler
  int j; // Brightness value
  int k; // Pixel flag
  int l; // Alias X-Axis sampler
  int color; // Leaf colouring, for debugging

  static const float S  =   0.9;      // Scale
         const float Xs =   0.5 / S; // X-Scale
         const float Ys =   1.1 / S; // Y-Scale
  static const float Rs =   0.9;      // Radius-Scale
  static const float Xt =  23;        // X-Translation
  static const float Yt =  15;        // Y-Translation
  static const float Rt =   5;        // Radius-Translation
  
  //puts("\x1b[0;34m"); // Set colour to blue
  for (f = F; f > 0; f--) // Frame counter loop
  {
    for (y = 0; y < H; y++) // Y-Axis loop (Subpixel-sampling)
    {
      for (x = 0; x < W; x++) // X-Axis loop
      {
	j = 0; // Initial brightness: dark
	color = 0; // Default colour is black
	for (l = 0; l < A; l++) // Sampling loop
	{
	  k = 0;
	  for (i = 0; i <= 7-1; i++) // Leaf loop (pick circle from back- to foreground)
	  {
	    for (t = 0; t <= 1; t++) // Inner/Border loop (toggle Fill & Border/Erase method)
	    {
	      if ( ( pow(Xs*(x+l) + Xt - C[6*6+0], 2) + pow(Ys*y + -Yt - C[6*6+1], 2) > pow(Rs*C[6*6+2] + (Rt+!t*B), 2) &&   // Mask to hide...
		     pow(Xs*(x+l) + Xt - C[6*5+3], 2) + pow(Ys*y + -Yt - C[6*5+4], 2) > pow(Rs*C[6*5+5] + (Rt+!t*B), 2) ) || // ...lower-half...
  		     y >= H*S)                                                                                                 // ...overflow
	      {
		continue;
	      }
	      if ( pow(Xs*(x+l) + Xt - C[6*i+0], 2) + pow(Ys*y + -Yt - C[6*i+1], 2) <= pow(Rs*C[6*i+2] + (Rt+!t*B), 2) && // Inside left...
		   pow(Xs*(x+l) + Xt - C[6*i+3], 2) + pow(Ys*y + -Yt - C[6*i+4], 2) <= pow(Rs*C[6*i+5] + (Rt+!t*B), 2) )  // ...and right circle?
	      {
		k = t; // Update current pixel with latest value (filled or erased)
		color = colors[i]; // Set colour of most foreground leaf
	      }
	    }
	  }
	  
	  j ^= k << l; // Increase pixel brightness: t=1,{l=0->1, l=1->2, l=2->4}.
	}
	
	//printf("\x1b[0;3%dm", color); // Colourize pixel
	putchar(K[j]); // Print pixel with brightness
      }
      putchar('\n'); // Next line
    }
    if (f > 1) // Last frame
    {
      usleep(1*1000*1000 / FPS); // 1s div FPS
      puts("\x1b[25A"); // Next frame: Move cursor up
    }
  }
  
  puts("\x1b[0m"); // Reset colour
  return EX_OK;
}
