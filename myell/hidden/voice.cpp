#include "voice.h"

Voice::Voice() 
{
    max_phase_count = 1024;
}

void Voice::setPhaseMaxCount(iunt16_t max_count)
{
    // Why strip people out of a sweet distortion potential?
    //max_phase_count = min(max_count, 32767);
    max_phase_count = max_count;
}

void Voice::setSampleFrequency(uint32_t q16_frq, uint32_t max_phase_count)
{
    q16_base_frq = (0x00010000 * max_phase_count) / q16_frq;
}


void Voice::connectWaveRAM(uint16_t* wave_ram)
{
    wave = wave_ram;
}

void Voice::setWaveOffset(uint16_t offset)
{
    wave_offset = offset;
}

void Voice::setFrequency(uint32_t frq)
{
    tune_word = frq * base_frq;
}

void Voice::clock()
{
    phase += tune_word;
    if (phase > max_phase_count)
        phase -= max_phase_count;
    //sample = wave[phase + wave_offset] & 0x03FF; // max output is 10 bit
    sample = phase;
}
