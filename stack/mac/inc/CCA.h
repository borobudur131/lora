/** 
* @file         CCA
* @brief        clock caculation. 
* @details  
* @author       LMH
* @date         3/22/2018
* @version      V0.1 
* @par Copyright (c):  
*/

/** 
 * the class of cca
 */  
 
 typedef struct 
 {
	 u8* ccaRoutermap;
	 u8 ccaThisLayer;
	 u8 ccaThisSation;
	 u8 ccaUpClock[40];
	 u8 ccaDownClock[2];
	 bool ccaRouterMapIsReady;
	 

 }CCA