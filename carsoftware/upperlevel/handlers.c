#include "headers.h"

extern struct Settings settings;
extern struct Vehicle vehicle;
extern int VEHICLE_ID;

void GPIO4_IRQHandler() {
	MSS_GPIO_clear_irq(MSS_GPIO_4);

	sample_steer();

	if (settings.acc_counter >> 3 == 1) {
		sample_acc();
		settings.acc_counter = 0;
	} else
		++settings.acc_counter;

	decode_controller();

	trigger();
}

void GPIO6_IRQHandler() {
	got_shot();
	MSS_GPIO_clear_irq(MSS_GPIO_6);
}

void incoming_handler(mss_uart_instance_t * this_uart) {
	// print so that the user knows that data has been received
	// printf("received data : ");

	// keep reading off the FIFO until there is something nothing in it
	uint8_t receive_buffer[14];

	size_t received;
	size_t received_till_now = 0;
	while ((received = MSS_UART_get_rx(&g_mss_uart1, receive_buffer
			+ received_till_now, 14 - received_till_now)) != 0) {

		// increment the received till now variable so that the new values read
		// in get read in from where they were left off
		received_till_now += received;

		if (received_till_now == 14) break;

		//xbee_broadcast(receive_buffer);
	}

	int i;
	for (i = 0; i < 14; ++i) {
		printf("0x%02x ", receive_buffer[i]);
	}

	printf("\r\n");
	if ((receive_buffer[1] == VEHICLE_ID || receive_buffer[1] == 1) && receive_buffer[0] == 0 && receive_buffer[1]) {
		if (receive_buffer[2] == 0x73) {
			if (receive_buffer[3] == 0) {
				init_vehicle(VEHICLE_ID, F1, VEHICLE_ID);
			}
			else if (receive_buffer[3] == 1) {
				init_vehicle(VEHICLE_ID, Toyota, VEHICLE_ID);
			}
			else if (receive_buffer[3] == 2) {
				init_vehicle(VEHICLE_ID, Tank, VEHICLE_ID);
			}
			else if (receive_buffer[3] == 3) {
				init_vehicle(VEHICLE_ID, Script, VEHICLE_ID);
			}
		}
		else if (receive_buffer[2] == 0x67) {
			start_motor();
			start_steer();
			settings.game_start = 1;
		}
		else if (receive_buffer[2] == 0x74) {
			tmnt_vehicle();
		}
		else if (receive_buffer[2] == 0x75) {
			receive_upgrade(1);
		}
	}




}
