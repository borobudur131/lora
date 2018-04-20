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
* 读当前模块同步后的时钟.
* @TSY this   时钟同步对象. 
* @retval  	  u32时钟
* @par   	  3/22/2018
*/
u32 get_clock(TSY this)
{
	
	return  this->tsy_plus_clock_correct+ system_clock-tsy_minus_clock_correct;	
	
}

/** 
* 获取当前操作系统时钟
* @retval  	  u32时钟
* @par   	  3/22/2018
*/
u32 get_system_clock(void)
{
	u32 i;
	return i;	
	
}

/** 
* 检查模块同步后的时钟与主时钟之间的误差是否在5ms以内
* @TSY this   时钟同步对象. 
* @u32 clock  主时钟. 
* @retval  	  true or false
* @par   	  3/22/2018
*/

bool check_clock(TSY this,u32 clock)
{
	u32 system_clock = this.get_system_clock();
	
	if (clock>=system_clock)
	{
		
		if (clock-system_clock<=tsy_plus_clock_correct+tsy_minus_clock_correct+5&&clock-system_clock+5>=tsy_plus_clock_correct+tsy_minus_clock_correct)
			return true
		
		else
		{
			this.tsy_clock_error = system_clock - this.tsy_set_time; 
			return false
		}
		
	}

	if (clock<system_clock)
	{
		
	{
		
		if (system_clock-clock<=tsy_plus_clock_correct+tsy_minus_clock_correct+5&&system_clock-clock+5>=tsy_plus_clock_correct+tsy_minus_clock_correct)
			return true
		
		else
		{
			this.tsy_clock_error = system_clock - this.tsy_set_time; 
			return false
		}
	
		
	}
		
	
}

/** 
* 设置同步时差
* @TSY this   时钟同步对象. 
* @u32 clock  主时钟. 
* @par   	  3/22/2018
*/

void set_clock(TSY this,u32 clock)
{
	u32 system_clock = this.get_system_clock();
	
	if (clock>=system_clock)
	{
		
		tsy_plus_clock_correct = clock - system_clock;
		tsy_minus_clock_correct = 0;

	}

	else
	{
		
		tsy_tsy_clock_correct = 0;
		tsy_minus_clock_correct = system_clock - clock;
	}
	this.tsy_set_time = system_clock;	
	this.tsy_minus_clock_correct = TURE;
	
}
/** 
* TSY的构造函数
* @TSY this   时钟同步对象. 
* @par   	  3/22/2018
*/
void tsy(TSY this)
{
	this.tsy_clock_error = 0;
	this.tsy_minus_clock_correct = 0;
	this.tsy_plus_clock_correct	= 0;
	this.tsy_synchroned = FALSE;                          ///模块已经和主时钟同步
	this.tsy_get_clock = get_clock;
	this.tsy_get_system_clock = et_system_clock;
	this.tsy_check_clock= check_clock;
	this.tsy_set_clock= set_clock;
		
	
}