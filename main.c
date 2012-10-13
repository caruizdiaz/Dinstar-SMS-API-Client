/*
 * main.c
 *
 *  Created on: Sep 19, 2012
 *      Author: caruizdiaz
 *      		caruizdiaz.com
 */

#include "dwg/dwg_util.h"
#include "dwg/dwg.h"

#define LISTEN_PORT		7008
#define SEND_SMS_PORT	0

void new_sms_handler(str_t *gw_ip, dwg_sms_received_t *sms)
{
	LOG(L_DEBUG, "[%.*s] Encoding: %d, From: %s, Len: %d, Text: %.*s\n", gw_ip->len, gw_ip->s,
										sms->encoding,
										sms->number,
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
}

void msg_response_handler(str_t *gw_ip, dwg_sms_response_t *response)
{
	LOG(L_DEBUG, "\t[%.*s] ACK response from %s\n", gw_ip->len, gw_ip->s, response->number);
}

int main(int argc, char** argv)
{
	dwg_message_callback_t callbacks	= {
				.status_callback 		= status_handler,
				.msg_response_callback	= msg_response_handler,
				.msg_sms_recv_callback	= new_sms_handler
	};

	LOG(L_DEBUG, "%s: Setting up sever... \n", __FUNCTION__);

	dwg_start_server(LISTEN_PORT, &callbacks);

	LOG(L_DEBUG, "%s: Server listening at port %d\n", __FUNCTION__, LISTEN_PORT);

	str_t des = { "59598146623", sizeof("59598146623") - 1};
	str_t msg = { "test from PY!", sizeof("test from PY!") - 1 };

	for(;;)
	{
		getchar();
		dwg_send_sms(&des, &msg, SEND_SMS_PORT);
	}

	return 0;
}
