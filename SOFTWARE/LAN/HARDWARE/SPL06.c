#include "spl06.h"
#include "math.h"
#include "ioi2c.h"

float Initial_Altitude;


float _kT=0,_kP=0;
int16_t _C0=0,_C1=0,_C01=0,_C11=0,_C20=0,_C21=0,_C30=0;
int32_t _C00=0,_C10=0;

u8 spl06_write_reg(u8 reg_addr,u8 reg_val)
{

	IICwriteByte(SP06_Advice_Address,reg_addr,reg_val);
	
	return 0;
	
}

u8 spl06_read_reg(u8 reg_addr)
{

	
	unsigned char temp;

  temp =I2C_ReadOneByte(SP06_Advice_Address,reg_addr);

  return temp;

}
void spl06_read_buffer(u8 reg_addr,u8 *buffer,uint16_t len)
{

	
	IICreadBytes(SP06_Advice_Address,reg_addr,len,buffer);
	
	
}


void spl06_start(u8 mode)
{
    spl06_write_reg(SP06_MEAS_CFG, mode);//测量模式配置
}

void spl06_config_temperature(u8 rate,u8 oversampling)
{
    switch(oversampling)
    {
	case TMP_PRC_1:
		_kT = 524288;
		break;
	case TMP_PRC_2:
		_kT = 1572864;
		break;
	case TMP_PRC_4:
		_kT = 3670016;
		break;
	case TMP_PRC_8:
		_kT = 7864320;
		break;
	case TMP_PRC_16:
		_kT = 253952;
		break;
	case TMP_PRC_32:
		_kT = 516096;
		break;
	case TMP_PRC_64:
		_kT = 1040384;
		break;
	case TMP_PRC_128:
		_kT = 2088960;
		break;
    }
    spl06_write_reg(SP06_TMP_CFG,rate|oversampling|0x80);   //温度每秒128次测量一次（最快速度）
    if(oversampling > TMP_PRC_8)
    {
        u8 temp = spl06_read_reg(SP06_CFG_REG);
        spl06_write_reg(SP06_CFG_REG,temp|SPL06_CFG_T_SHIFT);
    }
}

void spl06_config_pressure(u8 rate,u8 oversampling)//设置补偿系数及采样速率
{
    switch(oversampling)
    {
	case PM_PRC_1:
		_kP = 524288;
		break;
	case PM_PRC_2:
		_kP = 1572864;
		break;
	case PM_PRC_4:
		_kP = 3670016;
		break;
	case PM_PRC_8:
		_kP = 7864320;
		break;
	case PM_PRC_16:
		_kP = 253952;
		break;
	case PM_PRC_32:
		_kP = 516096;
		break;
	case PM_PRC_64:
		_kP = 1040384;
		break;
	case PM_PRC_128:
		_kP = 2088960;
		break;
    }
	
    spl06_write_reg(SP06_PSR_CFG,rate|oversampling);
    if(oversampling > PM_PRC_8)
    {
        u8 temp = spl06_read_reg(SP06_CFG_REG);
        spl06_write_reg(SP06_CFG_REG,temp|SPL06_CFG_P_SHIFT);
    }
}

int32_t spl06_get_pressure_adc(void)//获取压力ADC值
{
    u8 buf[3];
    int32_t adc;
	
    spl06_read_buffer(SP06_PSR_B2,buf,3);
    adc = (int32_t)buf[0]<<16 | (int32_t)buf[1]<<8 | (int32_t)buf[2];
    adc = (adc&0x800000)?(0xFF000000|adc):adc;
	
    return adc;
}

int32_t spl06_get_temperature_adc(void)//获取温度ADC值
{
    u8 buf[3];
    int32_t adc;
	
    spl06_read_buffer(SP06_TMP_B2,buf,3);
    adc = (int32_t)buf[0]<<16 | (int32_t)buf[1]<<8 | (int32_t)buf[2];
    adc = (adc&0x800000)?(0xFF000000|adc):adc;
    return adc;
}
u8 spl06_update(float *Temp,float *Press)//获取并计算出温度值、气压值
{
	float Praw_src=0,Traw_src=0;
    float qua2=0, qua3=0;
    Traw_src = spl06_get_temperature_adc()/_kT;
    Praw_src = spl06_get_pressure_adc()/_kP ;
	//计算温度
    *Temp = 0.5f*_C0 + Traw_src * _C1;
    //计算气压
    qua2 = _C10 + Praw_src * (_C20 + Praw_src* _C30);
    qua3 = Traw_src * Praw_src * (_C11 + Praw_src * _C21);
    *Press = _C00 + Praw_src * qua2 + Traw_src * _C01 + qua3;
    return 0;
}

u8 spl06_init(void)
{
    u8 coef[18];
    u8 id;
    if(spl06_write_reg(SP06_RESET,0x89))
    {
			
		return 1;
    }
		delay_ms(40);
    id = spl06_read_reg(SP06_ID);
		
		
    if(id != 0x10)
    {
			
		  return 2;
    }
		

    delay_ms(100);        //复位后系数准备好需要至少40ms
	
    spl06_read_buffer(SP06_COEF,coef,18);//读取相关数据
    _C0 = ((int16_t)coef[0]<<4 ) | ((coef[1]&0xF0)>>4);
    _C0 = (_C0&0x0800)?(0xF000|_C0):_C0;
    _C1 = ((int16_t)(coef[1]&0x0F)<<8 ) | coef[2];
    _C1 = (_C1&0x0800)?(0xF000|_C1):_C1;
    _C00 = (int32_t)coef[3]<<12  | (int32_t)coef[4]<<4  | (int32_t)coef[5]>>4;
    _C10 = (int32_t)(coef[5]&0x0F)<<16  | (int32_t)coef[6]<<8  | (int32_t)coef[7];
    _C00 = (_C00&0x080000)?(0xFFF00000|_C00):_C00;
	_C10 = (_C10&0x080000)?(0xFFF00000|_C10):_C10;
	
	_C01 = ((int16_t)coef[8]<<8 ) | coef[9];
	_C01 = (_C01&0x0800)?(0xF000|_C01):_C01;
	
    _C11 = ((int16_t)coef[10]<<8 ) | coef[11];
    _C11 = (_C11&0x0800)?(0xF000|_C11):_C11;
	
    _C20 = ((int16_t)coef[12]<<8 ) | coef[13];
    _C20 = (_C20&0x0800)?(0xF000|_C20):_C20;
    _C21 = ((int16_t)coef[14]<<8 ) | coef[15];
    _C21 = (_C21&0x0800)?(0xF000|_C21):_C21;
    _C30 = ((int16_t)coef[16]<<8 ) | coef[17];
    _C30 = (_C30&0x0800)?(0xF000|_C30):_C30;
	
    spl06_config_pressure(PM_RATE_128,PM_PRC_64);
    spl06_config_temperature(PM_RATE_8,TMP_PRC_8);
	
    spl06_start(MEAS_CTRL_ContinuousPressTemp); //启动连续的气压温度测量
    delay_ms(20);
    return 0;
}
float Caculate_Altitude(float GasPress)
{
	float Altitude=0;
	Altitude =(44330.0 *(1.0-pow((float)(GasPress) / 101325.0,1.0/5.255)));
	return Altitude;
}

float Get_Altitude()
{
static float pa =0;

static float hight =0;

float temp =0;	
	
	 spl06_update(&temp,&pa);
	 hight = Caculate_Altitude(pa);		
//	 printf("pa:%.2f   gao:%.2fm\r\n",pa,hight);	
	return hight;
}

float Get_Absolute_Altitude()
{
return Get_Altitude()-Initial_Altitude;
}


void Init_Altitude(void)
{
	static int count=0;
	static int cnt_p = 0;
	static float PressNUM = 0;
	
while(1)
{
Get_Altitude();
	count++;
	if(count>=100) break;
}
	
while (Get_Altitude()!=0)
{
		if(cnt_p == 0)
		{
			cnt_p = 1;
			PressNUM = 0;
		}
		PressNUM += Get_Altitude(); //100个气压数据累加
		if(cnt_p == 100)
		{
		  Initial_Altitude= (PressNUM/cnt_p)-2.5; //求平均值减去实测螺旋桨影响
			cnt_p = 0; 
			PressNUM =0;
			break;
		}
		cnt_p++;
}
}

