#include "UartDataPacket.h"
#include <stdint.h>
#include <string.h>

/* define the global variables from the header file UartDataPacket.h */
const uint32_t DATA_PACKET_SIZE = 8;
const uint8_t MASTER_ID = 1;
const uint8_t SLAVE_ID = 2;

static const uint8_t INCORRECT_ID = 0;
static const uint8_t FORWARD_PADDING = 0;
static const uint8_t BACK_PADDING = 255;

int data_packet_convert_to_buffer(UartDataPacket* this_packet, uint8_t* buffer) {
    
    /* verify whether the packet is in the right state */
    if (data_packet_verify(this_packet) == -1) {
        return -1;
    }

    /* copy into the buffer */
    memcpy((void*)buffer, (void*)this_packet, sizeof(UartDataPacket));
    return 1;
}

int data_packet_construct_from_buffer(UartDataPacket* this_packet, 
        uint8_t* buffer) {

    /* verify whether the packet is in the right state */
    if (data_packet_verify((UartDataPacket*) buffer) == -1) {
        
        /* leave the data packet in an invalid state */
        data_packet_construct(this_packet);
        return -1;
    }

    /* copy from the buffer into this */
    memcpy((void*)this_packet, (void*)buffer, sizeof(UartDataPacket));
    return 1;
}

void data_packet_construct(UartDataPacket* this_packet) {

    /* set paddings */
    this_packet->forward_padding = FORWARD_PADDING;
    this_packet->back_padding = BACK_PADDING;

    /* set the sender and receiver to an incorrect value */
    this_packet->sender = INCORRECT_ID;
    this_packet->receiver = INCORRECT_ID;

    this_packet->interrupt_vector_index = 0;
}

int data_packet_verify(UartDataPacket* this_packet) {

    /* check for appropriate padding presence */
    if (this_packet->forward_padding != FORWARD_PADDING ||
            this_packet->back_padding != BACK_PADDING) {
        return -1;
    }

    /* check if the sender and receiver are what they should be */
    if ((this_packet->sender != MASTER_ID && this_packet->sender != SLAVE_ID)  ||
            (this_packet->receiver != MASTER_ID && 
            this_packet->receiver != SLAVE_ID)) {
        return -1;
    }

    return 1;
}
