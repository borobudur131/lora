/** 
* @file         TSY 
* @brief        time sychronization. 
* @details  
* @author       LMH
* @date         3/22/2018
* @version      V0.1 
* @par Copyright (c):  
*/

/** 
 * the class of tsy
 */  
typedef struct 
{
	u32  tsy_set_time;
	u32  tsy_clock_error;
	u32  tsy_minus_clock_correct; 
///模块时钟与需要同步的时钟源时钟之差，同步时钟源时钟落后于模块时钟，则tsy_minus_clock_correct为二者差，tsy_plus_clock_correct为0，否则plus_tsy_clock_correct为二者差，minus_tsy_clock_correct为0，
	u32  tsy_plus_clock_correct;
	bool tsy_synchroned;                           ///模块已经和主时钟同步
	u32  (*tsy_get_clock)(TSY this);               ///读当前模块同步后的时钟
	u32  (*tsy_get_system_clock)(void);			   ///获取当前操作系统时钟
	bool (*tsy_check_clock)(TSY this,u32 clock);   ///检查当前模块同步后的时钟与时钟源之差是否在合理范围内
	void (*tsy_set_clock)(TSY this,u32 clock);	   ///记录主时钟到当前模块
		
}TSY

void tsy(TSY this);