/*
 * filters.c
 *
 *  Created on: 31 maj 2021
 *      Author: pdabal/Kacper Janowski
 */

#include "pdsp/pdsp.h"
#include "processing/filters.h"

// new
void FIR_InitF(FIR_CfgF_t *hCfg, circular_buf_F_t *circBufF, float *x, float *h, uint32_t n) {
	hCfg->x = x;
	hCfg->h = h;
	hCfg->n = n;

	// uzupelnienie bufora cyklicznego
	circBufF->buffer = x;
	circBufF->maxlen = n;
	circBufF->head = 0;
	circBufF->tail = 0;
}

// new
void FIR_InitI(FIR_CfgI_t *hCfg, circular_buf_I_t *circBufI, channel_t *x, channel_t *h, uint32_t n) {
	hCfg->x = x;
	hCfg->h = h;
	hCfg->n = n;

	// uzupelnienie bufora cyklicznego
	circBufI->buffer = x;
	circBufI->maxlen = n;
	circBufI->head = 0;
	circBufI->tail = 0;
}

// new
channel_t FIR_GetSampleI(FIR_CfgI_t *hCfg, circular_buf_I_t *circBufI, channel_t value) {
	uint32_t k;
	int32_t y = 0;
	channel_t data;

	// Dodanie próbki do bufora
	// Funkcja która dodaje dane do bufora kołowego
	circ_buf_I_push(circBufI, value);

	// Suma iloczynów
	for (k = 0; k < hCfg->n; k++) {
		// Użycie funkcji pobierającej dane z bufora kołowego
		// przy każdym wywołaniu funkcji circ_buf_I_pop odczytywana jest kolejna wartość z bufora
		// aż do końca czyli do rzędu filtru
		y += ((hCfg->h[k]) * circ_buf_I_pop(circBufI, &data));
	}

	// Nie ma potrzeby przesuwania bufora ponieważ używany jest bufor kołowy
	// przy kolejnym wywoałanu funkcji rozpocznie się zapis danych do zwiększonego bufora
	// gdy funkcji wykona się tyle razy ile wynosi rząd filtru, bufor zostanie wyzerowany i procedura rozpocznie sie od poczatku

	if (PDSP_SAMPLE_SIZE == 1)
		return (channel_t) (y >> 7);
	else
		return (channel_t) (y >> 15);
}

// new
float FIR_GetValueF(FIR_CfgF_t *hCfg, circular_buf_F_t *circBufF, channel_t value) {
	uint32_t k;
	float y = 0.0f;
	channel_t data;

	// Dodanie próbki do bufora
	// Funkcja która dodaje dane do bufora kołowego
	circ_buf_F_push(circBufF, (value * PDSP_CODEC_mVres));

	// Suma iloczynów
	for (k = 0; k < hCfg->n; k++) {
		// Użycie funkcji pobierającej dane z bufora kołowego
		// przy każdym wywołaniu funkcji circ_buf_F_pop odczytywana jest kolejna wartość z bufora
		// aż do końca czyli do rzędu filtru
		y += ((hCfg->h[k]) * circ_buf_F_pop(circBufF, &data));
	}

	// Nie ma potrzeby przesuwania bufora ponieważ używany jest bufor kołowy
	// przy kolejnym wywoałanu funkcji rozpocznie się zapis danych do zwiększonego bufora
	// gdy funkcji wykona się tyle razy ile wynosi rząd filtru, bufor zostanie wyzerowany i procedura rozpocznie sie od poczatku

	return y;
}

// new
// Dodanie do parametrow funckji bufora cyklicznego
channel_t FIR_GetSampleF(FIR_CfgF_t *hCfg, circular_buf_F_t *circBufF, channel_t value) {
	return (channel_t) ((FIR_GetValueF(hCfg, circBufF, value)) / PDSP_CODEC_mVres);
}

float FILTER_GetValueFromSample(channel_t value) {
	return (float) (value * PDSP_CODEC_mVres);
}


void FILTER_ShiftSample(float *x, uint32_t n) {
	uint32_t i;
	for (i = (n - 1); i > 0; i--) {
		x[i] = x[i - 1];
	}
}

void FILTER_InitFirAvrF(float *h, uint32_t n){
	float avr = (1.0f / (float)n);
	for (uint32_t i = n; i > 0; i--) {
		*h = avr;
		h++;
	}
}

void FILTER_InitFirAvrI(channel_t *h, uint32_t n){
	channel_t avr = (channel_t)(128 / n);
	for (uint32_t i = n; i > 0; i--) {
			*h = avr;
			h++;
	}
}
