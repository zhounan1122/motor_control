/*=====================================================================================
 File name:        PID_REG3.C  (IQ version)                  
                    
 Originator:	Digital Control Systems Group
			Texas Instruments

 Description:  The PID controller with anti-windup                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
-------------------------------------------------------------------------------------*/

//#include "IQmathLib.h"         // Include header for IQmath library
// Don't forget to set a proper GLOBAL_Q in "IQmathLib.h" file
#include "dmctype.h"
#include "control_algorithm.h"

void pid_reg3_calc(PIDREG3 *v)
{	
    
    // Compute the error
   
    v->Err = v->Ref - v->Fdb;

    // Compute the proportional output
    v->Up = (v->Kp*v->Err)>>v->Qvalue; //(v->Kp,v->Err);

    // Compute the integral output

	v->Ui = v->Ui + ((v->Ki*v->Err)>>v->Qvalue) + ((v->Kc*v->SatErr)>>v->Qvalue);
    
    // limit integral Sat
    if(v->Ui>v->IntegralMAX)
	  v->Ui = v->IntegralMAX;
	else if (v->Ui<(0-v->IntegralMAX))
	  v->Ui = 0-v->IntegralMAX;
 

    // Compute the derivative output
    v->Ud = (v->Kd*(v->Up - v->Up1))>>8;
    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;

    // Saturate the output
    if (v->OutPreSat > v->OutMax)
      v->Out =  v->OutMax;
    else if (v->OutPreSat < v->OutMin)
      v->Out =  v->OutMin;
    else
      v->Out = v->OutPreSat;
    // Compute the saturate difference
    v->SatErr = v->OutPreSat - v->Out;

    // Update the previous proportional output
    v->Up1 = v->Up; 

}


