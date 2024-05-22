#include  "../header/api.h"         // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"


// Global Variables
float one_simple;
float measure;
int dalta_measure;
int final_measure;
unsigned int nenty_dgree =202;
unsigned int finsh_calibartion= 0;
int measure1;
int measure2;
int flag_finsh_calibartion =0;
int flag_clean = 1;
int flag_clean_ldr = 0;





void scan180degree(void){
    clear_data_to_send();


    int dgree = 68;// set dgree to -12 dont take the 3 first simpale
    int i = 0; // i its the number of jump in the dagree

    TIMER_A0_config();


    while(i <= 80){
    if (i<=63){
        if (move_engine_hal == 4)

        {   STOP_TIMERA0(); // stop the engine for triger and acho
            delay_of70ms(); // give delay of 70 mesc using again timer A0
            STOP_TIMERA0(); // Clr timer A0 for next iteration
            start_puls(15);// start puls of 14.3 us
            set_TimerA1_echo(); // start echo
            clculate(R1,R2,i); // clculate the dalta and save in array
            TIMER_A0_config(); // config new PWM for next iteration
            SET_TIMER_A0_config(dgree); // set new degree for move the engine

            i++;
            dgree = dgree+4;
            move_engine_hal =0;
        }
    }
    else{ // return to 0 dgree
        if(move_engine_hal == 4)
        {
            SET_TIMER_A0_config(79);
            i++;
            move_engine_hal =0;
        }
        if(move_engine_hal == 4 && i == 80)
            {STOP_TIMERA0();
                        }
    }
    }

    STOP_TIMERA0();



}


void Telemeter(void)
{   clear_data_to_send();
    __bis_SR_register(LPM0_bits + GIE);
    TIMER_A0_config();
    int i = 0;
    SET_TIMER_A0_config(207);
    int deg  = (int) (78.6432 + angle*1.384);
    TIMER_A0_config();


    while(i <= 35){
        if (i<=15){
            if (move_engine_hal == 4)
            {
                STOP_TIMERA0(); // stop the engine for triger and acho
                delay_of70ms(); // give delay of 70 mesc using again timer A0
                STOP_TIMERA0(); // Clr timer A0 for next iteration
                start_puls(15);// start puls of 14.3 us
                set_TimerA1_echo(); // start echo
                clculate(R1,R2,i); // clculate the dalta and save in array
                TIMER_A0_config(); // config new PWM for next iteration
                SET_TIMER_A0_config(deg); // set new dagree for move the engine

                i++;
                move_engine_hal =0;


            }
        }
        else{
                if(move_engine_hal == 4)
                {
                    SET_TIMER_A0_config(79);
                    i++;
                    move_engine_hal =0;
                }
            }
       }

            STOP_TIMERA0();
            one_simple = calculate_for_one_semple(data_to_send);
            data_to_send[0] =one_simple;
            data_to_send[1] = '\0';

    }



void scan180degreeLDR(int start_dgree,int scan,int calibration_flag){
    if(flag_clean_ldr ==1)
        {clear_data_to_send();}
    int dgree = 79;// set dgree to 0 ;
    int i = 0; // i its the number of jump in the dagree
    //int measure1;
    //int measure2;
    TIMER_A0_config();

    //SET_TIMER_A0_config(203); // ferst move the engine to 90 degree


    while(i <= (scan +20) ){
    if (i<=scan+3){
        if (move_engine_hal == 4)

        {

            STOP_TIMERA0(); // stop the engine for triger and acho
            delay_of70ms(); // give delay of 70 mesc using again timer A0
            STOP_TIMERA0(); // Clr timer A0 for next iteration
            //THIS ARIR FOR DAC
            // first simple
            ADC_config_P0();
            ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
            ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

            measure1 = ADC10MEM;

            // second simple
            ADC_config_P3();
            ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
            ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

            measure2 = ADC10MEM;

            clculate_ldr(i,calibration_flag,measure1,measure2);

            TIMER_A0_config(); // config new PWM for next iteration
            SET_TIMER_A0_config(dgree); // set new dagree for move the engine

            i++;
            dgree = dgree+4;
            move_engine_hal =0;
            if(calibration_flag == 1)
                {dgree = start_dgree;}
        }
    }

            else{
                    if(calibration_flag ==1 ){
                        if(move_engine_hal == 4)
                          {
                              SET_TIMER_A0_config(start_dgree);
                              i++;
                              move_engine_hal =0;
                          }

                    }
                    else
                        if (move_engine_hal == 4)
                         {
                         SET_TIMER_A0_config(79);
                          i++;
                          move_engine_hal =0;
                         }


        }
    }

    STOP_TIMERA0();
}


void calibration(void){
    if(flag_clean == 1)
        {clear_data_to_send();
        flag_clean = 0;
        }
    scan180degreeLDR(202,10,1);
    dalta_measure = ((measure1+measure2)/2);
    data_to_send[finsh_calibartion] = (dalta_measure/4);
    finsh_calibartion++;


    if(finsh_calibartion == 10)
    {
        flag_finsh_calibartion = 1 ;
        IE2 |= UCA0TXIE;
        flag_clean = 1;
        finsh_calibartion = 0;
        }

}






void ldrAndObjekt(void)
{  clear_data_to_send();



    int dgree = 68;// set dgree to -12 dont take the 3 first simpale
    int i = 0; // i its the number of jump in the dagree

    //SET_TIMER_A0_config(203); // ferst move the engine to 90 degree


    while(i <= 80){
    if (i<=63){
        if (move_engine_hal == 4)

        {   STOP_TIMERA0(); // stop the engine for triger and acho
            delay_of70ms(); // give delay of 70 mesc using again timer A0
            STOP_TIMERA0(); // Clr timer A0 for next iteration

            ADC_config_P0();
             ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
             __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
             ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

             measure1 = ADC10MEM;

             // second simple
             ADC_config_P3();
             ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
             __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
             ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt

             measure2 = ADC10MEM;
            if(i > 2){
            clculate_ldr(i+60,0,measure1,measure2);
            dalta_measure = ((measure2 +measure1)/2);
            }
            delay_of70ms();
            STOP_TIMERA0();
            if(dalta_measure >1022){
                if(i > 2)
                {start_puls(15);// start puls of 14.3 us
                set_TimerA1_echo(); // start echo
                clculate(R1,R2,i); // clculate the dalta and save in array
                }
                }

            TIMER_A0_config(); // config new PWM for next iteration
            SET_TIMER_A0_config(dgree); // set new dagree for move the engine

            i++;
            move_engine_hal =0;
            dalta_measure = 0;
            if(i % 3==0)
                {dgree = dgree+5;}
            dgree = dgree+4;



        }
    }
    else{ // return to 0 dgree
        if(move_engine_hal == 4)
        {
            SET_TIMER_A0_config(79);
            i++;
            move_engine_hal =0;
        }
        if(move_engine_hal == 4 && i == 80)
            {STOP_TIMERA0();
                        }
    }
    }

    STOP_TIMERA0();



}


void clear_data_to_send(void)
{   int i;
    for ( i = 0; i < 123; i++) {
        data_to_send[i] = 0;
        }
}





