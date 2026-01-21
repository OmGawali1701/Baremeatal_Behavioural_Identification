#include "uart_checker.h"
#include "driver/uart.h"
#include "esp_log.h"

#define UART_PORT UART_NUM_0

void uart_checker_init(void)
{
    uart_config_t cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_PORT, 2048, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &cfg);
}

void uart_checker_print_json(const char *json)
{
    if (!json) return;
    uart_write_bytes(UART_PORT, json, strlen(json));
    uart_write_bytes(UART_PORT, "\n", 1);
}
