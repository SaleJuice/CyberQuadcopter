//
// sj_key.c
//

#include "sj_key.h"

#define INTERVAL_TIMES     ((uint16_t)(KEY_INTERVAL_MS / (1000 / 1000.0f)))
const int16_t INTERVALTIMES = INTERVAL_TIMES;

typedef struct
{
    uint16_t  timeAfterLast;      // 本次按键到上次按键的时间
    uint16_t  timeNeedBeforeNext; // 到下次触发按键需要的时间
    uint16_t  changeNum;          // 增减按键每次改变的大小
} key_t;

static key_t key = {
    .timeAfterLast = 0,
    .timeNeedBeforeNext = 0,
    .changeNum = 0
};

keyMode_t keyMode = _KEY_NONE_;

//按键初始化函数
void KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁用JTAG
	
	RCC_APB2PeriphClockCmd(KEY_GPIO_RCC, ENABLE);			//使能时钟,stm32配置外设时必须先使能外设时钟	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//设置成上拉输入
	GPIO_InitStructure.GPIO_Pin  = KEY1_GPIO_PIN;
 	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = KEY3_GPIO_PIN;
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = KEY4_GPIO_PIN;
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = KEY5_GPIO_PIN;
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = KEY6_GPIO_PIN;
	GPIO_Init(KEY6_GPIO_PORT, &GPIO_InitStructure);			//初始化GPIO
}

////简单延时
//static void KeyDelay(unsigned int ms)
//{
//	unsigned int a;
//    while(ms--)
//    {
//        a = 6675;
//        while(a--);
//    }
//}

/*
* @func 扫描单个按键
*/
uint8_t ScanfKey(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
	{
		if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
		{
			if (key.timeAfterLast >= key.timeNeedBeforeNext)
			{
        key.timeAfterLast = 0;
				return KEY_ON;
			}
			else
			{
				return KEY_CONTINUOUS; 
			}
		}
	}
	return KEY_OFF;
}

/*
* @func 扫描所有按键
*/
keyMode_t KeyScanfAll(void)
{
	//uint8_t key_states;
	
	if 		(ScanfKey(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)					return _KEY_ADD_PAGE_;
	else if (ScanfKey(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)					return _KEY_START_;
	/*else if (ScanfKey(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == KEY_ON)					return _KEY_DOWN_LINE_;
	else if ((key_states = ScanfKey(KEY4_GPIO_PORT, KEY4_GPIO_PIN)) == KEY_ON)	return _KEY_POOR_NUM_;
	else if (key_states == KEY_CONTINUOUS) 										return _KEY_CONTINUOUS_;
	else if ((key_states = ScanfKey(KEY5_GPIO_PORT, KEY5_GPIO_PIN)) == KEY_ON)	return _KEY_ADD_NUM_;
	else if (key_states == KEY_CONTINUOUS) 										return _KEY_CONTINUOUS_;
	else if (ScanfKey(KEY6_GPIO_PORT, KEY6_GPIO_PIN) == KEY_ON)					return _KEY_UP_LINE_;*/
	return _KEY_NONE_;
}

/*
* @func 计算和上次按键间隔时间
*/
void Key_AddTimeAfterLast(void)
{
    if (key.timeAfterLast < key.timeNeedBeforeNext)
    {
        key.timeAfterLast++;
    }
}

/*
* @func 用于控制按键触发的时间
*/
static void KeyChooseSpeed(void)
{
    key.timeNeedBeforeNext = INTERVALTIMES - key.changeNum;
}

/*
* @func 此函数的返回值会随着按键时间而增加
*/
static int KeySpeedUp(void)
{
	// （INTERVALTIMES无法被整除时可能会出现最后飞速加减的情况）
    static uint16_t delt = INTERVALTIMES / 4; // 修改这个系数，可以加快增减速度
    
    if (key.changeNum < INTERVALTIMES - delt) {
        key.changeNum += delt;
    }
    return key.changeNum / delt;
}

/*
* @func 扫描按键并作出操作
*/
keyMode_t Key_Scanf(void)
{
	KeyChooseSpeed();           // 改变按键需要的间隔时间
	keyMode = KeyScanfAll();
    return keyMode;
}

/*
* @func 调整参数按键函数
*/
int DebugChar(char *num, char deltNum) 
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}

int DebugUnChar(unsigned char *num, unsigned char deltNum) 
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}

int DebugInt16(int16_t *num, int16_t deltNum) 
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}

int DebugInt(int *num, int deltNum) 
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}

int DebugUnInt(unsigned int *num, unsigned int deltNum)
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}

int DebugDouble(double *num, double deltNum)
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}

int DebugFloat(float* num, float deltNum)
{
    if(keyMode==_KEY_ADD_NUM_)    {*num+=deltNum*KeySpeedUp(); return 1;}
    if(keyMode==_KEY_POOR_NUM_)   {*num-=deltNum*KeySpeedUp(); return 1;}
    return 0;
}
