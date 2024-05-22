#include  "../header/api.h"         // private library - API layer
#include  "../header/app.h"         // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
int l=0;




void main(void){

  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  //_BIS_SR(GIE);
  //lcd_init();//start
  //lcd_clear();


  while(1){
    switch(state){
    case state0:
        STOP_TIMERA0();
        IE2 |= UCA0RXIE;
        enterLPM(lpm_mode);
        break;
    case state1: //PB0

        scan180degree();  // Frequency Measurements
        IE2 |= UCA0TXIE;
        state = state0;
        break;
      case state2: //idle
         Telemeter();
         IE2 |= UCA0TXIE;
         state = state0;
         break;

      case state3: //PB0
          scan180degreeLDR(79,60,0);  // Frequency Measurements
          IE2 |= UCA0TXIE;
          state = state0;
      case state4:
          calibration();
          state = state0;
          break;

    case state5:
         ldrAndObjekt();
         IE2 |= UCA0TXIE;
         state = state0;
         break;









    }
  }
}







