#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Hardware SPI0 Pins
#define SPI_PORT spi0
#define PIN_MISO 19 // TX
#define PIN_CS   17 // CSn
#define PIN_SCK  18 // SCK
#define PIN_MOSI 16 // RX

// LED Pins
#define LED0 6 // LSB
#define LED1 7
#define LED2 8 // MSB

int main() {
    // Initialize standard I/O (routed to USB via CMake)
    stdio_init_all();
    
    // Wait 2 seconds for the USB serial connection to mount on the host PC
    sleep_ms(2000);
    printf("--- Shrike SPI Slave Initialized ---\n");
    printf("Listening for Master...\n");

    // Initialize LEDs
    gpio_init(LED0); gpio_set_dir(LED0, GPIO_OUT);
    gpio_init(LED1); gpio_set_dir(LED1, GPIO_OUT);
    gpio_init(LED2); gpio_set_dir(LED2, GPIO_OUT);

    // Initialize SPI
    spi_init(SPI_PORT, 1000 * 1000);
    spi_set_slave(SPI_PORT, true);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    uint8_t rx_data;

    while (1) {
        // Block and wait for an incoming byte
        spi_read_blocking(SPI_PORT, 0, &rx_data, 1);

        // Apply modulo 8
        uint8_t display_val = rx_data % 8;

        // Print the logs to the USB Serial terminal
        printf("Received RAW: %3u | Modulo 8: %u | LEDs (MSB->LSB): [%d %d %d]\n", 
                rx_data, 
                display_val, 
                (display_val >> 2) & 0x01, 
                (display_val >> 1) & 0x01, 
                display_val & 0x01);

        // Update physical LED IO ports
        gpio_put(LED0, display_val & 0x01);
        gpio_put(LED1, (display_val >> 1) & 0x01);
        gpio_put(LED2, (display_val >> 2) & 0x01);
    }
    
    return 0;
}
