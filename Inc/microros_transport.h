/*
 * microros_transport.h
 *
 *  Created on: May 20, 2026
 *      Author: mat-ros
 */

#ifndef MICROROS_TRANSPORT_H_
#define MICROROS_TRANSPORT_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <uxr/client/transport.h>


bool transport_open(struct uxrCustomTransport* transport);
bool transport_close(struct uxrCustomTransport* transport);

size_t transport_write(struct uxrCustomTransport* transport,const uint8_t* buf, size_t len, uint8_t* errcode);
size_t transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout_ms, uint8_t* errcode);

#endif /* MICROROS_TRANSPORT_H_ */
