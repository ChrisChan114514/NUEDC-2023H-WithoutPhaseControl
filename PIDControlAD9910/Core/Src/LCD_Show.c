
#include "stdio.h"
#include "lcd.h"
#include "pid.h"
#include "AD9910.h"

#include "AD9833.h"
#include "main.h"
#include "AD9833_Setup.h"
#define AD9910_FREQ 25000 
#define FFT_LENGTH 4096
extern uint16_t adcBuff[FFT_LENGTH];
extern float buff3[400];
extern uint32_t f_sa ;										//基础采样频率为400K=84M/(21*10)	
extern float fft_frequency;
extern float adcVolt[FFT_LENGTH];								//adc转换后的真实电压数组
uint16_t c = 0;
float Vmin=0.0;
float Vmax=0.0;
float Vpp=0.0;
char spr_str[100];  //为sprintf服务
float wave[4096]={0}; //存储整数周期的波
int lenwave=1;
#define Tnumber 10; //提取10个周期进行示波与FFT

//PID变量

//const float KPL[]={2.2,2.1,2.1,4,4,4,4,2.5,2.5,7,4.3,4.3,4.3,4.3,4.3,7.1,7.1};
//const float KDL[]={-20,-20,-20,-5,-5,-5,-5,-5,-5,-10,-5,-5,-5,-5,-5,-5,-5};
int phase=360;
float set_peak=0;
PID phase_pid20000= 
{ 
.kp =1.97,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000 -2.8
.ki = 0, //可以让相位差动起来ki
.kd = 5,
.error_acc = 0,
.lastError = 0

};

PID phase_pid25000= 
{ 
.kp = 1.97,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd = 5,
.error_acc = 0,
.lastError = 0

};
PID phase_pid30000= 
{ 
.kp = 1.6,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd = 5,
.error_acc = 0,
.lastError = 0

};
PID phase_pid35000= 
{ 
.kp = 5,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};
PID phase_pid40000= 
{ 
.kp = 4,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};
PID phase_pid45000= 
{ 
.kp = 4,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};


PID phase_pid50000= 
{ 
.kp = 4,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};

PID phase_pid55000= 
{ 
.kp = 3,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};
PID phase_pid60000= 
{ 
.kp = 3,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};


PID phase_pid65000= 
{ 
.kp = 5,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};


PID phase_pid70000= 
{ 
.kp = 5,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};
PID phase_pid75000= 
{ 
.kp = 5,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  10,
.error_acc = 0,
.lastError = 0

};
PID phase_pid80000= 
{ 
.kp = 5,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd = 10,
.error_acc = 0,
.lastError = 0

};
PID phase_pid85000= 
{ 
.kp = 4.3,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  -5,
.error_acc = 0,
.lastError = 0

};
PID phase_pid90000= 
{ 
.kp = 4.3,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  -5,
.error_acc = 0,
.lastError = 0

};

PID phase_pid95000= 
{ 
.kp =6,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  -5,
.error_acc = 0,
.lastError = 0

};
PID phase_pid100000= 
{ 
.kp = 6,  //和相位差大小相关 一定范围内 (AD9910_FREQ-20000)/5000
.ki = 0, //可以让相位差动起来ki
.kd =  -5,
.error_acc = 0,
.lastError = 0

};
/////////////////////

void clear_point(u16 num)//更新显示屏当前列
{
	u16 index_clear_lie = 0; 
	g_point_color = DARKBLUE ;
	for(index_clear_lie = 1;index_clear_lie < 320;index_clear_lie++)
	{		
		lcd_draw_point(num,index_clear_lie,g_point_color);
	}
	if(!(num%40))//判断hang是否为40的倍数 画列点
	{
		for(index_clear_lie = 10;index_clear_lie < 320;index_clear_lie += 10)
		{		
			lcd_draw_point(num ,index_clear_lie,WHITE );
		}
	}
	if(!(num%10))//判断hang是否为10的倍数 画行点
	{
		for(index_clear_lie = 40;index_clear_lie <320;index_clear_lie += 40)
		{		
			lcd_draw_point(num ,index_clear_lie,WHITE );
		}
	}	
	g_point_color = YELLOW;	
}

//void DrawOscillogram(float *buff)//画波形图
//{
//	static u16 Ypos1 = 0,Ypos2 = 0;
////	u16 Yinit=100;
//	u16 i = 0;
//	g_point_color = YELLOW;
//	if(c==0)    //0：实时更新  1：显示存储的波形
//	{
//		for(i = 1;i < 400;i++)
//		{
//			clear_point(i );	
//			Ypos2 = 320-(buff[i] * 320/ 4096);//转换坐标
//			//Ypos2 = Ypos2 * bei;
//			if(Ypos2 >400)
//				Ypos2 =400; //超出范围不显示
//			lcd_draw_line(i ,Ypos1 , i+1 ,Ypos2,g_point_color);
//			Ypos1 = Ypos2 ;
//		}
//	}
//	else
//	{
//		for(i = 1;i < 400;i++)
//		{
//			clear_point(i );	
//			Ypos2 = 320-(buff3[i] * 320/ 4096);//转换坐标
//			//Ypos2 = Ypos2 * bei;
//			if(Ypos2 >400)
//				Ypos2 =400; //超出范围不显示
//			lcd_draw_line(i ,Ypos1 , i+1 ,Ypos2,g_point_color);
//			Ypos1 = Ypos2 ;
//		}	
//	}
//    Ypos1 = 0;	
//}

void ADCvolt_ANA(void) //分析adcvolt数组
{

//	phase_pid1.ki=2.2; //(AD9910_FREQ-20000)/5000
//	phase_pid1.kd=-20;
	//先提取Vpp Vmin Vmax 基本量
	int i;
	int deta; //pid结果量
	//频率输入逻辑
	int Freq1=findex1*5000+20000;
	int Freq2=findex2*5000+20000;
	int FreqMain=0; //AD9910单通道输出频率
	if (1) //由按键选择
	{
		FreqMain=Freq1;
	}
	else
	{
		FreqMain=Freq2;
	}
	
	//FreqMain=100000;
	Vmin=adcVolt[0];
	Vmax=adcVolt[0];
	for (i=0;i<FFT_LENGTH;i++)
	{
		if (Vmin>adcVolt[i]) 
		{
			Vmin=adcVolt[i];
		}
		if (Vmax<adcVolt[i])
		{
			Vmax=adcVolt[i];
		}
	}
	Vpp=Vmax-Vmin;

	
	//通过Vpp锁相逻辑 2023校赛C题

//	lcd_show_string(0,160,80,16,16,"             ",YELLOW);
//	lcd_show_string(0,160,80,16,16,spr_str,YELLOW);
	
	//将Vpp作为ERR进行PID运算
	if (FreqMain < 21000)
	{
		deta = Follow_PID(&phase_pid20000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 26000)
	{
		deta = Follow_PID(&phase_pid25000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 31000)
	{
		deta = Follow_PID(&phase_pid30000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 36000)
	{
		deta = Follow_PID(&phase_pid35000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 41000)
	{
		deta = Follow_PID(&phase_pid40000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 46000)
	{
		deta = Follow_PID(&phase_pid45000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 51000)
	{
		deta = Follow_PID(&phase_pid50000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 56000)
	{
		deta = Follow_PID(&phase_pid55000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 61000)
	{
		deta = Follow_PID(&phase_pid60000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 66000)
	{
		deta = Follow_PID(&phase_pid65000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 71000)
	{
		deta = Follow_PID(&phase_pid70000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 76000)
	{
		deta = Follow_PID(&phase_pid75000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 81000)
	{
		deta = Follow_PID(&phase_pid80000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 86000)
	{
		deta = Follow_PID(&phase_pid85000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 91000)
	{
		deta = Follow_PID(&phase_pid90000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 96000)
	{
		deta = Follow_PID(&phase_pid95000, set_peak - Vpp, 1);
	}
	else if (FreqMain < 101000)
	{
		deta = Follow_PID(&phase_pid100000, set_peak - Vpp, 1);
	}
	phase=phase-deta;
	
	while (phase >= 360)
	phase-= 360;
	while (phase< 0)
	phase += 360;
	

	AD9910_Singal_Profile_Set(0, FreqMain, 0x3FFF, phase); 
	
//	AD9833_Typedef AD9833_1;
//	AD9833_1.PORT = GPIOC;
//    AD9833_1.DATA = GPIO_PIN_0;
//    AD9833_1.SCK = GPIO_PIN_1;
//    AD9833_1.SS = GPIO_PIN_2;
//	
//	AD9833_SetWaveData(1000,phase , AD9833_1); //FRE\PHASE\Value
//	
//	lcd_show_string(100,140,80,16,16,"Phase:",YELLOW);
//	sprintf(spr_str,"%d",phase);
//	lcd_show_string(100,160,80,16,16,"             ",YELLOW);
//	lcd_show_string(100,160,80,16,16,spr_str,YELLOW);
	
}

void DrawOscillogram(void)//画波形图 整数个周期
{
	static u16 Ypos1 = 0,Ypos2 = 0;
	int chazhi=1;
	int i;
	int j=1;
	float wave2[4096];
	float pos0=0;
	if (lenwave==1)
	{
		lenwave=1;
	}
	if (400/lenwave<=1)
	{
		lenwave=400;
	}
	//进行插值 将Tnumber拓展至400
	for (i=0;i<lenwave;i++)
	{
		chazhi=400/lenwave;
		if (i==0) //在0处加入余项
		{
			for (j=0;j<chazhi+(400-chazhi*lenwave);j++)
			{
				wave2[j]=wave[0]+(wave[1]-wave[0])/(chazhi+(400-chazhi*lenwave))*j; //插值公式
			}
			pos0=chazhi+(400-chazhi*lenwave);
		}
		else
		{
			for (j=pos0+(i-1)*chazhi;j<chazhi*i+pos0;j++)
			{
				wave2[j]=wave[i]+(wave[(i+1)%lenwave]-wave[i])/(chazhi)*(j-pos0-(i-1)*chazhi);
			}
		}
	}
	
	//理论上插值完成(已检验，但是是已Npoint+1为周期)
	for (i = 1;i < 400;i++)
	{
		clear_point(i );
		Ypos2 = 320-((wave2[i]*4096/3.3)* 320/ 4096); 
	
		if(Ypos2 >400)
		Ypos2 =400; //超出范围不显示
		lcd_draw_line(i ,Ypos1 , i+1 ,Ypos2,WHITE);
		Ypos1 = Ypos2 ;
		
	}
}


void Set_BackGround(void)
{
	g_point_color = YELLOW;
  lcd_clear(DARKBLUE);
	lcd_draw_rectangle(0,0,400,320,g_point_color);//矩形
	//LCD_DrawLine(0,220,700,220);//横线
	//LCD_DrawLine(350,20,350,420);//竖线
	//POINT_COLOR = WHITE;
	//BACK_COLOR = DARKBLUE;
	//LCD_ShowString(330,425,210,24,24,(u8*)"vpp=");	
}

void Lcd_DrawNetwork(void)
{
	u16 index_y = 0;
	u16 index_x = 0;	
	
    //画列点	
	for(index_x = 40;index_x < 400;index_x += 40)
	{
		for(index_y = 10;index_y < 320;index_y += 10)
		{
			lcd_draw_point(index_x,index_y,WHITE);	
		}
	}
	//画行点
	for(index_y = 40;index_y < 320;index_y += 40)
	{
		for(index_x = 10;index_x < 400;index_x += 10)
		{
			lcd_draw_point(index_x,index_y,WHITE);	
		}
	}
}
void Draw_Prompt(void)
{
	g_back_color = DARKBLUE;
	g_point_color=WHITE; 
	
	
	

//	lcd_show_string(405,170,80,16,16,"Memory:",WHITE);

	
}
