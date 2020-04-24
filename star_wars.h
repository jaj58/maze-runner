#include "allcode_api.h"

/*
 * found a version on the internet and mofidied it to work with the robots' functions
 * used as a fun way to signal that we have explored all cells in the maze.
 */

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

void play_star_wars_theme_tune(){
  FA_PlayNote(a, 500);
  FA_PlayNote(a, 500);    
  FA_PlayNote(a, 500);
  FA_PlayNote(f, 350);
  FA_PlayNote(cH, 150);  
  FA_PlayNote(a, 500);
  FA_PlayNote(f, 350);
  FA_PlayNote(cH, 150);
  FA_PlayNote(a, 650);
  
  FA_DelayMillis(500);
  
  FA_PlayNote(eH, 500);
  FA_PlayNote(eH, 500);
  FA_PlayNote(eH, 500);  
  FA_PlayNote(fH, 350);
  FA_PlayNote(cH, 150);
  FA_PlayNote(gS, 500);
  FA_PlayNote(f, 350);
  FA_PlayNote(cH, 150);
  FA_PlayNote(a, 650);
  
  FA_DelayMillis(500);
 
  FA_PlayNote(aH, 500);
  FA_PlayNote(a, 300);
  FA_PlayNote(a, 150);
  FA_PlayNote(aH, 500);
  FA_PlayNote(gSH, 325);
  FA_PlayNote(gH, 175);
  FA_PlayNote(fSH, 125);
  FA_PlayNote(fH, 125);    
  FA_PlayNote(fSH, 250);
 
  FA_DelayMillis(325);
 
  FA_PlayNote(aS, 250);
  FA_PlayNote(dSH, 500);
  FA_PlayNote(dH, 325);  
  FA_PlayNote(cSH, 175);  
  FA_PlayNote(cH, 125);  
  FA_PlayNote(b, 125);  
  FA_PlayNote(cH, 250);  
 
  FA_DelayMillis(350);
 
  FA_PlayNote(f, 250);  
  FA_PlayNote(gS, 500);  
  FA_PlayNote(f, 350);  
  FA_PlayNote(a, 125);
  FA_PlayNote(cH, 500);
  FA_PlayNote(a, 375);  
  FA_PlayNote(cH, 125);
  FA_PlayNote(eH, 650);
 
  FA_DelayMillis(500);
 
  FA_PlayNote(aH, 500);
  FA_PlayNote(a, 300);
  FA_PlayNote(a, 150);
  FA_PlayNote(aH, 500);
  FA_PlayNote(gSH, 325);
  FA_PlayNote(gH, 175);
  FA_PlayNote(fSH, 125);
  FA_PlayNote(fH, 125);    
  FA_PlayNote(fSH, 250);
 
  FA_DelayMillis(325);
 
  FA_PlayNote(aS, 250);
  FA_PlayNote(dSH, 500);
  FA_PlayNote(dH, 325);  
  FA_PlayNote(cSH, 175);  
  FA_PlayNote(cH, 125);  
  FA_PlayNote(b, 125);  
  FA_PlayNote(cH, 250);  
 
  FA_DelayMillis(350);
 
  FA_PlayNote(f, 250);  
  FA_PlayNote(gS, 500);  
  FA_PlayNote(f, 375);  
  FA_PlayNote(cH, 125);
  FA_PlayNote(a, 500);  
  FA_PlayNote(f, 375);  
  FA_PlayNote(cH, 125);
  FA_PlayNote(a, 650);  
 
  FA_DelayMillis(650);
}