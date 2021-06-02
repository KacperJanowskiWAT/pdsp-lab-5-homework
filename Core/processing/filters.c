/*
 * filters.c
 *
 *  Created on: 31 maj 2021
 *      Author: pdabal
 */

#include "pdsp/pdsp.h"
#include "processing/filters.h"

void FIR_InitF(FIR_CfgF_t *hCfg, float *x, float *h, uint32_t n) {
	hCfg->x = x;
	hCfg->h = h;
	hCfg->n = n;

	uint32_t i;
	for (i = n; i > 0; i--) {	// Zerowanie bufora
		*x = 0.0f;
		x++;
	}
}

void FIR_InitI(FIR_CfgI_t *hCfg, channel_t *x, channel_t *h, uint32_t n) {
	hCfg->x = x;
	hCfg->h = h;
	hCfg->n = n;

	uint32_t i;
	for (i = n; i > 0; i--) {	// Zerowanie bufora
		*x = 0;
		x++;
	}
}

channel_t FIR_GetSampleI(FIR_CfgI_t *hCfg, channel_t value) {
	uint32_t k;
	int32_t y = 0;

	// Dodanie próbki do bufora
	hCfg->x[0] = value;

	// Suma iloczynów
	for (k = 0; k < hCfg->n; k++) {
		y += (hCfg->h[k]) * (hCfg->x[k]);
	}

	// Przesunięcie bufora
	for (k = (hCfg->n - 1); k > 0; k--) {
		hCfg->x[k] = hCfg->x[k - 1];
	}

	if (PDSP_SAMPLE_SIZE == 1)
		return (channel_t) (y >> 7);
	else
		return (channel_t) (y >> 15);
}

float FIR_GetValueF(FIR_CfgF_t *hCfg, channel_t value) {
	uint32_t k;
	float y = 0.0f;

	// Dodanie próbki do bufora
	hCfg->x[0] = value * PDSP_CODEC_mVres;

	// Suma iloczynów
	for (k = 0; k < hCfg->n; k++) {
		y += (hCfg->h[k]) * (hCfg->x[k]);
	}

	// Przesunięcie bufora
	for (k = (hCfg->n - 1); k > 0; k--) {
		hCfg->x[k] = hCfg->x[k - 1];
	}

	return y;
}

channel_t FIR_GetSampleF(FIR_CfgF_t *hCfg, channel_t value) {
	return (channel_t) ((FIR_GetValueF(hCfg, value)) / PDSP_CODEC_mVres);
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
