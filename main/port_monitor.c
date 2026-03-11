#include "port_monitor.h"
#include <string.h>

static uint32_t uart_tx = 0;
static uint32_t uart_rx = 0;

static uint32_t i2c_tx = 0;
static uint32_t i2c_rx = 0;

static uint32_t spi_tx = 0;
static uint32_t spi_rx = 0;

static uint32_t gpio_events = 0;

void port_monitor_init()
{
    uart_tx = 0;
    uart_rx = 0;
    i2c_tx = 0;
    i2c_rx = 0;
    spi_tx = 0;
    spi_rx = 0;
    gpio_events = 0;
}

int port_monitor_collect(port_stat_t *ports, int max_ports)
{
    int index = 0;

    if (index < max_ports)
    {
        strcpy(ports[index].name, "UART");
        ports[index].active = (uart_tx + uart_rx) > 0;
        ports[index].tx_count = uart_tx;
        ports[index].rx_count = uart_rx;
        index++;
    }

    if (index < max_ports)
    {
        strcpy(ports[index].name, "I2C");
        ports[index].active = (i2c_tx + i2c_rx) > 0;
        ports[index].tx_count = i2c_tx;
        ports[index].rx_count = i2c_rx;
        index++;
    }

    if (index < max_ports)
    {
        strcpy(ports[index].name, "SPI");
        ports[index].active = (spi_tx + spi_rx) > 0;
        ports[index].tx_count = spi_tx;
        ports[index].rx_count = spi_rx;
        index++;
    }

    if (index < max_ports)
    {
        strcpy(ports[index].name, "GPIO");
        ports[index].active = gpio_events > 0;
        ports[index].tx_count = gpio_events;
        ports[index].rx_count = 0;
        index++;
    }

    return index;
}