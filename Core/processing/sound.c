#include "pdsp/pdsp.h"
#include "processing/sound.h"

#include <math.h>

// @formatter:off
// Skala chromatyczna majorowa
const float tone[9][12] = {
/* 		 	C		 C#			D		  D#		E		  F			F#		  G			G#		  A			A#		  H 	*/
/* 0 */{   16.35f, 	17.32f,    18.35f,   19.45f,   20.60f,   21.83f,   23.12f, 	 24.50f,   25.96f, 	 27.50f,   29.14f, 	 30.87f },
/* 1 */{   32.70f, 	34.65f,    36.71f,   38.89f,   41.20f,   43.65f,   46.25f, 	 49.00f,   51.91f, 	 55.00f,   58.27f, 	 61.74f },
/* 2 */{   65.41f, 	69.30f,    73.42f,   77.78f,   82.41f,   87.31f,   92.50f, 	 98.00f,  103.83f,  110.00f,  116.54f,  123.47f },
/* 3 */{  130.81f, 	138.59f,  146.83f,  155.56f,  164.81f,  174.61f,  185.00f,  196.00f,  207.65f,  220.00f,  233.08f,  246.94f },
/* 4 */{  261.63f, 	277.18f,  293.66f,  311.13f,  329.63f,  349.23f,  369.99f,  392.00f,  415.30f,  440.00f,  466.16f,  493.88f },
/* 5 */{  523.25f, 	554.37f,  587.33f,  622.25f,  659.26f,  698.46f,  739.99f,  783.99f,  830.61f,  880.00f,  932.33f,  987.77f },
/* 6 */{ 1046.50f, 1108.73f, 1174.66f, 1244.51f, 1318.51f, 1396.91f, 1479.98f, 1567.98f, 1661.22f, 1760.00f, 1864.66f, 1975.53f },
/* 7 */{ 2093.00f, 2217.46f, 2349.32f, 2489.02f, 2637.02f, 2793.83f, 2959.96f, 3135.96f, 3322.44f, 3520.00f, 3729.31f, 3951.07f },
/* 8 */{ 4186.01f, 4434.92f, 4698.64f, 4978.03f, 5274.04f, 5587.65f, 5919.91f, 6271.93f, 6644.88f, 7040.00f, 7458.62f, 7902.13f }
};
// @formatter:on

// @formatter:off
// Utwór do tesotwania
Note_t notes[] = {  { G, 4 }, { E, 4 }, { E, 4 }, 	{ F, 4 }, { D, 4 }, { D, 4 }, 	{ C, 8 }, { E, 8 }, { G, 2 },
					{ G, 4 }, { E, 4 }, { E, 4 }, 	{ F, 4 }, { D, 4 }, { D, 4 },	{ C, 8 }, { E, 8 }, { C, 2 },
					{ N, 1 }, { N, 1 }, { N, 1 } };
uint8_t notesLength = 21;
// @formatter:on

/**
 *  Konfiguracja odtwarzacza melodii
 */
void SOUND_Init(SOUND_Cfg_t *hCfg, Note_t *notes, uint32_t length, float amplitude, float bpm, float gate,
		uint8_t octave) {
	hCfg->bpm = (4.0f / (bpm / 60.0f));
	hCfg->notes = notes;
	hCfg->length = length;
	hCfg->amplitude = amplitude;
	hCfg->octave = octave;

	hCfg->gateSample = (uint32_t) (PDSP_CODEC_Fs * (gate / 1000));
	hCfg->noteIndex = 0;
	hCfg->noteSample = 0;

	// Wyznaczenie liczby próbek czasu trwania nuty
	Note_t note = hCfg->notes[hCfg->noteIndex];
	float noteDurationTime = (hCfg->bpm / note.duration);
	hCfg->noteSampleLength = (uint32_t) (PDSP_CODEC_Fs * noteDurationTime);

	//// Konfiguracja generatora
	//	 Wyznaczenie częstotliwości dla podanej oktawy
	SOUND_InitTone(hCfg->tone, octave);
	//	 Inicjalizacja generatora
//	OSC_Init(&hCfg->ToneGen, OSC_Sinusoid, 1.0f, hCfg->tone[note.note], 0, 50);
	OSC_Init(&hCfg->ToneGen, OSC_Sinusoid, 1.0f, tone[hCfg->octave][note.note], 0, 50);
}

/**
 *  Generowanie kolejnych próbek skonfigurowanej melodii
 */
float SOUND_GetSampleF(SOUND_Cfg_t *hCfg) {
	float wave = 1.0f;

	if (hCfg->gateSample > 0.0f) {
		if (hCfg->noteSample < hCfg->gateSample) {
			float phase = ((float) hCfg->noteSample / (float) hCfg->gateSample) * PDSP_PI_DIV_2;
			wave = pdsp_sinf(phase);
			wave *= wave;
		} else if ((hCfg->noteSampleLength - hCfg->noteSample) < hCfg->gateSample) {
			float phase = ((float) (hCfg->noteSampleLength - hCfg->noteSample) / (float) hCfg->gateSample)
					* PDSP_PI_DIV_2;
			wave = pdsp_sinf(phase);
			wave *= wave;
		} else {
			wave = 1.0f;
		}
	}

	wave *= OSC_GetValueF(&hCfg->ToneGen);

	hCfg->noteSample++;
	if (hCfg->noteSample == hCfg->noteSampleLength) {
		hCfg->noteSample = 0;
		hCfg->noteIndex++;								// Kolejna nuta
		if (hCfg->noteIndex == hCfg->length) {
			hCfg->noteIndex = 0;						// Zapętlenie melodii
		}
		// Wyznaczenie liczby próbek kolejnej nuty
		Note_t note = hCfg->notes[hCfg->noteIndex];
		float noteDurationTime = (hCfg->bpm / note.duration);
		hCfg->noteSampleLength = (uint32_t) (PDSP_CODEC_Fs * noteDurationTime);
		// Zmiana częstotliwości generatora
		if (note.note != N) {
//			OSC_SetFrequency(&hCfg->ToneGen, hCfg->tone[note.note]);
			OSC_SetFrequency(&hCfg->ToneGen, tone[hCfg->octave][note.note]);
		} else {
			hCfg->ToneGen.phaseStep = 0.0f;
			hCfg->ToneGen.phase = 0.0f;
		}
	}

	return wave;
}

/*
 * Wyznaczenie rzeczywistej wartości próbki melodii
 * */
channel_t SOUND_GetSample(SOUND_Cfg_t *hCfg) {
	return (channel_t) ((hCfg->amplitude * SOUND_GetSampleF(hCfg)) / PDSP_CODEC_mVres);
}

/**
 * Wyznaczenie tablicy tonów dla podanej oktawy w skali chromatycznej majorowej
 */
void SOUND_InitTone(float *tone, uint8_t octave) {
	float ratio = powf(2.0f, 1.0f / 12.0f);
	for (uint32_t i = 0; i < 12; i++) {
		*tone = powf(ratio, (octave * 12 + i) - 57.0f) * 440.0f;
		tone++;
	}
}

