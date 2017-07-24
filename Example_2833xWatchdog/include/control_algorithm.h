/* =================================================================================
File name:       PID_REG3.H  (IQ version)                    
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PIDREG3.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
------------------------------------------------------------------------------*/
#ifndef __PIDREG3_H__
#define __PIDREG3_H__

typedef struct {  int32  Ref;   			// Input: Reference input 
				  int32  Fdb;   			// Input: Feedback input 
				  int32  Err;				// Variable: Error
				  int32  Kp;				// Parameter: Proportional gain，Q8
				  int32  Up;				// Variable: Proportional output 
				  int32  Ui;				// Variable: Integral output 
				  int32  Ud;				// Variable: Derivative output 	
				  int32  OutPreSat; 		// Variable: Pre-saturated output
				  int32  OutMax;		    // Parameter: Maximum output 
				  int32  OutMin;	    	// Parameter: Minimum output
				  int32  Out;   			// Output: PID output 
				  int32  SatErr;			// Variable: Saturated difference
				  int32  Ki;			    // Parameter: Integral gain
				  int32  Kc;		     	// Parameter: Integral correction gain
				  int32  Kd; 		        // Parameter: Derivative gain
				  int32  Up1;		   	    // History: Previous proportional output
				  int32  IntegralMAX;       // Integral Sat
				  int32  Qvalue;            // 参数的Q值
		 	 	  void  (*calc)();	  	// Pointer to calculation function
				 } PIDREG3;	            

typedef PIDREG3 *PIDREG3_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIDREG3 object.
-----------------------------------------------------------------------------*/                     
#define PIDREG3_DEFAULTS { 0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
                           0, \
              			  (void (*)(Uint32))pid_reg3_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/
void pid_reg3_calc(PIDREG3_handle);

#endif // __PIDREG3_H__
