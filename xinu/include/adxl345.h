/* adxl345.h - ADXL345 definitions */

/* Some useful registers */
#define DEVID 0x0
#define POWER_CTL 0x2D	//Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32	//X-Axis Data 0
#define DATAX1 0x33	//X-Axis Data 1
#define DATAY0 0x34	//Y-Axis Data 0
#define DATAY1 0x35	//Y-Axis Data 1
#define DATAZ0 0x36	//Z-Axis Data 0
#define DATAZ1 0x37	//Z-Axis Data

#define READ_MASK 0x80
#define MB_MASK 0x40
#define SB_WRITE_MASK 0x3F
#define DEVICEID 0xE5
#define RANGE_2G 0x0
#define RANGE_4G 0x1
#define RANGE_8G 0x2
#define RANGE_16G 0x3
#define MEASURE 0x08


struct accel_data {
    int16 x;
    int16 y;
    int16 z;
};

extern devcall	accel_read(struct dentry *, struct accel_data *, uint32);
