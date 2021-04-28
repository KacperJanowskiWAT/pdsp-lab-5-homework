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
#include "processing/processing.h"

#include "Controller.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	PDSP_Init();

	while (1) {
		AD_On(DEBUG_MAIN_LOOP_IO);
		if (PDSP_MODE == PDSP_MODE_POLL) {
			DataIn = CODEC_GetSample();  	// Odczytanie nowej próbki od kodeka
			CODEC_SetSample(DataOut);  		// Wysłanie próbki do kodeka
			DataOut = DataIn;
		} else {
			if (DataNew == true) {
				DataNew = false;
				DataOut = DataIn;
				AD_Off(DEBUG_PROCESSING_IO);
				MENU_Operation(&hMenu, (uint16_t)BSP_JOY_GetCurrentState());
			}
		}
		AD_Off(DEBUG_MAIN_LOOP_IO);
	}
}

void CODEC_IRQHandler(void){
	DataIn = CODEC_GetSample();   		// Odczytanie nowej próbki od kodeka
	CODEC_SetSample(DataOut);     		// Wysłanie próbki do kodeka

	DataNew = true;             		// Ustawienie flagi obecności nowych danych
	SampleNumber++;
}
