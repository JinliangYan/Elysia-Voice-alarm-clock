//
// Created by Jinliang on 11/22/2023.
//

#include "DS1302.h"

#define DS1302_CLK_RCC		RCC_APB2Periph_GPIOB		//时钟
#define DS1302_CLK_PORT		GPIOB						//端口
#define DS1302_CLK_PIN		GPIO_Pin_13					//引脚

#define DS1302_DAT_RCC		RCC_APB2Periph_GPIOB		//时钟
#define DS1302_DAT_PORT		GPIOB						//端口
#define DS1302_DAT_PIN		GPIO_Pin_14					//引脚

#define DS1302_RST_RCC		RCC_APB2Periph_GPIOB		//时钟
#define DS1302_RST_PORT		GPIOB						//端口
#define DS1302_RST_PIN		GPIO_Pin_15					//引脚


#define DS1302_CLK_HIGH		GPIO_SetBits(DS1302_CLK_PORT, DS1302_CLK_PIN)			//引脚输出高电平
#define DS1302_CLK_LOW		GPIO_ResetBits(DS1302_CLK_PORT, DS1302_CLK_PIN)			//引脚输出低电平

#define DS1302_DAT_HIGH		GPIO_SetBits(DS1302_DAT_PORT, DS1302_DAT_PIN)			//引脚输出高电平
#define DS1302_DAT_LOW		GPIO_ResetBits(DS1302_DAT_PORT, DS1302_DAT_PIN)			//引脚输出低电平

#define DS1302_RST_HIGH		GPIO_SetBits(DS1302_RST_PORT, DS1302_RST_PIN)			//引脚输出高电平
#define DS1302_RST_LOW		GPIO_ResetBits(DS1302_RST_PORT, DS1302_RST_PIN)			//引脚输出低电平

#define DS1302_SET_IN		DS1302_SetInputMode()							//设置输入模式
#define DS1302_SET_OUT 		DS1302_SetOutputMode()						//设置输出模式


/*读相关寄存器地址*/
#define DS1302_READ_SECOND 	0X81   	//秒
#define DS1302_READ_MINUTE 	0X83	//分
#define DS1302_READ_HOUR   	0X85	//时
#define DS1302_READ_DAY		0X87	//日
#define DS1302_READ_MONTH 	0X89	//月
#define DS1302_READ_WEEK 	0X8B	//周
#define DS1302_READ_YEAR 	0X8D	//年
//#define DS1302_READ_TIME 	0XBF	//读取全部时间

/*写相关寄存器地址*/
#define DS1302_WRITE_SECOND 0X80   	//秒
#define DS1302_WRITE_MINUTE 0X82	//分
#define DS1302_WRITE_HOUR   0X84	//时
#define DS1302_WRITE_DAY	0X86	//日
#define DS1302_WRITE_MONTH 	0X88	//月
#define DS1302_WRITE_WEEK 	0X8A	//周
#define DS1302_WRITE_YEAR 	0X8C	//年
#define DS1302_WRITE_PROTECT 0X8E	//保护
//#define DS1302_WRITE_TIME 	0XBE 	//写全部时间

//日期和 时间 定义 		  年 月 日 时 分 秒 周
uint8_t DS1302_Time[8] = {23, 11, 23, 22, 2, 20, 4};

/******************************************************************
描述: DS1302 DAT设置成输入模式 函数
*******************************************************************/
void DS1302_SetInputMode(void)
{
    GPIO_InitTypeDef DS1302_Struction;				//定义结构体成员
    RCC_APB2PeriphClockCmd(DS1302_DAT_RCC, ENABLE);	//打开时钟

    DS1302_Struction.GPIO_Pin  = DS1302_DAT_PIN;		//引脚
    DS1302_Struction.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入 或者 浮空输入 GPIO_Mode_IN_FLOATING 都可以

    GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//对成员进行初始化
}


/******************************************************************
描述: DS1302 DAT设置成输出模式 函数
*******************************************************************/
void DS1302_SetOutputMode(void)
{
    GPIO_InitTypeDef DS1302_Struction;				//定义结构体成员
    RCC_APB2PeriphClockCmd(DS1302_DAT_RCC, ENABLE);	//打开时钟

    DS1302_Struction.GPIO_Pin  = DS1302_DAT_PIN;		//引脚
    DS1302_Struction.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
    DS1302_Struction.GPIO_Speed = GPIO_Speed_50MHz;		//速率

    GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//对成员进行初始化
}


/******************************************************************
描述: DS1302 配置 函数
*******************************************************************/
void DS1302_Config(void)
{
    GPIO_InitTypeDef DS1302_Struction;				//定义结构体成员

    RCC_APB2PeriphClockCmd(DS1302_CLK_RCC, ENABLE);	//打开时钟
    RCC_APB2PeriphClockCmd(DS1302_DAT_RCC, ENABLE);	//打开时钟
    RCC_APB2PeriphClockCmd(DS1302_RST_RCC, ENABLE);	//打开时钟

    //DS1302 CLK
    DS1302_Struction.GPIO_Pin  = DS1302_CLK_PIN;		//引脚
    DS1302_Struction.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
    DS1302_Struction.GPIO_Speed = GPIO_Speed_50MHz; 	//50MHz
    GPIO_Init(DS1302_CLK_PORT,&DS1302_Struction);		//对成员进行初始化
    //DS1302 DAT
    DS1302_Struction.GPIO_Pin  = DS1302_DAT_PIN;		//引脚
    DS1302_Struction.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
    DS1302_Struction.GPIO_Speed = GPIO_Speed_50MHz; 	//50MHz
    GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//对成员进行初始化
    //DS1302 RST
    DS1302_Struction.GPIO_Pin  = DS1302_RST_PIN;		//引脚
    DS1302_Struction.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
    DS1302_Struction.GPIO_Speed = GPIO_Speed_50MHz; 	//50MHz
    GPIO_Init(DS1302_RST_PORT,&DS1302_Struction);		//对成员进行初始化

}
/******************************************************************
描述: DS1302 写一字节 函数
*******************************************************************/
void DS1302_WriteByte(uint8_t addr_or_data)
{
    uint8_t i;
    DS1302_SET_OUT;
    for(i=0;i<8;i++)
    {
        if(addr_or_data & 0x01)		//从低位开始传输
        {
            DS1302_DAT_HIGH;
        }
        else
        {
            DS1302_DAT_LOW;
        }
        addr_or_data >>= 1;		//右移一位
        DS1302_CLK_HIGH;		//拉高时钟表示已经发送
        DS1302_CLK_LOW;			//拉低时钟准备继续放新的数据位
    }
}

/******************************************************************
描述: DS1302 写命令 函数
*******************************************************************/
void DS1302_WriteCmd(uint8_t addr, uint8_t dat)
{
    DS1302_RST_LOW;
    DS1302_CLK_LOW;
    DS1302_RST_HIGH;
    DS1302_WriteByte(addr);
    DS1302_WriteByte(dat);
    DS1302_RST_LOW;
}

/******************************************************************
描述: DS1302 读一字节 函数
*******************************************************************/
uint8_t DS1302_ReadByte(void)
{
    uint8_t i;
    uint8_t dat = 0;
    DS1302_SET_IN;
    for(i=0;i<8;i++)
    {
        dat >>= 1;		//左移一次 低位开始 共有效左移7次
        if( GPIO_ReadInputDataBit(DS1302_DAT_PORT, DS1302_DAT_PIN) == SET)
        {
            dat = dat | 0x80;
        }
//		else
//		{
//			dat = dat & 0x7f;
//		}
        DS1302_CLK_HIGH;
        DS1302_CLK_LOW;
    }
    return dat;
}


/******************************************************************
描述: DS1302 读数据 函数
*******************************************************************/
uint8_t DS1302_ReadData(uint8_t addr)
{
    uint8_t dat = 0;
    DS1302_RST_LOW;
    DS1302_CLK_LOW;
    DS1302_RST_HIGH;
    DS1302_WriteByte(addr);
    dat = DS1302_ReadByte();
    DS1302_RST_LOW;
    return dat;
}

/******************************************************************
描述: BCD 转 十进制 函数 例如 0X24 转成 24
*******************************************************************/
void DS1302_Bcd2Dec(uint8_t *bcd, uint8_t times)
{
    uint8_t i;
    for(i=0;i<times;i++)
    {
        bcd[i] = ((bcd[i]>>4)*10) + (bcd[i]&0x0f);
    }
}

/******************************************************************
描述: 十进制 转 BCD 函数 例如 12 转成 0X12
*******************************************************************/
void DS1302_Dec2Bcd(uint8_t *dec, uint8_t times)
{
    uint8_t i;
    for(i=0;i<times;i++)
    {
        dec[i] = ((dec[i]/10)<<4) | (dec[i]%10);
    }
}




/******************************************************************
描述: DS1302 初始化 日期和时间 函数
*******************************************************************/
void DS1302_Init(void)
{
    DS1302_Config();	//引脚配置

    DS1302_Dec2Bcd(DS1302_Time, 7);	//十进制转BCD码
//    DS1302_WriteCmd(DS1302_WRITE_PROTECT,0x00); //关闭写保护
    DS1302_WriteCmd(DS1302_WRITE_PROTECT, 0x80); //开启写保护

    DS1302_WriteCmd(DS1302_WRITE_YEAR, DS1302_Time[0]);	//年
    DS1302_WriteCmd(DS1302_WRITE_MONTH, DS1302_Time[1]);	//月
    DS1302_WriteCmd(DS1302_WRITE_DAY, DS1302_Time[2]);  	//日
    DS1302_WriteCmd(DS1302_WRITE_HOUR, DS1302_Time[3]);   	//时
    DS1302_WriteCmd(DS1302_WRITE_MINUTE, DS1302_Time[4]); 	//分
    DS1302_WriteCmd(DS1302_WRITE_SECOND, DS1302_Time[5]);  	//秒
    DS1302_WriteCmd(DS1302_WRITE_WEEK, DS1302_Time[6]);  	//周

    DS1302_WriteCmd(DS1302_WRITE_PROTECT, 0x80); //开启写保护
}

/******************************************************************
描述: DS1302 读取 日期和时间 函数
*******************************************************************/
void DS1302_Read(void)
{
    DS1302_Time[0] = DS1302_ReadData(DS1302_READ_YEAR);  //读取年
    DS1302_Time[1] = DS1302_ReadData(DS1302_READ_MONTH); //读取月
    DS1302_Time[2] = DS1302_ReadData(DS1302_READ_DAY);   //读取日
    DS1302_Time[3] = DS1302_ReadData(DS1302_READ_HOUR);  //读取时
    DS1302_Time[4] = DS1302_ReadData(DS1302_READ_MINUTE);//读取分
    DS1302_Time[5] = DS1302_ReadData(DS1302_READ_SECOND);//读取秒
    DS1302_Time[6] = DS1302_ReadData(DS1302_READ_WEEK);  //读取周

    DS1302_Bcd2Dec(DS1302_Time, 7);	//BCD码转成十进制
}


