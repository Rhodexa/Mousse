// Converts MIDI pitch values in Q16.16 format into frequency values
// The output is 10.546ths of a Hertz (OPL3 base). That is, 1 unit translates to ~0.09482Hz or ~1/10.54587...
// A value of 0 << 16 means F#1, likewise 12 << 16 means F#2, and so on.
// This comes from Muffin. It was modified to use our neww base frq

// C#1 to C#2
static constexpr uint32_t lut_base_fnumber[13] = { 857, 908, 962, 1020, 1080, 1144, 1212, 1284, 1361, 1442, 1528, 1618, 1715 };

// Outputs frequency values in Myell base (40000/2^20)
uint32_t pitchToFrequency(uint32_t q16_pitch)
{
  // Split the Q16.16 pitch number into its Integer and Fractional components
  uint32_t pitch_integer_component = (q16_pitch >> 16); // +6 corrects for MIDI. This fuction begins at F#, but MIDI starts at C

  if (pitch_integer_component < 114) // Pitch 114
  {
    uint16_t pitch_fractional_component = q16_pitch & 0x0000FFFF;

    // Compute octave number using pitch_integer/12 (This bit-shifting magic does just that but avoiding division which can be slow)
    uint16_t octave = (pitch_integer_component * 1366) >> 14; 
    uint8_t lut_index = pitch_integer_component - (12 * octave); // Get the index into the fnumber lookup table

    // Interpolate between fnum_a and fnum_b based on the fractional part of pitch
    // Add in the octave number
    uint32_t fnum_a = lut_base_fnumber[lut_index];  // Change type to uint32_t to accommodate 32-bit values
    uint32_t fnum_b = lut_base_fnumber[lut_index + 1];  // Same here, uint32_t for 32-bit values
    return (fnum_a + ((pitch_fractional_component * (fnum_b - fnum_a)) >> 20)) << (octave);
  }
  return 0;
}
