#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/api.h"
#include "stdio.h"
// Global Variables

unsigned int Count = 0x0;
unsigned int move_engine_hal = 0;
volatile unsigned int R1 = 0; // Variable to store the time of rising edge
volatile unsigned int R2 = 0; // Variable to store the time of falling edge
volatile unsigned char risingEdgeDetected = 0; // Flag to indicate rising edge detection
volatile unsigned char isFirstEdge = 1; // Flag to indicate if it's the fir
volatile unsigned int DALTA_ECHO;
float range;
float C_div_SMCLK= 0.0162125;
unsigned int j = 0;
int data_to_send[123];
int idx = 0;
int measure1;
int measure2;
int telemeter_flag = 0;
unsigned int flag= 0;
extern int angle=0;



//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------

//void setmove_engine(int* from_api){

//}

void sysConfig(void){ 
    UART_init();
    GPIOconfig();


}

//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  //
    volatile unsigned int i;

    for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            General Function - No need
//---------------------------------------------------------------------
void int2str(char *str, unsigned int num){
    int strSize = 0;
    long tmp = num, len = 0;
    int j;
    // Find the size of the intPart by repeatedly dividing by 10
    while(tmp){
        len++;
        tmp /= 10;
    }

    // Print out the numbers in reverse
    for(j = len - 1; j >= 0; j--){
        str[j] = (num % 10) + '0';
        num /= 10;
    }
    strSize += len;
    str[strSize] = '\0';
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
    if (LPM_level == 0x00)
      _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
      _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
      _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
    else if(LPM_level == 0x03)
      _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
      _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
 }
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}


//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){

    unsigned char i;
    for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec

}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){

    unsigned char i;
    for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec

}
void SET_TIMER_A0_config(int ccr1){
    TA0CCR1 = ccr1;
    flag = 0;

}

void delay_of70ms(void) {
    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR; // SMCLK, Divide by 1, Up mode, Clear Timer
    TA0CCR0 = 30000; // Set CCR0 to its maximum value (65535) for a long delay
    TA0CCTL0 |= CCIE; // Enable CCR2 interrupt
    flag =1;
    __bis_SR_register(LPM0_bits + GIE);
}

void SET_TIMERA1_TRIGER(void)
{    TA1CTL = TASSEL_2 + MC_1; // use smclk 2^20 and up mode
    TA1CCTL0 |= CCIE; // Enable Timer A0 interrupt
    }

void dealy_us(int usec){
    TA1CTL = TASSEL_2 + MC_1; // use smclk 2^20 and up mode
    TA1CCTL0 |= CCIE; // Enable Timer A0 interrupt
    TA1CCR0 = usec; // for the value usec = 15 we get 14.3 us
    TA1CCTL0 &= ~CCIFG;
}

void start_puls(int usec){
    trigerOut |= triger; // set p1.0 to 1
    dealy_us(usec); // make dealy of 15 us for the pulse
}

void STOP_TIMERA0(void){
    //PWMDIR &= ~PWM;
    //PWMSEL |= PWM;
    TA0CTL = MC_0;
    TA0CCTL0 &= ~CCIFG; // Clear CCR0 interrupt flag
    TA0CCTL1 &= ~CCIFG; // Clear CCR1 interrupt flag
    TA0CCTL2 &= ~CCIFG;
    // Clear the TimerA0 counter (reset to zero)
    TA0R = 0; // Set the timer counter to zero

}

void STOP_TIMERA1(void)
{TA1CTL = MC_0;
    }

void set_TimerA1_echo(void)
{   //TA1CCTL1 &= ~CCIFG;

    TA1CTL = TASSEL_2 | MC_2 ;
    TA1CCTL1 = CM_3 | CCIS_1 | CAP  | CCIE; // Capture mode, both edges, CCI1B input, asynchronous capture, interrupt enabled
    if (j > 1){__bis_SR_register(LPM0_bits + GIE);}
    // smclk , contnous mode and clear
    }
//*********************************************************************
//                        TIMER A0 ISR
//*********************************************************************
#pragma vector = TIMER0_A0_VECTOR // For delay
__interrupt void TimerA_ISR (void)
{
    if(flag == 0){
    move_engine_hal++;

    P2OUT |= PWM;
    TA0CCTL0 &= ~CCIFG;
    }
    else
        TA0CCTL0 &= ~CCIFG;
        LPM0_EXIT;

}




//*********************************************************************
//                        TIMER A ISR
//*********************************************************************
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
    switch (TA0IV)
    {
        case TA0IV_TACCR1: // CCR1 match
            P2OUT &= ~PWM;
            TA0CCTL1 &= ~CCIFG;
            break;

        // Add cases for other CCRx interrupts if needed

        default:
            break;

}
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1_ISR(void) {
    trigerOut &= ~triger;
    TA1CCTL0 &= ~CCIE; // Clear Timer A0 interrupt flag

}

void clculate(int R1,int R2,int i)
{//if (risingEdgeDetected)
{   DALTA_ECHO = R2-R1;
    R2 = 0;
    R1 = 0;
    range = DALTA_ECHO*C_div_SMCLK;
    if(range >499)
        {range = 0;}
    data_to_send[i] = (int) (range/2);
    }
    }

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TimerA1_CCR1_ISR(void) {
    // This interrupt is triggered on a capture event (both edges of P2.2)
    if (risingEdgeDetected) {
        j++;
        // If falling edge (change from 1 to 0) detected
        R2 = TA1CCR1;
        TA1CTL = MC_0; // Stop TimerA1 after capturing R2

        // Wake up from LPM3 after the falling edge
        LPM0_EXIT;
    }
    else {
        // If rising edge (change from 0 to 1) detected
        R1 = TA1CCR1;

        if (isFirstEdge) {
            isFirstEdge = 0; // Set the flag to indicate that the first edge has occurred

            // Go back to sleep after the first rising edge
            TA1CCTL1 &= ~CCIFG; // Clear the capture interrupt flag
            TA1CTL = MC_2; // Start TimerA1 in continuous mode
            //risingEdgeDetected = !risingEdgeDetected;


        }
    }


    risingEdgeDetected = !risingEdgeDetected; // Toggle the risingEdgeDetected flag for the next edge detection
    TA1CCTL1 &= ~CCIFG; // Clear the capture interrupt flag
    //__bis_SR_register(LPM3_bits | GIE);

    }




//*********************************************************************
//                         ADC10 ISR
//*********************************************************************
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    LPM0_EXIT;
}



int calculate_for_one_semple(int list_range[123]){
    float simple;
    simple = ((list_range[15]+list_range[14]+list_range[13])/3);
    return (int) simple;
}


void clculate_ldr(int i,int calibration_flag,int measure1,int measure2){
    int dalta_measure = ((measure1+measure2)/2);
    if (calibration_flag==0)
                   {data_to_send[i] = (dalta_measure/4);}


}



  // ----------------------- RX ISR ------------------------------------------

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)

{
    if(telemeter_flag == 1){
        angle = 0xFF & UCA0RXBUF;
        telemeter_flag = 0;
    }
    else{
      switch (UCA0RXBUF) {
          case '1':
              state = state1;      //object detector
              break;
          case '2':
              telemeter_flag = 1;
              state = state2;        //telemeter
              break;
          case '3':
              state = state3;        //lights_detector
              break;
          case '4':
              state = state4;        //LDR_meas
              break;
          case '5':
              state = state5;        //Object_and_light_scan
              break;
      }
      LPM0_EXIT;
    }
}
  // ----------------------- TX ISR ------------------------------------------

  #pragma vector=USCIAB0TX_VECTOR
  __interrupt void USCI0TX_ISR(void)
      {
          UCA0TXBUF = data_to_send[idx++];
          if(data_to_send[idx - 1] == '\n'){
              idx = 0;
              IE2 &= ~UCA0TXIE;
          }
      }


  void UART_init(void){
      if (CALBC1_1MHZ==0xFF)                  // If calibration constant erased
        {
          while(1);                               // do not load, trap CPU!!
        }
      DCOCTL = 0;                               // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
      DCOCTL = CALDCO_1MHZ;

      P2DIR = 0xFF;                             // All P2.x outputs
      P2OUT = 0;                                // All P2.x reset
      P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
      P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
      P1DIR |= RXLED + TXLED;
      P1OUT &= 0x00;

      UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
      UCA0BR0 = 104;                           //
      UCA0BR1 = 0x00;                           //
      UCA0MCTL = UCBRS0;               //
      UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  }
