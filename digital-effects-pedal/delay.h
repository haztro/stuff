#ifndef DELAY_H
#define	DELAY_H

/* Includes */

#include <p32xxxx.h>

/* Defines */

#define SAMPLE_FREQ (44100)
#define NUM_SAMPLES (SAMPLE_FREQ / 6)

/* Typedefs */

typedef struct
{
    signed short buffer[NUM_SAMPLES];
    uint32_t read_index;
    uint32_t write_index;
    uint32_t delay_depth;
    uint8_t decay;
} DelayLine_t;

/* Function Prototypes */

signed int DELAY_Echo(signed int sample);
void DELAY_InitDelayLines();
void DELAY_SetDelayDepth(uint32_t val);
void DELAY_SetDecay(uint8_t val);

#endif	/* DELAY_H */