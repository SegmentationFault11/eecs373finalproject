#ifndef UART_DATA_PACKET_H__
#define UART_DATA_PACKET_H__

/*
 * Written by Aaryaman Sagar, Steven Ma, Artem Dudkov and Anton Markov for
 * EECS 373 @ The University of Michigan - Ann Arbor
 *
 * Please compile this with c99, "//" comments and variable declarations mixed
 * with code have been used freely here.  
 *
 * This module should be used for sending data across with UART, the UART
 * setup in the SmartFusions should be setup to get interrupted every time
 * there is 8 bytes worth of data in the buffer.
 */

#include <stdint.h>

/*
 * The size of a single data packet in bytes
 */
extern const uint32_t DATA_PACKET_SIZE;

/*
 * The unique IDs of the master and the slave nodes in the project.  These
 * should match the IDs set in the Xbee receiver.
 */
extern const uint8_t MASTER_ID;
extern const uint8_t SLAVE_ID;

typedef struct {

    /*
     * Forward padding.
     *
     * The forward and backwards padding must be present in the beginning and
     * at the end of the data packet.  In our tests all garbage data was
     * transerred in the beginning before and\or after the transfer.  This adds
     * an extra layer of protection against garbage data and makes sure that
     * the incoming data is not garbage.
     *
     * They are defined by the implementation and not exposed here. 
     *
     * So in mathematical terms this adds a safety factor that the incoming
     * data only has a 1/(2^16) proabability of being corrupt
     */
    uint8_t forward_padding;

    /*
     * The sender and receiver of the command.
     */
    uint8_t sender;
    uint8_t receiver;

    /*
     * This tells the slaves (cars) which interrupt handler in their local
     * user level interrupt vector is to be called.  i.e.  this defines an
     * index into an array of fucntion pointers in the slaves (cars).
     */
    uint8_t interrupt_vector_index;

    /*
     * The data of the command.  3 bytes are provided for that purpose. 
     */
    uint8_t command_data;

    /*
     * An unique identifier from the sender of the message.  This is relevant
     * only to the sender.  The receiver needs to reply to a message with the
     * sent unique id with the ack byte set to a 1 for the receiver to know
     * that the message has been received.  Or else he will keep sending the
     * same message over and over again.  
     */
    uint8_t message_id;

    /*
     * Used in a response message from a slave to its master indicating that
     * the packet was accepted.  If this is missing then the previous packet
     * should be sent again. 
     */
    uint8_t ack;

    /*
     * Forward padding.
     *
     * The forward and backwards padding must be present in the beginning and
     * at the end of the data packet.  In our tests all garbage data was
     * transerred in the beginning before and\or after the transfer.  This adds
     * an extra layer of protection against garbage data and makes sure that
     * the incoming data is not garbage.
     *
     * They are defined by the implementation and not exposed here. 
     *
     * So in mathematical terms this adds a safety factor that the incoming
     * data only has a 1/(2^16) proabability of being corrupt
     */
    uint8_t back_padding;

} UartDataPacket;

/*
 * PRECONDITION : Size of buffer array must be 14 bytes!  
 *
 * Use the following function to convert the command packet into a string.
 * Returns -1 on failure and 1 on success.
 */
int data_packet_convert_to_buffer(UartDataPacket* this_packet, uint8_t* buffer);

/*
 * PRECONDITION : Size of the buffer must be 14 bytes!
 *
 * Use the following function to convert the bytes from a given buffer to an
 * object of type UartDataPacket.  Returns -1 on failure and 1 on success.  On
 * failure the data packet is left in an invalid state
 */
int data_packet_construct_from_buffer(UartDataPacket* this_packet, 
        uint8_t* buffer);

/*
 * Constructs a data packet with the appropriate padding.
 */
void data_packet_construct(UartDataPacket* this_packet);

/*
 * Verifies whether a packet is in the appropriate state.  Use this to check a
 * packet before sending it.  Returns 1 on success and -1 on failure.
 */
int data_packet_verify(UartDataPacket* this_packet);

#endif
