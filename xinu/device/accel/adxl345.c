/* adxl345.c - digital accel sensor */

#include <xinu.h>


byte devid=0;


void write_register(byte address, byte value)
{
    struct spi_transfer adxl_buf;
    byte tx[2], rx[2];

    // Writes are only single byte so MB not needed
    tx[0] = address & 0x3F;
    tx[1] = value &0x3F;
    adxl_buf.txbuf = &tx[0];
    adxl_buf.rxbuf = &rx[0]; //don't care
    adxl_buf.length = 2;

    control(SPI0, SPI_CTRL_TRANSFER, &adxl_buf, 0); // 4th arg is dummy
}

byte read_register(byte address)
{
    struct spi_transfer adxl_buf;
    byte tx[2] = {0, 0}, rx[2];

    // Indicate a read
    tx[0] = (address & 0x3F) | 0x80;

    adxl_buf.txbuf = &tx[0];
    adxl_buf.rxbuf = &rx[0];
    adxl_buf.length = 2;

    control(SPI0, SPI_CTRL_TRANSFER, &adxl_buf, 0); // 4th arg is dummy

    return rx[1];
}

devcall accel_init()
{
    // Init SPI driver
    init(SPI0);

    // Read the ID
    devid = read_register(0);

    if((devid & 0xFF) != 0xE5){
        kprintf("Dev Id returned 0x%X doesn't match\n", devid);
        return SYSERR;
    }

    // Set the sensor to +/- 2G mode. Better sensitivity
    write_register(DATA_FORMAT, 0x0);

    // Now set the power control reg in measurement mode
    write_register(POWER_CTL, 0x08);

    return OK;
}

devcall accel_read(struct accel_data *dataPtr)
{
    byte x0=0, x1=0, y0=0, y1=0, z0=0, z1=0;

    if (devid != 0xE5){
        kprintf("Read before Init - Error\n");
        return SYSERR;
    }

    x0 = read_register(DATAX0);
    x1 = read_register(DATAX1);
    y0 = read_register(DATAY0);
    y1 = read_register(DATAY1);
    z0 = read_register(DATAZ0);
    z1 = read_register(DATAZ1);

    dataPtr->x = x1 << 8 | x0;
    dataPtr->y = y1 << 8 | y0;
    dataPtr->z = z1 << 8 | z0;

    return OK;
}
