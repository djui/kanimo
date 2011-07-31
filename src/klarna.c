/*
  Design decisions:
   * No float: Extra initialization space
   * No math: Extra include header file space
 */
#include <stdio.h>
#include <math.h>
#include <sysexits.h>

static const int FPS = 25;   // Frames per second; not too precise
static const int F   = 50;   // Frame amount
static const int A   =  3;   // Anti-Aliasing value
static const int W   = 46;   // Output width
static const int H   = 18;   // Output height
static const int B   =  1;   // Circle border size
static const int X   = 23;   // X-Translation
static const int Y   =  7;   // Y-Translation
static const float S =  0.5; // Scale
static const int K[] = {' ', '.', '.', ')', '.', '\\', '/', '@'}; // Pixel "brightness"
static const int C[] = {      // Circles' data, leaves in background to foreground order
// x, y, r
   0, 21, 48, // < yellow
  74, 11, 41, // > yellow
   0, 10, 51, // < green
  77, 23, 40, // > green
  11, 38, 34, // < blue
  46,  4, 21, // > blue
   0, 28, 48, // < cyan
  65,  9, 28, // > cyan
  20,  3, 41, // < red
  94, 31, 49, // > red
  13, 48, 38, // < purple
  46,  6, 24, // > purple
  43,  6, 24, // < black
  78, 37, 32  // > black
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

  const float Xs =  0.17 / S; // X-Scale
  const float Ys =  1.0  / S; // Y-Scale
  
  puts("\x1b[0;34m"); // Set colour to blue
  for (f = F; f > 0; f--) // Frame counter loop
  {
    for (y = 0; y < H; y++) // Y-Axis loop (Subpixel-sampling)
    {
      for (x = 0; x < W*3; x+=3) // X-Axis loop
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
	      if ( pow(Xs*(x+l) + X - C[6*6+0], 2) + pow(Ys*y + -Y - C[6*6+1], 2) > pow(C[6*6+2] - 8*(1-pow(1-(float)f/F,2)) + !t*B, 2) && // Mask to hide...
		   pow(Xs*(x+l) + X - C[6*5+3], 2) + pow(Ys*y + -Y - C[6*5+4], 2) > pow(C[6*5+5] - 8*(1-pow(1-(float)f/F,2)) + !t*B, 2) )  // ...lower overflow
	      {
		continue;
	      }
	      if ( pow(Xs*(x+l) + X - C[6*i+0], 2) + pow(Ys*y + -Y - C[6*i+1], 2) <= pow(C[6*i+2] - 8*(1-pow(1-(float)f/F,2)) + !t*B, 2) && // Inside left...
		   pow(Xs*(x+l) + X - C[6*i+3], 2) + pow(Ys*y + -Y - C[6*i+4], 2) <= pow(C[6*i+5] - 8*(1-pow(1-(float)f/F,2)) + !t*B, 2) )  // ...and right circle?
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
      usleep(1*1000*1000 / (FPS*3)); // 1s div FPS
      printf("\x1b[%dA", H); // Next frame: Move cursor up
    }
  }
  
  puts("\x1b[0m"); // Reset colour
  return EX_OK;
}
