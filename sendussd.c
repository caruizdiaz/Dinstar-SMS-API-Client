/*
 * main.c
 *
 *  Created on: Sep 19, 2012
 *      Author: caruizdiaz
 *      		caruizdiaz.com
 */

#include "dwg/dwg_util.h"
#include "dwg/dwg.h"

static int _connected	= 0;

void new_ussd_handler(str_t *gw_ip, dwg_ussd_received_t *ussd)
{
	LOG(L_DEBUG, "[%.*s] status: %d content: [%.*s]\n", gw_ip->len, gw_ip->s,
														ussd->status,
														ussd->message.len, ussd->message.s);

}

void new_sms_handler(str_t *gw_ip, dwg_sms_received_t *sms)
{
	LOG(L_DEBUG, "[%.*s] Encoding: %d, From: %.*s, Len: %d, Text: %.*s\n", gw_ip->len, gw_ip->s,
										sms->encoding,
										sms->str_number.len, sms->str_number.s,
										sms->message.len,
										sms->message.len, sms->message.s);
}


void status_handler(str_t *gw_ip, dwg_ports_status_t *status)
{
	int index = 0;

	LOG(L_DEBUG, "\t[%.*s] Number of ports: %d\n", gw_ip->len, gw_ip->s, status->size);

	for (index = 0; index < status->size; index++)
	{
		LOG(L_DEBUG, "\tPort%d: %d\n", index, status->status_array[index].status);
	}

	_connected = 1;
}

void msg_response_handler(str_t *gw_ip, dwg_sms_response_t *response)
{
	LOG(L_DEBUG, "\t[%.*s] ACK response from %s\n", gw_ip->len, gw_ip->s, response->number);
}

void start_interactive_ussd_session(int port)
{
	char input[256];

	while(1)
	{
		printf("ussd request: ");

		if (fgets(input, sizeof(input), stdin))
		{
			char *nl = strchr(input, '\n');

			input[nl - input] = 0;

			if (strlen(input) == 0)
				continue;

			str_t service = { input, strlen(input) };

			dwg_send_ussd(&service, port);
			printf("Request [%.*s] sent. Waiting for answer... \n", service.len, service.s);
		}
	}
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Invalid usage:\n\t./app <listen-port> <gw-port>\n");
		exit(1);
	}

	dwg_message_callback_t callbacks	= {
				.status_callback 		= status_handler,
				.msg_response_callback	= msg_response_handler,
				.msg_sms_recv_callback	= new_sms_handler,
				.msg_ussd_recv_callback = new_ussd_handler
	};

	LOG(L_DEBUG, "%s: Setting up server... \n", __FUNCTION__);

	int listen_port	= atoi(argv[1]);
	int port  		= atoi(argv[2]);

	dwg_start_server(listen_port, &callbacks);

	LOG(L_DEBUG, "%s: Server listening at port %d\n", __FUNCTION__, listen_port);

	do
	{
		LOG(L_DEBUG, "%s: Gateway not connected. Waiting... \n", __FUNCTION__);
		sleep(1);
	}
	while(_connected == 0);

	start_interactive_ussd_session(port);

	return 0;
}
