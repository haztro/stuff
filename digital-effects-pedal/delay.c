/* Includes */

#include "delay.h"
#include <p32xxxx.h>

/* Variables */

DelayLine_t dl1 = {0};
uint8_t downsample = 0;

/* Prototypes */

signed int DelayLineRead(DelayLine_t *dl);
void DelayLineWrite(DelayLine_t *dl, signed short sample);

/*******************************************************************************
 * FUNCTION NAME:   DELAY_DelayLineRead
 * 
 * DESCRIPTION:
 * 
 * Read the current value in the delay line buffer. Multiply by 2^8 to get back
 * to 24bit signed int. Increment the read index. 
 * 
 * ARGUMENT LIST:
 * 
 * Argument     Type            IO      Description
 * --           -------------   --      -----------------------
 * dl           DelayLine_t     I       A delay line structure. 
 * 
 * RETURN VALUE:    
 * 
 * The current delay line value (signed int).
 ******************************************************************************/
signed int DELAY_DelayLineRead(DelayLine_t *dl)
{
    signed short sample = dl->buffer[dl->read_index++];
    
    if (dl->read_index >= dl->delay_depth)
    {
        dl->read_index = 0;
    }
    
    return (signed int) (sample << 8);
}

/*******************************************************************************
 * FUNCTION NAME:   DELAY_DelayLineWrite
 * 
 * DESCRIPTION:
 * 
 * Write a sample to the current write position in the delay line. Increment the
 * write index.
 * 
 * ARGUMENT LIST:
 * 
 * Argument     Type            IO      Description
 * ------       ------------    ---     ------------------------------
 * dl           DelayLine_t     I/O     Delay line data structure. 
 * sample       signed short    I       Current sample read from codec.    
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void DELAY_DelayLineWrite(DelayLine_t *dl, signed short sample)
{   
    dl->buffer[dl->write_index++] = sample;
    
    if (dl->write_index >= dl->delay_depth)
    {
        dl->write_index = 0;
    }
}

/*******************************************************************************
 * FUNCTION NAME:   DELAY_Echo
 * 
 * DESCRIPTION:
 * 
 * Add the current delay line value to the current sample with delay line decay.
 * Only add a value to the delay line every 3 samples (effectly downsampling)
 * by a factor of three. This gives us a longer delay at the expensive of 
 * aliasing. 
 * 
 * ARGUMENT LIST:
 * 
 * Argument     Type            IO      Description
 * ------       ------------    ---     -------------------------------
 * sample       signed int      I       Current sample read from codec. 
 * 
 * RETURN VALUE:    The computed echo value (signed int). 
 ******************************************************************************/
signed int DELAY_Echo(signed int sample)
{
    signed int val;
    
    // How long it lasts
    if (downsample == 2)
    {
        val = sample + ((DELAY_DelayLineRead(&dl1) * (dl1.decay-1)) / dl1.decay);
        DELAY_DelayLineWrite(&dl1, (signed short) (val >> 8));
        downsample = 0;
    }
    else
    {
        val = sample + (signed int) (dl1.buffer[dl1.read_index] << 8);
    }
    downsample++;

    
    return val;
}

/*******************************************************************************
 * FUNCTION NAME:   DELAY_SetDelayDepth
 * 
 * DESCRIPTION:
 * 
 * Update the delay line struct with the most recent delay depth value read from
 * the ADC. 
 * 
 * ARGUMENT LIST: 
 * 
 * Argument     Type            IO      Description
 * ------       ------------    ---     -------------------------------
 * val          uint32_t        I       New delay depth read from ADC.
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void DELAY_SetDelayDepth(uint32_t val)
{
    dl1.read_index = 0;
    dl1.write_index = val - 1;
    dl1.delay_depth = val;
}

/*******************************************************************************
 * FUNCTION NAME:   DELAY_SetDecay
 * 
 * DESCRIPTION:
 * 
 * Update the delay line struct with the most recent decay value read from the 
 * ADC. The value is between 1 and 8. When applying the decay it is applied as
 * audio * (decay - 1) / decay ie. 1/2, 2/3, 3/4.. etc.
 * 
 * ARGUMENT LIST:
 * 
 * Argument     Type            IO      Description
 * ------       ------------    ---     -------------------------------
 * val          uint8_t         I       The new decay value.  
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void DELAY_SetDecay(uint8_t val)
{
    if (val < 2)
        val = 2;
    dl1.decay = val;
}

/*******************************************************************************
 * FUNCTION NAME:   DELAY_InitDelayLines
 * 
 * DESCRIPTION:
 * 
 * Initialise the delay line .
 * 
 * ARGUMENT LIST:   void
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void DELAY_InitDelayLines()
{
    dl1.read_index = 0;
    dl1.write_index = (NUM_SAMPLES / 2) - 1;
    dl1.delay_depth = (NUM_SAMPLES / 2);
    dl1.decay = 2;
}