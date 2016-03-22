#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "UartDataPacket.h"

int main() {

    UartDataPacket uart_packet;
    assert(sizeof(UartDataPacket) == 8 && sizeof(uart_packet) == 8);

    uint8_t buffer1[8] = {0, 1, 2, 0, 10, 10, 10, 255};;
    assert(data_packet_construct_from_buffer(&uart_packet, buffer1) != -1);
    assert(uart_packet.forward_padding == 0);
    assert(uart_packet.sender == 1);
    assert(uart_packet.receiver == 2);
    assert(uart_packet.interrupt_vector_index == 0);
    assert(uart_packet.back_padding == 255);

    uint8_t buffer2[8] = {0, 3, 2, 0, 10, 10, 10, 255};
    assert(data_packet_construct_from_buffer(&uart_packet, buffer2) == -1);

    uint8_t buffer3[8] = {0, 1, 3, 0, 10, 10, 10, 255};
    assert(data_packet_construct_from_buffer(&uart_packet, buffer3) == -1);

    return 0;
}
