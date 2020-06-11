//
//  sj_bmp280.c
//

#include "sj_bmp280.h"
#include <math.h>

/*bmp280*/
#define BMP280_PRESSURE_OSR (BMP280_OVERSAMP_8X)
#define BMP280_TEMPERATURE_OSR (BMP280_OVERSAMP_16X)
#define BMP280_MODE (BMP280_PRESSURE_OSR << 2 | BMP280_TEMPERATURE_OSR << 5 | BMP280_NORMAL_MODE)

typedef struct
{
  int dig_T1; /* calibration T1 data */
  int dig_T2; /* calibration T2 data */
  int dig_T3; /* calibration T3 data */
  int dig_P1; /* calibration P1 data */
  int dig_P2; /* calibration P2 data */
  int dig_P3; /* calibration P3 data */
  int dig_P4; /* calibration P4 data */
  int dig_P5; /* calibration P5 data */
  int dig_P6; /* calibration P6 data */
  int dig_P7; /* calibration P7 data */
  int dig_P8; /* calibration P8 data */
  int dig_P9; /* calibration P9 data */
  int t_fine; /* calibration t_fine data */
} bmp280Calib;

//气压计总结构体
_BMPDATA_ bmpdata;

bmp280Calib bmp280Cal;
static int bmp280ID = 0;
static int bmp280RawPressure = 0;
static int bmp280RawTemperature = 0;

int BMP280_Config(void)
{
  //simiic_write_reg(BMP280_ADDR, BMP280_RST_REG, 0xB6);
  bmp280ID = simiic_read_reg(BMP280_ADDR, BMP280_CHIP_ID, IIC);

  //simiic_read_regs(BMP280_ADDR, BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG, bmp280cal, 12, IIC);
  simiic_write_reg(BMP280_ADDR, BMP280_CTRL_MEAS_REG, BMP280_MODE);
  simiic_write_reg(BMP280_ADDR, BMP280_CONFIG_REG, 5 << 2);

  unsigned char byte1, byte2;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x88, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x89, IIC);
  bmp280Cal.dig_T1 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x8A, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x8B, IIC);
  bmp280Cal.dig_T2 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x8C, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x8D, IIC);
  bmp280Cal.dig_T3 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x8E, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x8F, IIC);
  bmp280Cal.dig_P1 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x90, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x91, IIC);
  bmp280Cal.dig_P2 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x92, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x93, IIC);
  bmp280Cal.dig_P3 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x94, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x95, IIC);
  bmp280Cal.dig_P4 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x96, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x97, IIC);
  bmp280Cal.dig_P5 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x98, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x99, IIC);
  bmp280Cal.dig_P6 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x9A, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x9B, IIC);
  bmp280Cal.dig_P7 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x9C, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x9D, IIC);
  bmp280Cal.dig_P8 = byte2 << 8 | byte1;
  byte1 = simiic_read_reg(BMP280_ADDR, 0x9E, IIC);
  byte2 = simiic_read_reg(BMP280_ADDR, 0x9F, IIC);
  bmp280Cal.dig_P9 = byte2 << 8 | byte1;

  bmp280Cal.dig_T1 = 27504;
  bmp280Cal.dig_T2 = 26435;
  bmp280Cal.dig_T3 = -1000;
  bmp280Cal.dig_P1 = 36477;
  bmp280Cal.dig_P2 = -10685;
  bmp280Cal.dig_P3 = 3024;
  bmp280Cal.dig_P4 = 2855;
  bmp280Cal.dig_P5 = 140;
  bmp280Cal.dig_P6 = -7;
  bmp280Cal.dig_P7 = 15500;
  bmp280Cal.dig_P8 = -14600;
  bmp280Cal.dig_P9 = 6000;

  return bmp280ID;
}

static void bmp280GetPressure(void)
{
  int data[BMP280_DATA_FRAME_SIZE];

  // read data from sensor
  simiic_read_regs(BMP280_ADDR, BMP280_PRESSURE_MSB_REG, data, BMP280_DATA_FRAME_SIZE, IIC);
  bmp280RawPressure = (int)((((uint32_t)(data[0])) << 12) | (((uint32_t)(data[1])) << 4) | ((uint32_t)data[2] >> 4));
  bmp280RawTemperature = (int)((((uint32_t)(data[3])) << 12) | (((uint32_t)(data[4])) << 4) | ((uint32_t)data[5] >> 4));
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC
// t_fine carries fine temperature as global value
static float bmp280CompensateT(int adcT)
{
  int var1, var2;
  float T;

  var1 = (((double)adcT) / 16384.0 - ((double)bmp280Cal.dig_T1) / 1024.0) * ((double)bmp280Cal.dig_T2);
  var2 = ((((double)adcT) / 131072.0 - ((double)bmp280Cal.dig_T1) / 8192.0) * (((double)adcT) / 131072.0 - ((double)bmp280Cal.dig_T1) / 8192.0)) * ((double)bmp280Cal.dig_T3);

  bmp280Cal.t_fine = (unsigned long)(var1 + var2);
  T = (bmp280Cal.t_fine * 5 + 128) >> 8;

  return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
static int bmp280CompensateP(int adcP)
{
  /*int64_t var1, var2, p;
    //
    var1 = ((double)bmp280Cal.t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)bmp280Cal.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)bmp280Cal.dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)bmp280Cal.dig_P4) * 65536.0);
    var1 = (((double)bmp280Cal.dig_P3) * var1 * var1 / 524288.0 + ((double)bmp280Cal.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)bmp280Cal.dig_P1);
    p = 1048576.0 - (double)adcP;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)bmp280Cal.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)bmp280Cal.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)bmp280Cal.dig_P7)) / 16.0;

    return p;*/

  int64_t var1, var2, p;
  var1 = ((int64_t)bmp280Cal.t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp280Cal.dig_P6;
  var2 = var2 + ((var1 * (int64_t)bmp280Cal.dig_P5) << 17);
  var2 = var2 + (((int64_t)bmp280Cal.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)bmp280Cal.dig_P3) >> 8) + ((var1 * (int64_t)bmp280Cal.dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp280Cal.dig_P1) >> 33;
  if (var1 == 0)
    return 0;
  p = 1048576 - adcP;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)bmp280Cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)bmp280Cal.dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280Cal.dig_P7) << 4);
  return (uint32_t)p;
}

#define FILTER_NUM 5
#define FILTER_A 0.1f


static void presssureFilter(float *in, float *out)
{
  static int i = 0;
  static float filter_buf[FILTER_NUM] = {0.0};
  double filter_sum = 0.0;
  int cnt = 0;
  float deta;

  if (filter_buf[i] == 0.0f)
  {
    filter_buf[i] = *in;
    *out = *in;
    if (++i >= FILTER_NUM)
      i = 0;
  }
  else
  {
    if (i)
      deta = *in - filter_buf[i - 1];
    else
      deta = *in - filter_buf[FILTER_NUM - 1];

    if (fabs(deta) < FILTER_A)
    {
      filter_buf[i] = *in;
      if (++i >= FILTER_NUM)
        i = 0;
    }
    for (cnt = 0; cnt < FILTER_NUM; cnt++)
    {
      filter_sum += filter_buf[cnt];
    }
    *out = filter_sum / FILTER_NUM;
  }
}

#define CONST_PF 0.1902630958 //(1/5.25588f) Pressure factor
#define FIX_TEMP 25           // Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
// TLDR: Adjusting for temp changes does more harm than good.
/**
   Converts pressure to altitude above sea level (ASL) in meters
*/
static float bmp280PressureToAltitude(float pressure, float groundPressure, float groundTemp)
{
  if (pressure > 0)
  {
    return 44300 * (1 - pow((pressure / groundPressure), CONST_PF)); //101325.0
    //return ((pow((1015.7f / pressure), CONST_PF) - 1.0f) * (FIX_TEMP + 273.15f)) / 0.0065f;
  }
  else
  {
    return 0;
  }
}

void BMP280_DataGet(void)
{
  static float t;
  static float p;

  bmp280GetPressure();
  t = bmp280CompensateT(bmp280RawTemperature) / 100.0;
  p = bmp280CompensateP(bmp280RawPressure) / 25600.0;

  presssureFilter(&p, &bmpdata.pressure);
  bmpdata.temperature = (float)t; /*单位度*/
  bmpdata.altitude = bmp280PressureToAltitude(bmpdata.pressure, 1162.79, t); /*转换成海拔*/
}
