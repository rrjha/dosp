/* adxl345.c - digital accel sensor */

#include <xinu.h>


void send_tx_data(byte value)
{
    struct spi_transfer adxl_buf;
    byte tx, rx;

    tx = value;
    adxl_buf.txbuf = &tx;
    adxl_buf.rxbuf = &rx; //don't care
    adxl_buf.length = 1;

    control(SPI0, SPI_CTRL_TRANSFER, &adxl_buf, 0); // 4th arg is dummy
}

byte read_rx_data()
{
    struct spi_transfer adxl_buf;
    byte tx=0, rx;

    adxl_buf.txbuf = &tx;
    adxl_buf.rxbuf = &rx; //don't care
    adxl_buf.length = 1;

    control(SPI0, SPI_CTRL_TRANSFER, &adxl_buf, 0); // 4th arg is dummy

    return rx;
}

void write_register(byte address, byte value)
{
    send_tx_data(address);
    send_tx_data(value);
}

byte read_register(byte address)
{
    byte value;
    send_tx_data(address);
    value = read_rx_data();

    return value;
}

void accel_init()
{
    // Init SPI driver
    init(SPI0);

    // Set the sensor to +/- 2G mode. Better sensitivity
    write_register(DATA_FORMAT, 0x0);

    // Now set the power control reg in measurement mode
    write_register(POWER_CTL, 0x08);
}

devcall accel_read(struct accel_data *dataPtr)
{
    byte x0, x1, y0, y1, z0, z1;

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
