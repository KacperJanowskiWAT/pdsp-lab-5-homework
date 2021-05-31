#ifndef SOUND_H_
#define SOUND_H_

#include "processing/generator.h"

// Do zapisu symbolicznego nuty - dźwięk
typedef enum {
	C = 0, Cb = 1, D = 2, Db = 3, E = 4, F = 5, Fb = 6, G = 7, Gb = 8, A = 9, Ab = 10, H = 11, N = 12
} NoteName_t;

// Do zapisu symbolicznego nuty - dźwięk i czas trwania
typedef struct {
	NoteName_t note;		// Symbol nuty
	float duration;		// Odwrotność czasu trwania nuty np. dla ćwierć nuty wynosi 4
} Note_t;

// Konfiguracja modułu generowania dźwięków
typedef struct {
	// Konfiguracja generatora
	float amplitude;			// Określa poziom maksymalny sygnału w mV
	float bpm;					// Tempo grania nut wyrażona w uderzeniach na minutę - ćwierć nuta trwa 60/bpm, np. dla BPM 120 -> (60 [s] / 120) = 0,5 [s]
	uint8_t octave;				// Wybrana oktawa
	// Informacja o melodii
	Note_t *notes;				// Wskazuje nuty i czasy ich trwania
	uint32_t length;			// Liczba nut w tablicy notes
	// Informacja z odtwarzania melodii
	uint32_t noteIndex;			// Numer nuty
	uint32_t noteSample;		// Liczba próbek danej nuty
	uint32_t noteSampleLength;	// Czas trwania nuty wyrażony w liczbie próbek
	float tone[12];				// Tablica z częstotliwościami
	//
	uint32_t gateSample;					// Czas trwania zmiany amplitudy
	//
	OSC_Cfg_t ToneGen;			// Generator sygnału
} SOUND_Cfg_t;

extern Note_t notes[];
extern uint8_t notesLength;

void SOUND_InitTone(float *tone, uint8_t octave);
void SOUND_Init(SOUND_Cfg_t *hCfg, Note_t *notes, uint32_t length, float amplitude, float bpm, float gate, uint8_t octave);
float SOUND_GetSampleF(SOUND_Cfg_t *hCfg);
channel_t SOUND_GetSample(SOUND_Cfg_t *hCfg);

#endif /*SOUND_H_*/
