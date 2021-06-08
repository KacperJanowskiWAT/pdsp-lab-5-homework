/*
 * filters.h
 *
 *  Created on: Jun 2, 2021
 *      Author: pdabal/Kacper Janowski
 */

#ifndef _FILTERS_H_
#define _FILTERS_H_

#include "processing/circular_buffer.h"

typedef struct {
    float * h;          // Współczynniki filtru FIR
    float * x;          // Próbki sygnału
    uint32_t n;         // Rząd filtru
}FIR_CfgF_t;

typedef struct {
    channel_t * h;      // Współczynniki filtru FIR
    channel_t * x;      // Próbki sygnału
    uint32_t n;         // Rząd filtru
}FIR_CfgI_t;


void FIR_InitF(FIR_CfgF_t *hCfg, circular_buf_F_t *circBufF, float *x, float *h, uint32_t n);
float FIR_GetValueF(FIR_CfgF_t *hCfg, circular_buf_F_t *circBufF, channel_t value);
channel_t FIR_GetSampleF(FIR_CfgF_t *hCfg, circular_buf_F_t *circBufF, channel_t value);

void FIR_InitI(FIR_CfgI_t *hCfg, circular_buf_I_t *circBufI, channel_t *x, channel_t *h, uint32_t n);
channel_t FIR_GetSampleI(FIR_CfgI_t *hCfg, circular_buf_I_t *circBufI, channel_t value);


float FILTER_GetValueFromSample(channel_t value);
void FILTER_AVR_GetParameters(float * h, uint32_t n);
void FILTER_ShiftSample(float * x, uint32_t n);

void FILTER_InitFirAvrF(float *h, uint32_t n);
void FILTER_InitFirAvrI(channel_t *h, uint32_t n);

#endif /* _FILTERS_H_ */
