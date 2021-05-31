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
#include "processing/generator.h"
#include "processing/sound.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

OSC_Cfg_t OscCfg;
SOUND_Cfg_t sound;


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

 	PDSP_Init();
	MENU_Init();
	MENU_Operation(hMainMenu, 0);

	OSC_Init(&OscCfg, OSC_Sinusoid, 1000, 100, 0, 25);
	SOUND_Init(&sound, notes, notesLength, 750, 120, 10, 4);

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
//				DataOut.channel[LEFT] = OSC_GetValue(&OscCfg);
				DataOut.channel[LEFT] = SOUND_GetSample(&sound);
				AD_Off(DEBUG_PROCESSING_IO);
			}
		}
		AD_Off(DEBUG_MAIN_LOOP_IO);

	}
}

void CODEC_IRQHandler(void){
	DataIn = CODEC_GetSample();   		// Odczytanie nowej próbki od kodeka
//	DataOut = DataIn;			        // Przetwarzanie - opcjonalnie w przerwaniu
	CODEC_SetSample(DataOut);     		// Wysłanie próbki do kodeka
	DataNew = true;             		// Ustawienie flagi obecności nowych danych
	SampleNumber++;
}
