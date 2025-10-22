#pragma once

#include <Arduino.h>

/*
    Note most fine-tuning varibales are in Q16.16 format.
*/

class Voice
{
public:
    Voice();
    void clock();
    void setPhaseMaxCount(uint16_t max_count);
    void setSampleFrequency(uint32_t q16_frq);   
     
    void connectWaveRAM(uint16_t* wave_ram);
    void setFrequency(uint32_t q16_frq);


public:
    uint32_t q16_base_frq;
    uint16_t max_phase_count;
    uint16_t phase;
    uint16_t tune_word;
    uint16_t *wave;
    uint16_t wave_offset
    uint16_t sample;
};