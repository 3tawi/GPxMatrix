/*
 * Adapted by 3tawi
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef AudioLogic_H
#define AudioLogic_H

#define AUDIO_PIN 34
#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT();

#define NOISE           10    
#define bandCount       16            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define SAMPLES         128          // Must be a power of 2
double vReal[SAMPLES];
double vImag[SAMPLES];           
int bandValues[bandCount];  
int levels[bandCount];  
int totalevels = 0;


void ReadAudio() {

  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = 4096 - analogRead(AUDIO_PIN);
    vImag[i] = 0.0;
  }

  // Compute FFT
  FFT.DCRemoval(vReal, SAMPLES);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
  // Analyse FFT results
  for (int i = 1; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
    //16 bands, 12kHz top band
      if (i<=2 )           bandValues[0]  =  max(bandValues[0], (int)vReal[i]);
      if (i>2   && i<=3  ) bandValues[1]  =  max(bandValues[1], (int)vReal[i]);
      if (i>3   && i<=5  ) bandValues[2]  =  max(bandValues[2], (int)vReal[i]);
      if (i>5   && i<=7  ) bandValues[3]  =  max(bandValues[3],  (int)vReal[i]);
      if (i>7   && i<=9  ) bandValues[4]  =  max(bandValues[4],  (int)vReal[i]);
      if (i>9   && i<=12 ) bandValues[5]  =  max(bandValues[5],  (int)vReal[i]);
      if (i>12  && i<=15 ) bandValues[6]  =  max(bandValues[6],  (int)vReal[i]);
      if (i>15  && i<=18 ) bandValues[7]  =  max(bandValues[7],  (int)vReal[i]);
      if (i>18  && i<=22 ) bandValues[8]  =  max(bandValues[8],  (int)vReal[i]);
      if (i>22  && i<=26 ) bandValues[9]  =  max(bandValues[9],  (int)vReal[i]);
      if (i>26  && i<=31 ) bandValues[10] =  max(bandValues[10], (int)vReal[i]);
      if (i>31  && i<=36 ) bandValues[11] =  max(bandValues[11], (int)vReal[i]);
      if (i>36  && i<=42)  bandValues[12] =  max(bandValues[12], (int)vReal[i]);
      if (i>42 && i<=48)   bandValues[13] =  max(bandValues[13], (int)vReal[i]);
      if (i>48 && i<=56)   bandValues[14] =  max(bandValues[14], (int)vReal[i]);
      if (i>56          )  bandValues[15] =  max(bandValues[15], (int)vReal[i]);
    }
  }
  // Process the FFT data into bar heights
  totalevels = 0;
  for (byte band = 0; band < bandCount; band++) {
    // Scale the bars for the display
    // int barHeight = bandValues[band];
    // Serial.println(barHeight);
    int barHeight = map(bandValues[band], 0, sp4, 0, 15);
    if (barHeight > 15) barHeight = 15;
    totalevels += barHeight;
    if (levels[band] < barHeight) levels[band] = barHeight;
    bandValues[band] = 0;
  }
  totalevels /= bandCount;
  totalevels = map(totalevels, 0, 15, -180, 180);
}

#endif
