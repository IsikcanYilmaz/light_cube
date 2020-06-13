#include "random_fade_animation.h"
#include "addressable_led_manager.h"
#include "utils.h"
#include <stdlib.h>

/*
 *  RANDOM TRIANGLES ANIMATION MODULE 
 *

 */

typedef struct {
  uint8_t numPanels;
  uint8_t numLeds;

  // This will be a ptr to an array of consecutive panel objects
  AddrLEDPanel_t *panels; 

  // Below is tobe used as storage for metadata about the Pixels that this animation module will keep
  RandomFadePixelData_t *pixelDataPtr;
  
  // Below are animation config variables
  // lower and upper limits for the random numbers
  uint8_t upperBrightness;
  uint8_t lowerBrightness;

  // Pointer to the strip. assuming there's only one
  AddrLEDStrip_t *stripBegin;
} AnimationContext_t; 

static AnimationContext_t context;

void Animation_RandomTriangles_Init(AddrLEDPanel_t *panels, uint8_t numPanels, RandomFadePixelData_t *pixelDataPtr)
{
  context.panels = panels;
  context.pixelDataPtr = pixelDataPtr;
  context.stripBegin = panels->strip;
  context.numPanels = numPanels;
  context.numLeds = panels->numLeds * numPanels;

  context.lowerBrightness = 0;
  context.upperBrightness = 10;
}

void Animation_RandomTriangles_Update(void)
{
    static uint8_t testx = 0;
    static uint8_t testy = 0;
    static Pixel_t col1 = {0, 10, 10};
    static uint32_t count = 0;

    // DO ALL SIDES BESIDES THE TOP
    for (int side = 0; side < TOP; side++)
    {
      if (count % 2 == 0)
      {
        for (int j = 0; j < 4; j++)
        {
          if (testx >= j)
          {
            Pixel_t *p1 = GetPixelByLocalCoordinate(side, testx-j, testy+j);
            *p1 = col1;
          }
        }
      }
      else
      {
        for (int j = 0; j < 4; j++)
        {
          if (3-testx >= j)
          {
            Pixel_t *p1 = GetPixelByLocalCoordinate(side, 3-(2-testx-j), 3-(testy+j));
            *p1 = col1;
          }
        }
      }
    }

    // DO TOP DIFFERENTLY
    for (int j = 0; j < 4; j++)
    {
      for (int k = 0; k < 4; k++)
      {
        Pixel_t *p;
        switch(count % 4)
        {
          case 0:
            p = GetPixelByLocalCoordinate(TOP, testx, k);
            break;
          case 1:
            p = GetPixelByLocalCoordinate(TOP, testx, 3-k);
            break;
          case 2:
            p = GetPixelByLocalCoordinate(TOP, 3-testx, k);
            break;
          case 3:
            p = GetPixelByLocalCoordinate(TOP, 3-testx, 3-k);
            break;
        }
        *p = col1;
      }
    }

    testx++;
    if (testx > 3)
    {
      testx = 0;
      count++;
      if (count % 2 == 0)
      {
        col1 = (Pixel_t) {
          RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness), \
          RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness), \
          RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness)
        };
      }
    }
}