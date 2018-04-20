/** 
* @file         CCA 
* @brief        clock caculation. 
* @details  
* @author       LMH
* @date         3/22/2018
* @version      V0.1 
* @par Copyright (c):  
*/




#define LayerMask    0xE0
#define StationMask  0x1F

unsigned int data[33]={0x00,0x21,0x27,0x42,0x63,0x64,0x85,0x86,0xA7,0xA8,0x2F,0x2B,0x32,0x30,0x4C,0x6D,0x6E,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned int upClock[40], downClock[2]={0,0};
unsigned int statnum =8;

void upDataProc()
{
	unsigned int i,j;
	unsigned int layerRecord, upClockBase,numRecord;
	for(i=1;i<=32;i++)
	{
		if((data[i]&StationMask) ==statnum)

		{
			numRecord = i;
			layerRecord = data[i]&LayerMask;     //记录当前节点所在层
			break;
		}
		else
		{

			upClockBase += data[i]/32;

			       //累加之前节点所占用节拍

		}
	}

	upClock[0] = upClockBase;//节点上传时间



	j=0;

	for(i=numRecord+1;i<=32;i++)           //从当前节点开始寻找当前节点的子节点，直到找到当前节点的同层节点为止
	{

		if((data[i]&LayerMask)>layerRecord)
		{
			upClock[j+1]= upClock[j]+data[i]/32-1;
			j++;

		}
		else
		{

			break;

		}



	}


}
void downDataProc()
{
	unsigned int i,parentLayerRecord=0,layerRecord=0,numRecord;
	unsigned int layCount[9]={0,0,0,0,0,0,0,0,0};
	for(i=1;i<=32;i++)
	{
		if(data[i]==0)break;  // 数据读完 ，退出
		if((data[i]&StationMask) ==statnum)//找到了当前站
		{
			numRecord = i;
			layerRecord = data[i]/32;     //记录当前节点所在层
			downClock[1] = layCount[layerRecord]+1;   //记录当前层该节点之前有多少个节点，同时加网关
			if(layerRecord>1)
			{
				downClock[0]  = layCount[layerRecord-1]; //如果父节点不是网关，记录父节点所在层之前有多少个节点，同时加网关
				parentLayerRecord = layerRecord-1;
			}
		}
		else
		{
			layCount[data[i]/32]++	;//相应层节点数量++

		}

	}

	for(i = 1;i<=7;i++)
	{
		if(parentLayerRecord>i)
			downClock[0] += layCount[i]; //再加上之前层的节点个数

		if(layerRecord>i)
			downClock[1] += layCount[i];

	}

	downClock[0]+=parentLayerRecord;         //加上每一层空出来的一个节拍

	if((data[numRecord]&LayerMask) >=( data[numRecord+1]&LayerMask)) // 当前节点不存在子节点
		downClock[1]=0;                  //0代表不发送
	else
		downClock[1] += layerRecord;    //加上每一层空出来的一个节拍

}


