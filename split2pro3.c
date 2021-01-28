/* Serum-Format WAV to Pro 3 Sysex with split points
 *
 * Usage:
 * 
 * ./split2pro3 wavetable-number name serum1.wav s2.wav s3.wav s4.wav > sysex-file.syx
 * 
 * wavetable-number is a number between 33 and 64
 * if wavetable-number is 0, split2pro3 outputs raw PCM instead of sysex
 * name is a string of up to eight characters
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "sequential_lib/pro3_wavetable.h"
#define WAV_MAX 1050000
 
int main(int argc, char *argv[]) 
{
    /* The wavetable number is expected as a command-line argument */
    if (argc < 8) {
        fprintf(stderr, "\nusage: %s name wavetable-number frame-size serum1 s2 s3 s4 > serum.syx\n\n", argv[0]);
        return -1;
    }

    int wavetable_number = atoi(argv[2]);
    if (wavetable_number > 64 || wavetable_number < 33) {
        fprintf(stderr, "\nwavetable number out of range (33-64)\n\n");
        return -1;
    }
    wavetable_number--; /* Because wavetable numbers are zero-indexed to the Pro3 */

    int frame_size = atoi(argv[3]);
    if (frame_size < 128) {
        fprintf(stderr, "\nframe size out of range (>= 128)\n\n");
        return -1;
    }

    FILE *fp;
    int byte;
    int argnum;

    Wavetable table[4];
    int ref_num = 0;
    int t;
    for (t = 0; t < 4; t++)
    {
        table[t] = new_Wavetable();

        if ((fp = fopen(argv[4+t], "r")) == NULL) {
            fprintf(stderr, "\nwav file not found\n\n");
            return -1;
        } else {
            unsigned long int size = 0;
            int* wav = (int*)malloc(WAV_MAX * sizeof(int));
            while ((byte = getc(fp)) != EOF)
            {
                wav[size++] = byte;
                if (size == WAV_MAX) {
                    fprintf(stderr, "wav too big\n\n");
                    return -1;
                }
            }
            fclose(fp);
            WAVMeta meta = get_wav_meta(size, wav);
            
            int orig_size = meta.samples / frame_size;

            float ref_dist = (float) PRO3_WAVES / (float) orig_size;
            float ref_num = 0;
            PCMData ref;
            int wave_num;
            for (wave_num = 0; wave_num < orig_size; wave_num++)
            {
                ref = wav_extract(meta, wav, wave_num * frame_size, frame_size);
                pcm_change_size(&ref, PRO3_SAMPLE_SIZE);
                if (!(wave_num > 0 && (int) ref_num == 0)) { /* Protect the first waveform */
                    set_reference(&table[t], &ref, (int) ref_num);
                    /* fprintf(stderr, "Wave: %i  Ref: %i\n", wave_num, (int) ref_num); */
                }
                ref_num += ref_dist;
            }
            if ((int) (ref_num - ref_dist) < 15) {
                /* The library fills the first reference into the last, so this is here
                * to override that behavior */
                set_reference(&table[t], &ref, 15);
            }
            free(wav);
            wavetable_fill(&table[t]);
        }

    }

    split_sysex_dump(&table[0], &table[1], &table[2], &table[3], wavetable_number, argv[1]);

    return 0;
}

/*
 * Copyright (c) 2020 The Beige Maze Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
