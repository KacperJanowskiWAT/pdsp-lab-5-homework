/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "pdsp/pdsp.h"
#include "processing/filters.h"
#include "processing/generator.h"
#include "processing/generator_sweep.h"

#include "code_measure.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

OSC_Cfg_t OscCfg;		// Oscylator bazowy
SWEEP_Cfg_t SweepCfg;	// Generatora z przemiataniem częstotliwości

// Struktura konfiguracji statystyk
CM_Stat_t StatSweep, StatFirI, StatFirF;
// Statystyki czasu realizacji monitorowanej operacji
float avr, min, max;

// Struktury konfiguracji filtru FIR
FIR_CfgF_t firAvrF;
FIR_CfgI_t firAvrI;

circular_buf_F_t circBufF;
circular_buf_I_t circBufI;

// Rząd filtru uśredniającego
#define  firAvrLength 				 18	// 71980

// Bufory dla współczynników i próbek
float hF[firAvrLength];
float xF[firAvrLength];
channel_t hI[firAvrLength];
channel_t xI[firAvrLength];

// Zmienne pomocnicze do przechowywania wartości próbek
channel_t x, yI, yF;

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	PDSP_Init();
	MENU_Init();
	MENU_Operation(hMainMenu, 0);

	// Inicjalizacja generatora z przemiataniem częstotliwości
	OSC_Init(&OscCfg, OSC_Sinusoid, 1000, 0, 0, 0);									// Amplituda 1000 [mV]
	SWEEP_Init(&SweepCfg, &OscCfg, 0, PDSP_CODEC_Fs / 2, PDSP_CODEC_Fs / 1000, LINEAR);	// Przemiatanie od 0 do Fs/2 [Hz] liniowo przez czas Fs/1000 [s]

	// Konfiguracja filtrów FIR
	FILTER_InitFirAvrF(hF, firAvrLength);
	FILTER_InitFirAvrI(hI, firAvrLength);

	FIR_InitF(&firAvrF, &circBufF, xF, hF, firAvrLength);
	FIR_InitI(&firAvrI, &circBufI, xI, hI, firAvrLength);

	// Konfiguracja pomiaru czasu realizacji programu
	CM_Init();								// Konfiguracja modułu do wykonania pomiarów
	CM_StatInit(&StatSweep, PDSP_CODEC_Fs);	// Inicjalizacja modułu do wyznaczania statystyk
	CM_StatInit(&StatFirI, PDSP_CODEC_Fs);	// Inicjalizacja modułu do wyznaczania statystyk
	CM_StatInit(&StatFirF, PDSP_CODEC_Fs);	// Inicjalizacja modułu do wyznaczania statystyk

	while (1) {
		AD_On(DEBUG_MAIN_LOOP_IO);
		if (PDSP_MODE == PDSP_MODE_POLL) {
			DataIn = CODEC_GetSample();  	// Odczytanie nowej próbki od kodeka
			CODEC_SetSample(DataOut);  		// Wysłanie próbki do kodeka
			DataOut = DataIn;               // Przetwarzanie w trybie odpytywania
		} else {
			if (DataNew == true) {
				DataNew = false;
				AD_On(DEBUG_PROCESSING_IO);
//				DataOut = DataIn;           // Przetwarzanie w trybie przerwania
				CM_Start(0);				// Rozpoczęcie pomiaru odcinka 0
				x = SWEEP_GetSample(&SweepCfg);
				CM_Delta(0);				// Zakończenie pomiaru odcinka 0
				CM_Start(1);				// Rozpoczęcie pomiaru odcinka 1
				yI = FIR_GetSampleI(&firAvrI, &circBufI, x);
				CM_Delta(1);				// Zakończenie pomiaru odcinka 1
				CM_Start(2);				// Rozpoczęcie pomiaru odcinka 2
				yF = FIR_GetSampleF(&firAvrF, &circBufF, x);
				CM_Delta(2);				// Zakończenie pomiaru odcinka 2

				DataOut.channel[LEFT] = x;
				DataOut.channel[RIGHT] = yI;

				// Zbieranie statystyk
				if (CM_StatPut(&StatSweep, CM_DeltaGet(0))) {			// Aktualizacja statystyk
					CM_StatGetTime(&StatFirI, &avr, &max, &min, CM_us);	// Aktualizacja zmiennych wyjściowych
				}
				if (CM_StatPut(&StatFirI, CM_DeltaGet(1))) {			// Aktualizacja statystyk
					CM_StatGetTime(&StatFirI, &avr, &max, &min, CM_us);	// Aktualizacja zmiennych wyjściowych
				}
				if (CM_StatPut(&StatFirF, CM_DeltaGet(2))) {			// Aktualizacja statystyk
					CM_StatGetTime(&StatFirF, &avr, &max, &min, CM_us);	// Aktualizacja zmiennych wyjściowych
				}
				AD_Off(DEBUG_PROCESSING_IO);
			}
		}
		AD_Off(DEBUG_MAIN_LOOP_IO);
	}
}

void CODEC_IRQHandler(void) {
	DataIn = CODEC_GetSample();   		// Odczytanie nowej próbki od kodeka
//	DataOut = DataIn;			        // Przetwarzanie - opcjonalnie w przerwaniu
	CODEC_SetSample(DataOut);     		// Wysłanie próbki do kodeka
	DataNew = true;             		// Ustawienie flagi obecności nowych danych
	SampleNumber++;
}
