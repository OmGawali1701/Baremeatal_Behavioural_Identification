#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    char name[16];
    bool active;
    uint32_t tx_count;
    uint32_t rx_count;
} port_stat_t;

void port_monitor_init();
int port_monitor_collect(port_stat_t *ports, int max_ports);
