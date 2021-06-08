/*
 * circular_buffer.c
 *
 *  Created on: 4 cze 2021
 *      Author: Kacper Janowski
 */

#include "pdsp/pdsp.h"
#include "processing/circular_buffer.h"

/*
 * Bufor kołowy to struktura danych FIFO, która traktuje pamięć jako cykliczną; oznacza to, że indeksy odczytu/zapisu
 * zapętlają się z powrotem do 0 po osiągnięciu długości bufora. Osiąga się to za pomocą dwóch wskaźników do tablicy,
 * wskaźnika „głowy” i wskaźnika „ogonu”. Gdy dane są dodawane (zapis) do bufora, wskaźnik głowy jest zwiększany i podobnie,
 * gdy dane są usuwane (odczyt), wskaźnik ogona jest zwiększany.
 */

uint8_t circ_buf_F_push(circular_buf_F_t *cbuf, channel_t data){
	uint8_t next;

	next = cbuf->head + 1;				// next wskazuje na kolejny element bufora po wykonaniu poniższych operacji
	if (next >= cbuf->maxlen)
		next = 0;

	if (next == cbuf->tail)				// jeśli head + 1 == tail, bufor jest pełny
		return -1;

	cbuf->buffer[cbuf->head] = data;	// załaduj i przenieś dane
	cbuf->head = next;					// przejdź do kolejnego miejsca w buforze
	return 0;
}

channel_t circ_buf_F_pop(circular_buf_F_t *cbuf, channel_t *data){
	uint8_t next;

	if (cbuf->head == cbuf->tail)		// Jeśli head == tail, nie ma żadnych danych w buforze
		return 0;

	next = cbuf->tail + 1;				// next wskazuje na kolejny element bufora po wykonaniu poniższych operacji
	if (next >= cbuf->maxlen)
		next = 0;

	*data = cbuf->buffer[cbuf->tail];	// odczytaj dane
	cbuf->tail = next;					// przesunięce tail o wyznaczoną wcześniej wartość next
	return *data;						// zwraca dane
}

uint8_t circ_buf_I_push(circular_buf_I_t *cbuf, channel_t data){
	uint8_t next;

	next = cbuf->head + 1;
	if (next >= cbuf->maxlen)
		next = 0;

	if (next == cbuf->tail)
		return -1;

	cbuf->buffer[cbuf->head] = data;
	cbuf->head = next;
	return 0;
}

channel_t circ_buf_I_pop(circular_buf_I_t *cbuf, channel_t *data){
	uint8_t next;

	if (cbuf->head == cbuf->tail)
		return 0;

	next = cbuf->tail + 1;
	if (next >= cbuf->maxlen)
		next = 0;

	*data = cbuf->buffer[cbuf->tail];
	cbuf->tail = next;
	return *data;
}


















