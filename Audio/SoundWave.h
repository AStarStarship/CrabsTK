// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_AUDIO_SOUNDWAVE_DECL_DECL
#define CRABSTK_AUDIO_SOUNDWAVE_DECL_DECL
#include <_Config.h>
namespace _ {

struct SoundWave {
  IUA riff_id[4];    //< 'RIFF' chunk.
  IUC ruff_size;     //< filesize - 8.
  IUA wave_type[4];  //< 'WAVE' filetype.
  IUC fmt_size;      //< Format chunk size.
  IUB fmt_code;      //< 1 = PCM.
  IUB channels;      //< 1 = mono, 2 = stereo.
  IUC sample_rate;   //< sampling frequency, 44.1KHz for CD quality.
  IUC avg_bs;        //< samplerate * align.
  IUB align;         //< (channels * bits) / 8.
  IUB bits;          //< Sample bit depth, 16 for CD quality.
  IUA wave_id[4];    //< 'data' chunk.
  IUC wave_size;     //< Size of sample data.
};

}  //< namespace _
#endif
