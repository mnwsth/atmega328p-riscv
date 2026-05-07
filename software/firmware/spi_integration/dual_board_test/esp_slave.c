#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"

// Hardware VSPI (SPI3) Pins
#define PIN_MOSI 23
#define PIN_MISO 19
#define PIN_SCK  18
#define PIN_CS   5

// LED Pins
#define LED0 25 // LSB
#define LED1 26
#define LED2 27 // MSB

// ESP-IDF SPI Slave requires word-aligned buffers
WORD_ALIGNED_ATTR uint8_t rx_data[4] = {0};
WORD_ALIGNED_ATTR uint8_t tx_data[4] = {0};

void app_main(void) {
    // Give the USB/UART time to initialize so you don't miss the first logs
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    printf("--- ESP32 SPI Slave Initialized ---\n");
    printf("Listening for Master...\n");

    // Initialize LEDs as outputs
    gpio_set_direction((gpio_num_t)LED0, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)LED2, GPIO_MODE_OUTPUT);

    // Configure SPI Bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_MOSI,
        .miso_io_num = PIN_MISO,
        .sclk_io_num = PIN_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    // Configure SPI Slave Interface
    spi_slave_interface_config_t slvcfg = {
        .mode = 0, // Mode 0 (CPOL=0, CPHA=0). Change to 1 if you kept the software workaround.
        .spics_io_num = PIN_CS,
        .queue_size = 1,
        .flags = 0,
    };

    // Initialize SPI3 (VSPI) without DMA for simple 8-bit transfers
    spi_slave_initialize(SPI3_HOST, &buscfg, &slvcfg, SPI_DMA_DISABLED);

    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8; // 8 bits
    t.tx_buffer = tx_data;
    t.rx_buffer = rx_data;

    while (1) {
        // Block indefinitely until an 8-bit SPI transaction arrives from the FPGA
        spi_slave_transmit(SPI3_HOST, &t, portMAX_DELAY);

        uint8_t val = rx_data[0];
        uint8_t display_val = val % 8;

        printf("Received RAW: %3u | Modulo 8: %u | LEDs (MSB->LSB): [%d %d %d]\n", 
                val, 
                display_val, 
                (display_val >> 2) & 0x01, 
                (display_val >> 1) & 0x01, 
                display_val & 0x01);

        // Update physical LED IO ports
        gpio_set_level((gpio_num_t)LED0, display_val & 0x01);
        gpio_set_level((gpio_num_t)LED1, (display_val >> 1) & 0x01);
        gpio_set_level((gpio_num_t)LED2, (display_val >> 2) & 0x01);
    }
}
