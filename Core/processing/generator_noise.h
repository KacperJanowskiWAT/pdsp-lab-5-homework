#ifndef PROCESSING_GENERATOR_NOISE_H_
#define PROCESSING_GENERATOR_NOISE_H_

#include "processing/generator.h"

typedef struct {
	long seed;
	float start;
	float stop;
} NOISE_Cfg_t;

typedef struct {
	uint32_t value;
	uint32_t key;
	uint8_t length;
} NOISE_CfgGalois_t;

typedef struct {
	int32_t call;
	float X1;
	float X2;
	float mean;
	float sigma;
} NOISE_CfgNormal_t;

void NOISE_Init(NOISE_Cfg_t *hRandUni, long seed, float start, float stop);
float NOISE_GetF(NOISE_Cfg_t *hCfg);

void NOISE_InitNormal(NOISE_CfgNormal_t *hCfg, float mean, float sigma);
float NOISE_GetNormalF(NOISE_CfgNormal_t *hCfg);

void NOISE_InitGalois(NOISE_CfgGalois_t *hRandLfsr, uint32_t seed, uint32_t key, uint8_t length);
uint32_t NOISE_GetGalois(NOISE_CfgGalois_t *hRandLfsr);

#endif /* PROCESSING_GENERATOR_NOISE_H_ */
