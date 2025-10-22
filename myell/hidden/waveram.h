#pragma once

uint16_t wave_ram[8 * 1024];

void WaveRAM_init(){
    // make some basic waveforms by default:

    // square
    for(int i = 0; i < 1024; i++) {
        wave_ram[i + 1024*0] = i < 512 ? 1023 : 0;
    }

    // sawtooth
    for(int i = 0; i < 1024; i++) {
        wave_ram[i + 1024*1] = i;
    }    

    // sine
    for(int i = 0; i < 1024; i++) {
        wave_ram[i + 1024*2] = (uint16_t)(((sin((TWO_PI/1024.0f) * (float)i) + 1)/2) * 1023);
    }
}