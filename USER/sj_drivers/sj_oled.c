//
// sj_oled.c
//

#include "sj_oled.h"
 
const unsigned char F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 91
    { 0x00, 0x02, 0x04 ,0x08, 0x10, 0x20 },   // \92
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

//中文字符串原字
const unsigned char F14x16_Idx[] =
{
    "赛博"
};

//中文字符串编码
const unsigned char F14x16[] =
{
    0x0C,0x04,0x54,0x54,0xFC,0x54,0x56,0x54,0xFC,0x54,0x54,0x04,0x0C,0x00,
    0x09,0x49,0x5D,0x47,0x25,0x25,0x15,0x25,0x25,0x47,0x5D,0x09,0x09,0x00,//赛
    0x20,0x20,0xFE,0x20,0x20,0xF4,0x54,0x54,0xFE,0x54,0x56,0xF4,0x04,0x00,
    0x00,0x00,0x7F,0x00,0x08,0x0B,0x19,0x29,0x0B,0x49,0x7D,0x0B,0x08,0x00,//博
};

//------------------------------------------------------
//      对全屏写入同一个字符函数
//      如 LED_Fill(0x01);  对于某一位0为不亮 1为亮
//-------------------------------------------------------
//      共128列
//
//      1               1                1
//      0               0                0
//      0               0                0
//      0     ......    0     ......     0
//      0               0                0
//      0               0                0
//      0               0                0
//      0               0                0
//
//      1               1
//      0               0
//      .               .
//      .               .
//      .               .
//      .               .
//      0               0
//------------------------------------------------------

//OLEDIO口初始化函数
void OLED_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(OLED_GPIO_RCC, ENABLE); //使能OLED的GPIO端口时钟
    GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
    GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure); //IO口初始化为高电平
    
    GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
    GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = RES_GPIO_PIN;
    GPIO_Init(RES_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = DC_GPIO_PIN;
    GPIO_Init(DC_GPIO_PORT, &GPIO_InitStructure); 
}

void OLED_WrDat(unsigned char data)   // 写数据函数
{
    char i;

    OLED_DCH;        //设置指定数据端口位
    for(i=0;i<8;i++)
    {
        OLED_SCLL;
        if(data&0x80)    OLED_SDAH;
        else            OLED_SDAL;
        OLED_SCLH;
        data<<=1;   
    }
    OLED_DCH; 
}

void OLED_WrCmd(unsigned char cmd) //写命令函数
{
    char i;

    OLED_DCL;        //清除指定的数据端口位
    for(i=0;i<8;i++)
    {
        OLED_SCLL;
        if(cmd&0x80)    OLED_SDAH;
        else            OLED_SDAL;
        OLED_SCLH;
        cmd<<=1;
    }
    OLED_DCH;
}

/*
* @func 全部填满
*/
void OLED_Fill(unsigned char bmp_data)
{
    unsigned char y,x;

    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);   // 0xb0+0~7 表示0～7页
        OLED_WrCmd(0x00);     // 0x00+0~16表示将128列分为16组其他地址在某组中的第几列
        OLED_WrCmd(0x10);     // 0x10+0~16表示将128列分为16组其地址所在第几组
        for(x=0;x<128;x++)
            OLED_WrDat(bmp_data);
    }
}

void OLED_DLY_ms(unsigned int ms)
{
    unsigned int a;
    while(ms--)
    {
        a = 6675;
        while(a--);
    }
}

void SetStartColumn(unsigned char d)
{
    OLED_WrCmd(0x00+d%16);  // Set Lower Column Start Address for Page Addressing Mode
                            // Default => 0x00
    OLED_WrCmd(0x10+d/16);  // Set Higher Column Start Address for Page Addressing Mode
                            // Default => 0x10
}

void SetAddressingMode(unsigned char d)
{
    OLED_WrCmd(0x20);       // Set Memory Addressing Mode
    OLED_WrCmd(d);          // Default => 0x02
                            // 0x00 => Horizontal Addressing Mode
                            // 0x01 => Vertical Addressing Mode
                            // 0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
    OLED_WrCmd(0x21);        // Set Column Address
    OLED_WrCmd(a);           // Default => 0x00 (Column Start Address)
    OLED_WrCmd(b);           // Default => 0x7F (Column End Address)
}

void SetPageAddress(unsigned char a, unsigned char b)
{
    OLED_WrCmd(0x22);        // Set Page Address
    OLED_WrCmd(a);           // Default => 0x00 (Page Start Address)
    OLED_WrCmd(b);           // Default => 0x07 (Page End Address)
}

void SetStartLine(unsigned char d)
{
    OLED_WrCmd(0x40|d);     // Set Display Start Line
                            // Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char d)
{
    OLED_WrCmd(0x81);       // Set Contrast Control
    OLED_WrCmd(d);          // Default => 0x7F
}

void Set_Charge_Pump(unsigned char d)
{
    OLED_WrCmd(0x8D);       // Set Charge Pump
    OLED_WrCmd(0x10|d);     // Default => 0x10
                            // 0x10 (0x00) => Disable Charge Pump
                            // 0x14 (0x04) => Enable Charge Pump
}

void Set_Segment_Remap(unsigned char d)
{
    OLED_WrCmd(0xA0|d);     // Set Segment Re-Map
                            // Default => 0xA0
                            // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
                            // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void Set_Entire_Display(unsigned char d)
{
    OLED_WrCmd(0xA4|d);     // Set Entire Display On / Off
                            // Default => 0xA4
                            // 0xA4 (0x00) => Normal Display
                            // 0xA5 (0x01) => Entire Display On
}

void Set_Inverse_Display(unsigned char d)
{
    OLED_WrCmd(0xA6|d);     // Set Inverse Display On/Off
                            // Default => 0xA6
                            // 0xA6 (0x00) => Normal Display
                            // 0xA7 (0x01) => Inverse Display On
}

void Set_Multiplex_Ratio(unsigned char d)
{
    OLED_WrCmd(0xA8);       // Set Multiplex Ratio
    OLED_WrCmd(d);          // Default => 0x3F (1/64 Duty)
}

void Set_Display_On_Off(unsigned char d)
{
    OLED_WrCmd(0xAE|d);     // Set Display On/Off
                            // Default => 0xAE
                            // 0xAE (0x00) => Display Off
                            // 0xAF (0x01) => Display On
}

void SetStartPage(unsigned char d)
{
    OLED_WrCmd(0xB0|d);     // Set Page Start Address for Page Addressing Mode
                            // Default => 0xB0 (0x00)
}

void Set_Common_Remap(unsigned char d)
{
    OLED_WrCmd(0xC0|d);     // Set COM Output Scan Direction
                            // Default => 0xC0
                            // 0xC0 (0x00) => Scan from COM0 to 63
                            // 0xC8 (0x08) => Scan from COM63 to 0
}

void Set_Display_Offset(unsigned char d)
{
    OLED_WrCmd(0xD3);       // Set Display Offset
    OLED_WrCmd(d);          // Default => 0x00
}

void Set_Display_Clock(unsigned char d)
{
    OLED_WrCmd(0xD5);       // Set Display Clock Divide Ratio / Oscillator Frequency
    OLED_WrCmd(d);          // Default => 0x80
                            // D[3:0] => Display Clock Divider
                            // D[7:4] => Oscillator Frequency
}

void Set_Precharge_Period(unsigned char d)
{
    OLED_WrCmd(0xD9);       // Set Pre-Charge Period
    OLED_WrCmd(d);          // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                            // D[3:0] => Phase 1 Period in 1~15 Display Clocks
                            // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void Set_Common_Config(unsigned char d)
{
    OLED_WrCmd(0xDA);       // Set COM Pins Hardware Configuration
    OLED_WrCmd(0x02|d);     // Default => 0x12 (0x10)
                            // Alternative COM Pin Configuration
                            // Disable COM Left/Right Re-Map
}

void Set_VCOMH(unsigned char d)
{
    OLED_WrCmd(0xDB);       // Set VCOMH Deselect Level
    OLED_WrCmd(d);          // Default => 0x20 (0.77*VCC)
}

void Set_NOP(void)
{
    OLED_WrCmd(0xE3);       // Command for No Operation
}

/*
* @func 设置坐标函数
* @para y 行数0～7
* @para x 列数0～122
*/
void OLED_Set_Pos(unsigned char y, unsigned char x)
{
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(((x&0xf0)>>4)|0x10);
    OLED_WrCmd((x&0x0f)|0x00);
}

/*
* @func OLED初始化
*/
void OLED_Config(void)
{
    OLED_Pin_Init();
    OLED_SCLH;;;
    OLED_RSTL;;;
    OLED_DLY_ms(50);
    OLED_RSTH;;;

    Set_Display_On_Off(0x00);           // Display Off (0x00/0x01)
    Set_Display_Clock(0x80);            // Set Clock as 100 Frames/Sec
    Set_Multiplex_Ratio(0x3F);          // 1/64 Duty (0x0F~0x3F)
    Set_Display_Offset(0x00);           // Shift Mapping RAM Counter (0x00~0x3F)
    SetStartLine(0x00);                 // Set Mapping RAM Display Start Line (0x00~0x3F)
    Set_Charge_Pump(0x04);              // Enable Embedded DC/DC Converter (0x00/0x04)
    SetAddressingMode(0x02);            // Set Page Addressing Mode (0x00/0x01/0x02)
    Set_Segment_Remap(0x01);            // Set SEG/Column Mapping     0x00◊Û”“∑¥÷√ 0x01’˝≥£
    Set_Common_Remap(0x08);             // Set COM/Row Scan Direction 0x00…œœ¬∑¥÷√ 0x08’˝≥£
    Set_Common_Config(0x10);            // Set Sequential Configuration (0x00/0x10)
    SetContrastControl(0xCF);           // Set SEG Output Current
    Set_Precharge_Period(0xF1);         // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    Set_VCOMH(0x40);                    // Set VCOM Deselect Level
    Set_Entire_Display(0x00);           // Disable Entire Display On (0x00/0x01)
    Set_Inverse_Display(0x00);          // Disable Inverse Display On (0x00/0x01)
    Set_Display_On_Off(0x01);           // Display On (0x00/0x01)
    OLED_Fill(0x00);                    // ≥ı º«Â∆¡
    OLED_Set_Pos(0,0);
}

/*
* @func 清屏函数
*/
void OLED_Cls(void)
{
    unsigned char y, x;
    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x00);
        OLED_WrCmd(0x10);
        for(x=0;x<128;x++)
            OLED_WrDat(0);
    }
}

/*
* @func 计算位数函数
*/
static unsigned int digits(int num)
{
    unsigned int n = 0;
    
    if (num == 0)   return 1;
    if (num < 0)    num = -num;
    while (num > 0) {
        num = num/10;
        n++;
    }
    return n;
}

/*
* @func 计算m的n次方
*/
static int my_power(int m, int n)
{
    int result = 1;     
    while(n--){
        result *= m;
    }
    return result;
}

/*
* @func 显示一个6x8标准ASCII字符
* @para y 行数
* @para x 列数
* @para ch 要显示的字符
*/
void OLED_P6x8Char(unsigned char y, unsigned char x, char ch)
{
    unsigned char c=0,i=0;

    c=ch-32;
    if(x>122)
    {
        x=0;
        y++;
    }
    OLED_Set_Pos(y,x);
    for(i=0;i<6;i++)
    {
        OLED_WrDat(F6x8[c][i]);
    }
}

/*
* @func 写入一组6x8标准ASCII字符串
* @para y 行数
* @para x 列数
* @para ch[] 要显示的字符串
*/
void OLED_P6x8Str(unsigned char y, unsigned char x, char ch[])
{
    unsigned char c=0,i=0,j=0;
    
    x = x*6;
    
    while (ch[j]!='\0')
    {
        c=ch[j]-32;
        if(x>126)
        {
            x=0;
            y++;
        }
        OLED_Set_Pos(y,x);
        for(i=0;i<6;i++)
        {
            OLED_WrDat(F6x8[c][i]);
        }
        x+=6;
        j++;
    }
}

/*
* @func 显示一个int类型的数据
* @para y 行数
* @para x 列数
* @para length 设定的整个数据长度（右对齐）
* @para ival 数据
*/
void OLED_P6x8Int(unsigned char y, unsigned char x, int32_t ival, unsigned char length)
{
    unsigned char   i;
    unsigned char   len;
    unsigned char   cval;
    int             power;
    
    x = x * 6;
    
    if(ival < 0){
        ival = -ival;
        OLED_P6x8Char(y, x, '-');
    }
	else{
		OLED_P6x8Char(y, x, ' ');
	}
	x += 6;
    len = digits(ival);
    power = my_power(10, len-1);
    for(i = len; i < length; i++){
        OLED_P6x8Char(y, x, ' ');
        x += 6;
    }
    for(i = 0; i < len; i++){
        cval = ival/power + '0';
        ival %= power;
        power /= 10;
        OLED_P6x8Char(y, x, cval);
        x += 6;
    }
}

/*
* @func 显示一个float类型的数据
* @para y 行数
* @para x 列数
* @para length 设定的整个数据长度（右对齐）
* @para lenFloat 小数位数
* @para dval 数据
*/
void OLED_P6x8Float(unsigned char y, unsigned char x, float dval, unsigned char length, unsigned char lenFloat)
{
    unsigned char   i;
    unsigned char   len;
    unsigned char   cval;
    unsigned int    ival;
    float           fval;
    int             power;
    
    if((x = x * 6) >= 6)      // 显示符号位
    {
        x = x - 6;
    }
    
    if(dval < 0){
        dval = -dval;
        OLED_P6x8Char(y, x, '-');
    }
    else{
        OLED_P6x8Char(y, x, '+');
    }
	x += 6;
	
    ival = (unsigned int)dval;
    fval = dval - ival;
    len  = digits(ival);
    power = my_power(10, len-1);
    for(i = len; i < length - lenFloat - 1; i++){
        OLED_P6x8Char(y, x, ' ');
        x += 6;
    }
    for(i = 0; i < len; i++){
        cval = ival/power + '0';
        ival %= power;
        power /= 10;
        OLED_P6x8Char(y, x, cval);
        x += 6;
    }
    OLED_P6x8Char(y, x, '.');
    x += 6;
    power = 10;
    for(i = 0; i < lenFloat; i++){
        cval = (unsigned char)(fval*power) + '0';
        fval -= (float)((unsigned char)(fval*power))/power;
        power *= 10;
        OLED_P6x8Char(y, x, cval);
        x += 6;
    }
}

/*
* @func 显示一副图像
* @para unsigned char 类型的数组（保存了图像信息）
* @para rowNum, columnNum
*/
void OLED_PrintImage(unsigned char *pucTable, unsigned char rowNum,  unsigned char columnNum)
{
    unsigned char i, j;
    unsigned char row, column;
    unsigned char maxRow;
    unsigned char bmp;
    
    maxRow  = (rowNum % 8 == 0 ? rowNum/8 : rowNum/8+1);
    for(row = 0; row < maxRow; row++)
    {
        OLED_Set_Pos(row, 0);
        j = (rowNum - row * 8) >= 8 ? 8 : (rowNum % 8); // 计算剩余的行数，如果大于8，下一行按照一行8格写入
        for(column = 0; column < columnNum; column++)
        {
            bmp = 0;
            for(i = 0; i < j; i++)
            {
                if(*(pucTable+(row*8+i)*column) != 0){
                    bmp |= 1 << i;
                }
            }
            OLED_WrDat(bmp);
        }
    }
}
