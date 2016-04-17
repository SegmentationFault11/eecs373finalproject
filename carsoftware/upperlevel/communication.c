#include "communication.h"

extern struct Settings settings;
extern struct Vehicle vehicle;
void incoming_handler(mss_uart_instance_t * this_uart);

inline void init_comms() {
	MSS_UART_set_rx_handler(&g_mss_uart1, incoming_handler, MSS_UART_FIFO_FOURTEEN_BYTES);
}

inline void send_event() {
	unsigned char message[14] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	message[0]  = (unsigned char)0;
	message[1]  = (unsigned char)0;
	message[2]  = (unsigned char)'e';
	message[3]  = (unsigned char)vehicle.id;
	message[4]  = (unsigned char)vehicle.status.HP;
	message[5]  = (unsigned char)vehicle.last_hit_id;
	message[13] = (unsigned char)0;

	MSS_UART_polled_tx(&g_mss_uart1,  message, (uint32_t)14);
}
