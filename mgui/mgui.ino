/*
    MGUI (Magui or Mousse GUI)

    This is Mousse's tracker and use interface core
*/

#include "display.h"
#include "limepie.h"

void drawNote(uint8_t note, uint8_t octave, bool is_sharp){
    Display::tile(Tile::note_letter, constrain(note, 0, 6), 4);

    if (is_sharp) {
        Display::space(1);
        Display::tile(Tile::note_sharp, 0, 1);
        Display::space(1);
    }
    else {
        Display::space(3);
    }
    
    Display::tile(Tile::note_octave, constrain(octave, 0, 9), 4);
    Display::space(9);
}

void drawTracker(uint8_t cursor){
    for(int i = 0; i < 8; i++)
    {
        Display::writeCmd(0xB0 + i);
        Display::writeCmd(0x10);
        Display::writeCmd(0x00);
        if (i == cursor) {
            Display::tile(Tile::cursor_arrow, 0, 6);
            Display::space(2);
        }
        else {
            Display::space(8);
        }

        drawNote(random(), 1, 1);
        drawNote(3, 2, 0);
        drawNote(6, 5, 1);
        drawNote(5, 1, 1);
        drawNote(1, 2, 0);
        drawNote(6, 5, 1);
    }
}

void setup(){
    Serial.begin(115200);
    Display::init();
    Display::writeCmd(0xB0);
    Display::writeCmd(0x10);
    Display::writeCmd(0x00);
     
}

void loop(){
    for(int i = 0; i < 8; i++){
        drawTracker(i);
        delay(60000/(110*4));
    }
}
