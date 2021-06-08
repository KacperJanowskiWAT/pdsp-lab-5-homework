/*
 * circular_buffer.h
 *
 *  Created on: 4 cze 2021
 *      Author: Kacper Janowski
 */

#ifndef PROCESSING_CIRCULAR_BUFFER_H_
#define PROCESSING_CIRCULAR_BUFFER_H_

typedef struct {
	float * buffer;			// bufor na probki
	uint32_t head;			// aktualna pozycja "glowa"
	uint32_t tail;			// biezacy "ogon" (zwiekszamy, gdy elementy sa usuwane)
	uint32_t maxlen;		// maksymalny rozmiar bufora
}circular_buf_F_t;

typedef struct {
	channel_t * buffer;		// bufor na probki
	uint32_t head;			// aktualna pozycja "glowa"
	uint32_t tail;			// biezacy "ogon" (zwiekszamy, gdy elementy sa usuwane)
	uint32_t maxlen;		// maksymalny rozmiar bufora
}circular_buf_I_t;

uint8_t circ_buf_F_push(circular_buf_F_t *cbuf, channel_t data);

channel_t circ_buf_F_pop(circular_buf_F_t *cbuf, channel_t *data);

uint8_t circ_buf_I_push(circular_buf_I_t *cbuf, channel_t data);

channel_t circ_buf_I_pop(circular_buf_I_t *cbuf, channel_t *data);

#endif /* PROCESSING_CIRCULAR_BUFFER_H_ */
