// Converts MIDI pitch values in Q16.16 format into frequency values
// The output is 10.546ths of a Hertz (OPL3 base). That is, 1 unit translates to ~0.09482Hz or ~1/10.54587...
// A value of 0 << 16 means F#1, likewise 12 << 16 means F#2, and so on.
// This comes from Muffin. It was modified to use our neww base frq

uint32_t pitchToFrequency(uint32_t q16_pitch)
{
  // Split the Q16.16 pitch number into its Integer and Fractional components
  uint32_t pitch_integer_component = (q16_pitch >> 16) + 6; // +6 corrects for MIDI. This fuction begins at F#, but MIDI starts at C

  if (pitch_integer_component < 114) // Pitch 114
  {
    uint16_t pitch_fractional_component = q16_pitch & 0xFFFF;    
    uint16_t octave = (pitch_integer_component * 1366) >> 14;  // Compute octave number using pitch_integer/12 (This bit-shifting magic does just that but avoiding division which can be slow)
    uint8_t lut_index = pitch_integer_component - (12*octave); // Get the index into the fnumber lookup table

    // Interpolate between fnum_a and fnum_b based on the fractional part of pitch
    // Add in the octave number
    uint16_t fnum_a = lut_base_fnumber[lut_index];
    uint16_t fnum_b = lut_base_fnumber[lut_index + 1];
    return (fnum_a + ((pitch_fractional_component * (fnum_b - fnum_a)) >> 16)) << (octave);
  }
  return 0;
}