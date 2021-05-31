#include "processing/generator_noise.h"

void NOISE_Init(NOISE_Cfg_t *hCfg, long seed, float start, float stop) {
	hCfg->seed = seed;
	hCfg->start = start;
	hCfg->stop = stop;
}

float NOISE_GetF(NOISE_Cfg_t *hCfg) {
	static uint32_t a = 16807; /* a = 7^5 */
	static uint32_t m = 2147483647; /* m = 2^31 - 1 */
	static uint32_t q = 127773; /* q = m / a = quotient */
	static uint32_t r = 2836; /* r = m % a = remainder */

	float y;

	hCfg->seed = a * (hCfg->seed % q) - r * (hCfg->seed / q); /* update seed */
	if (hCfg->seed < 0) /* wrap to positive values */
		hCfg->seed += m;

	y = (float) hCfg->seed / (float) m;   // [0   1)
	y *= (hCfg->stop - hCfg->start);
	y += hCfg->start;

	return y;
}

void NOISE_InitNormal(NOISE_CfgNormal_t *hCfg, float mean, float sigma) {
	hCfg->mean = mean;
	hCfg->sigma = sigma;
	hCfg->call = 0;
	hCfg->X1 = 0.0;
	hCfg->X2 = 0.0;
}

float NOISE_GetNormalF(NOISE_CfgNormal_t *hRandNorm) {
	float U1, U2, W, mult;

	if (hRandNorm->call == 1) {
		hRandNorm->call = !(hRandNorm->call);
		return (hRandNorm->mean + hRandNorm->sigma * hRandNorm->X2);
	}

	do {
		U1 = -1 + ((float) rand() / RAND_MAX) * 2;
		U2 = -1 + ((float) rand() / RAND_MAX) * 2;
		W = powf(U1, 2) + powf(U2, 2);
	} while (W >= 1 || W == 0);

	mult = sqrt((-2 * logf(W)) / W);
	hRandNorm->X1 = U1 * mult;
	hRandNorm->X2 = U2 * mult;

	hRandNorm->call = !(hRandNorm->call);

	return (hRandNorm->mean + hRandNorm->sigma * hRandNorm->X1);
}

void NOISE_InitGalois(NOISE_CfgGalois_t *hCfg, uint32_t seed, uint32_t key, uint8_t length) {
	hCfg->key = key;
	hCfg->value = seed;
	hCfg->length = length;
}

uint32_t NOISE_GetGalois(NOISE_CfgGalois_t *hCfg) {
	uint32_t lsb;

	hCfg->value &= ((1 << hCfg->length) - 1);
	lsb = hCfg->value & 1;
	hCfg->value >>= 1;
	if (lsb)
		hCfg->value ^= hCfg->key;

	return hCfg->value;
}
