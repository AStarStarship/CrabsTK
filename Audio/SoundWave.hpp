// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_AUDIO_SOUNDWAVE_TEMPLATES
#define KABUKITOOLKIT_AUDIO_SOUNDWAVE_TEMPLATES
#include <_Config.h>
#include "SoundWave.h"
namespace _ {

template <typename IS>
void TSoundStore(const CHR* filename, const IS* sample_buffer,
                 ISN sample_count, ISN channels, ISN bit_depth = 16,
                 ISN sample_rate = 44100) {
  ISN byte_count = (sample_count * channels) * sizeof(IS);

  SoundWave aw;
  aw.riff_id[0] = 'R';
  aw.riff_id[1] = 'I';
  aw.riff_id[2] = 'F';
  aw.riff_id[3] = 'F';
  aw.ruff_size = byte_count + sizeof(aw) - 8;
  aw.wave_type[0] = 'W';
  aw.wave_type[1] = 'A';
  aw.wave_type[2] = 'V';
  aw.wave_type[3] = 'E';
  aw.fmt_id[0] = 'f';
  aw.fmt_id[1] = 'm';
  aw.fmt_id[2] = 't';
  aw.fmt_id[3] = ' ';
  aw.fmt_size = 16;
  aw.fmt_code = 1;
  aw.channels = (IUB)channels;
  aw.sample_rate = (IUC)sample_rate;
  aw.bits = bit_depth;
  wav_align = (aw.channels * aw.bits) >> 3;  // >> 3 means /8
  aw.align = wav_align;
  aw.avg_bps = sample_rate * wav_align;
  aw.wav_id[0] = 'd';
  aw.wav_id[1] = 'a';
  aw.wav_id[2] = 't';
  aw.wav_id[3] = 'a';
  aw.wave_size = byte_count;

  FILE* f = fopen(filename, "wb");
  fwrite(&aw, 1, sizeof(aw), f);
  fwrite(sample_buffer, 2 * channels, sample_total, f);
  fclose(f);
}

}  //< namespace _

#endif
