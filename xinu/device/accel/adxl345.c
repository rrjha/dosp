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

void read_rx_data(byte* rxstream, int32 length)
{
    struct spi_transfer adxl_buf;
    byte *txstream = (byte*)getmem(length);
    memset(txstream, 0, length); // tx for ignore

    /* We can not verify correctness of length of actual param so just null check */
    if((rxstream != NULL) && (txstream != NULL)){
        adxl_buf.txbuf = txstream;
        adxl_buf.rxbuf = rxstream;
        adxl_buf.length = length;
        control(SPI0, SPI_CTRL_TRANSFER, &adxl_buf, 0);
    }

    freemem((char*)txstream, length);
}

void write_register(byte address, byte value)
{
    send_tx_data(address);
    send_tx_data(value);
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
    read_rx_data((byte*) dataPtr, sizeof(struct accel_data));
    return OK;
}
