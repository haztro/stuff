/* Includes */

#include <p32xxxx.h>
#include "init.h"

/*******************************************************************************
 * FUNCTION NAME:   INIT_InitReferenceClock
 * 
 * DESCRIPTION:
 * 
 * Initialises REFCLK to run at 11.2896 MHz. We obtain 11.2896 MHz from the 
 * SYS_CLK (48 Mhz) then set RODIV = 2 and ROTRIM = 64 giving
 * (SYS_CLK / 2) / (2.1258) = 11.2896 Mhz
 * 
 * ARGUMENT LIST:   void
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void INIT_InitReferenceClock(void)
{
    REFOCONbits.ACTIVE = 0;     // Set REFCLK request status to inactive
    REFOCONbits.ON = 0;         // Disable REFCLK
    REFOCONbits.OE = 1;         // Output REFCLK to pin
    REFOCONbits.ROSEL = 0;      // Select SYSCLK as source for REFCLK
    REFOCONbits.RODIV = 2;      // Set divider to 2 (=SYSCLK / 2)
    REFOTRIMbits.ROTRIM = 64;   // 64/512 => 2 + 0.1258
    REFOCONbits.ON = 1;         // Enable REFCLK
}

/*******************************************************************************
 * FUNCTION NAME:   INIT_InitGPIO
 * 
 * DESCRIPTION:
 * 
 * Initialises necessary GPIO. Reroutes REFCLK, LRCK1, DIN and DOUT to the
 * relevant pins then sets TRIS registers accordingly.
 * 
 * ARGUMENT LIST:   void
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void INIT_InitGPIO(void)
{
        // Pin mappings
    SYSKEY = 0x0;               // Unlock sequence
    SYSKEY = 0xAA996655;         
    SYSKEY = 0x556699AA;         
    CFGCONbits.IOLOCK = 0;         
    RPB2Rbits.RPB2R = 0b0111;   // REFCLK to RPB2
    RPB4Rbits.RPB4R = 0b0011;   // LRCK1 (SS1) to RPB4
    SDI1Rbits.SDI1R = 0b0010;   // DIN (SDI1) to RPB1
    RPB8Rbits.RPB8R = 0b0011;   // DOUT (SDO1) to RPB8
    CFGCONbits.IOLOCK = 1;      // Lock Sequence   
    SYSKEY = 0x0;            
    
    // Set all to digital IO
    ANSELA = 0;                 // Clear ANSEL registers, set all IO to digital
    ANSELB = 0;

    // Initialise as input/output
    TRISBbits.TRISB1 = 1;       // DIN 
    TRISBbits.TRISB2 = 0;       // REFCLK
    TRISBbits.TRISB4 = 0;       // LRCK1
    TRISBbits.TRISB8 = 0;       // DOUT
    TRISBbits.TRISB14 = 0;      // BCK1
    TRISBbits.TRISB9 = 0;       // PDN
}

/*******************************************************************************
 * FUNCTION NAME:   INIT_InitI2S
 * 
 * DESCRIPTION:
 * 
 * Initialises SPI peripheral to I2S mode (mono) sampling at 44.1 kHz. Interrupt
 * driven. 
 * 
 * ARGUMENT LIST:   void
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void INIT_InitI2S(void)
{
     // Global interrupt enable. 
    asm volatile("ei");
    INTCONbits.MVEC = 1;        // Configure for multi vector mode.
    
    // Disable Interrupts 
    IEC1bits.SPI1EIE = 0;       // disable spi error interrupt
    IEC1bits.SPI1RXIE = 0;      // disable spi rx interrupt
    IEC1bits.SPI1TXIE = 0;      // disable spi tx interrupt
    SPI1CON = 0;                // Clear SPI1CON
    SPI1CON2 = 0;               // Clear SPI1CON2
    uint32_t dummy = SPI1BUF;             // Read SPI1BUF to clear. 
    
    SPI1STATbits.SPIROV = 0;    // Clear overflow flag bit
    SPI1CON2bits.AUDEN = 1;     // Enable audio codec support 
    SPI1CON2bits.AUDMONO = 1;   // Set to mono mode 
    SPI1CON2bits.AUDMOD = 0;    // Set to I2S mode
    SPI1CON2bits.IGNROV = 1;    // Ignore receive overflow
    SPI1CON2bits.IGNTUR = 1;    // Ignore transmit underrun 
    
    SPI1BRG = 1;                // Baudrate = (11.2896Mhz / (2*44.1khz*2*32bits)) - 1
    SPI1CONbits.MCLKSEL = 1;    // REFCLK is used by the baudrate generator
    SPI1CONbits.MSSEN = 1;      // Automatic slave select
    SPI1CONbits.SSEN = 1;       // Slave select enable
    SPI1CONbits.MSTEN = 1;      // Set to master mode
    SPI1CONbits.CKP = 1;        // Clock polarity select
    SPI1CONbits.FRMPOL = 0;     // Frame sync polarity?
    SPI1CONbits.MODE32 = 1;     // 24 bit data, 32 bit frame
    SPI1CONbits.MODE16 = 1;     // 24 bit data, 32 bit frame
    SPI1CONbits.FRMEN = 0;      // Framed SPI support disabled
    SPI1CONbits.ENHBUF = 0;     // Use standard buffer
    SPI1CONbits.STXISEL = 0b11; // Generate interrupt before buffer full.
            
    // Interrupts
    IFS1bits.SPI1EIF = 0;       // clear spi error interrupt 
    IFS1bits.SPI1RXIF = 0;      // clear spi rx interrupt
    IFS1bits.SPI1TXIF = 0;      // clear spi tx interrupt
    IPC7bits.SPI1IP = 3;        // Set priority to 3
    IPC7bits.SPI1IS = 1;        // Set sub-priority to 1
    IEC1bits.SPI1EIE = 1;       // Enable spi error interrupt
    IEC1bits.SPI1RXIE = 1;      // Enable spi rx interrupt
    IEC1bits.SPI1TXIE = 1;      // Enable spi tx interrupt
    
    SPI1CONbits.ON = 1;         // Enable spi peripheral   
}

/*******************************************************************************
 * FUNCTION NAME:   INIT_InitADC
 * 
 * DESCRIPTION:
 * 
 * Initialises the ADC peripheral in polling mode. Set pins B3 and B0 to analog
 * mode.
 * 
 * ARGUMENT LIST:   void
 * 
 * RETURN VALUE:    void
 ******************************************************************************/
void INIT_InitADC(void)
{
    AD1CON1 = 0;
    
    ANSELBbits.ANSB3 = 1;
    TRISBbits.TRISB3 = 1;
        
    ANSELBbits.ANSB0 = 1;
    TRISBbits.TRISB0 = 1;
    
    AD1CON1SET = 0x8000;
    AD1CON1bits.ON = 1;
    AD1CON1bits.SSRC = 0b111;   // Counter ends sampling and starts conversion
    AD1CON2 = 0;                // References to Vdd Vss
    AD1CON3bits.ADCS = 0b0001;  // ADC conversion clock bits
    AD1CON3bits.SAMC = 0b01111; // Auto sample time bits. 
    
}