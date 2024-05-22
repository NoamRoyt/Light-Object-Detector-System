#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void scan180degree(void);
extern void Telemeter(void);
extern void scan180degreeLDR(int start_dgree,int scan,int calibration);
extern float linearInterpolation(int x1, int y1, int x2, int y2, int target);
extern void calibration(void);
extern void ldrAndObjekt(void);
extern void clear_data_to_send(void);







#endif
