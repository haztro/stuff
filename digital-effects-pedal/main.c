/* Includes */

#include <xc.h>
#include <p32xxxx.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include "init.h"
#include "delay.h"

/* Defines */

#define SYS_FREQ (48000000)

/* Variables */

signed int right_channel;
signed int data;
bool channel = 0;

/* Prototypes */

int AnalogRead(char pin);

/*******************************************************************************
 * FUNCTION NAME:   AnalogRead
 * 
 * DESCRIPTION:
 * 
 * Simple ADC read. Begin sampling and wait for sample. 
 * 
 * ARGUMENT LIST:
 * 
 * Argument     Type            IO      Description
 * --           -------------   --      ------------------------------
 * pin          unsigned char   I       The analog pin of the mcu 0-12
 *
 * RETURN VALUE:    
 * 
 * Returns the ADC value 0-1024 (int).
 ******************************************************************************/
int AnalogRead(char pin)
{
    AD1CHS = pin << 16;
 
    AD1CON1bits.SAMP = 1;   
    while( AD1CON1bits.SAMP );  
    while( ! AD1CON1bits.DONE ); 
    
    return ADC1BUF0;  
}

/*******************************************************************************
 * FUNCTION NAME:   __SPI1Interrupt
 * 
 * DESCRIPTION:
 * 
 * Interrupt service routine for the SPI peripheral. Reads SPI1BUF into 'data'
 * when SPI1BUF is full. Fills SPI1BUF with 'data' if SPI1BUF empty. 
 * 
 * ARGUMENT LIST:   void
 *
 * RETURN VALUE:    void
 ******************************************************************************/
void __ISR(_SPI1_VECTOR, IPL3AUTO)__SPI1Interrupt(void)
{
    // Recieve interrupt fired.
    if (IFS1bits.SPI1RXIF)
    {
        // Make sure we're only dealing with the right channel.
        if (channel)
        {
            right_channel = SPI1BUF;
            data = DELAY_Echo(right_channel);
        }
        else
        {
            int dummy_left_channel = SPI1BUF;
        }

        channel ^= 1;
    }
   
    // Transmit interrupt fired.
    if (IFS1bits.SPI1TXIF)
    {
        SPI1BUF = data;
    }   

    // Error interrupt fired.
    if (IFS1bits.SPI1EIF)
    {
        
    }
    
    IFS1bits.SPI1RXIF = 0;
    IFS1bits.SPI1TXIF = 0;
    IFS1bits.SPI1EIF = 0;
}

/*******************************************************************************
 * FUNCTION NAME:   DelayMicrosecond
 * 
 * DESCRIPTION:
 * 
 * Delays execution by specified number of microseconds. Only suitable for short
 * delays. 
 * 
 * ARGUMENT LIST:
 * 
 * Argument     Type            IO      Description
 * --           ------------    --      -----------------------------
 * us           unsigned int    I       Time to delay in microseconds
 * 
 * RETURN VALUE:      void
 ******************************************************************************/
void DelayMicrosecond(unsigned int us)
{
    us *= SYS_FREQ / 1000000 / 2;
    _CP0_SET_COUNT(0);
    
    while (us > _CP0_GET_COUNT()); 
}

/*******************************************************************************
 * FUNCTION NAME:   main
 * 
 * DESCRIPTION:
 * 
 * Main execution loop.
 * 
 * ARGUMENT LIST:   void
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void main(void) 
{
    // Initialise everything. 
    DELAY_InitDelayLines();
    INIT_InitReferenceClock();
    INIT_InitGPIO();
    INIT_InitADC();
    INIT_InitI2S();
    
    // Reset Codec (AK4556VT recommends that the device be reset once)
    LATBbits.LATB9 = 0;         // Set PDN low
    DelayMicrosecond(100);
    LATBbits.LATB9 = 1;         // Set PDN high
    DelayMicrosecond(100);
    
    int prev_val1 = 0;
    int prev_val2 = 0;
    int count = 0;
    
    while (1)
    {
        
        // This sucks will need to change this. 
        if (count >= 10000)
        {
            // Read two pot values
            int val1 = AnalogRead(5);
            int val2 = AnalogRead(2);

            // If they've changed enough update the delay effect. 
            if ((val1 - prev_val1) >= 10 || (val1 - prev_val1) <= -10)
            {
                prev_val2 = val2;
                DELAY_SetDelayDepth(val1 * 7);
            }
            
            if ((val2 - prev_val2) >= 10 || (val2 - prev_val2) <= -10)
            {
                prev_val1 = val1;
                DELAY_SetDecay(val2 / 128);
            }
            
            count = 0;
        }
        count++;
    }
    
    return;
}
