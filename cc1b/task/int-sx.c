// USING INTERRUPTS

#pragma chip SX28


interrupt int_server( void)
// located at address 0
{
    // W, STATUS and FSR are automatically saved

    // determine the source of the interrupt
    // process the interrupt

    // W, STATUS and FSR are automatically restored
}



void main( void)
{
    RA = 0;
    DDRA = 0;  // all outputs

    // enable interrupts

    while (1)  {  /* infinite loop */
        RA.1 = 0;
        nop();
        nop();
        RA.1 = 1;
    }
}
