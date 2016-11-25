/* adxl345.h - ADXL345 definitions */

/* Some useful registers */
#define POWER_CTL 0x2D	//Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32	//X-Axis Data 0
#define DATAX1 0x33	//X-Axis Data 1
#define DATAY0 0x34	//Y-Axis Data 0
#define DATAY1 0x35	//Y-Axis Data 1
#define DATAZ0 0x36	//Z-Axis Data 0
#define DATAZ1 0x37	//Z-Axis Data

struct accel_data {
    int16 x;
    int16 y;
    int16 z;
};

devcall accel_read(struct accel_data *dataPtr);
