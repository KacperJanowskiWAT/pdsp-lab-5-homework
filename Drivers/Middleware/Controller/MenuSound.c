/*
 * MenuSound.c
 *
 *  Created on: May 27, 2021
 *      Author: pdabal
 */

#include "Controller.h"
#include "pdsp/pdsp.h"

static void CallbackSound(void *m);

/*---------------------------------------------------------------------------------------------------------------------
 * Definicja pozycji w menu filtra
 *-------------------------------------------------------------------------------------------------------------------*/
MenuItem_t miSoundParam1 = { (uint8_t*) "Amplitude", CallbackSound, MENU_PARAM_IS_NUM_INT, 0 };
MenuItem_t miSoundParam2 = { (uint8_t*) "BPM", CallbackSound, MENU_PARAM_IS_NUM_INT, 0 };
MenuItem_t miSoundParam3 = { (uint8_t*) "Play", CallbackSound, MENU_PARAM_IS_STRING | MENU_CALL_IS_FUNCTION, (uint32_t) "FIR" };

//
Menu_t mSound = { (uint8_t*) "Sound", .items = { &miSoundParam1, &miSoundParam2, &miSoundParam3, 0 }, .parent = NULL };
Menu_t *hSoundMenu = &mSound;

#define BPM_MAX					 720
#define GET_BPM()		(float)((BPM_MAX / BSP_POT_Bres) * BSP_POT_Read())

static void CallbackSound(void *m) {
	Menu_t *menu = ((Menu_t*) m);
	int32_t index = menu->selectedIndex;
	MenuItem_t *selectedItem = menu->items[menu->selectedIndex];

	switch (index) {
	case 0:
		selectedItem->parameter = (uint32_t) GET_AMPLITUDE();
		break;
	case 1:
		selectedItem->parameter = (uint32_t) GET_BPM();
		break;
	case 2:
		if (selectedItem->parameter == (uint32_t) "Play") {
			selectedItem->parameter = (uint32_t) "Stop";
		} else {
			selectedItem->parameter = (uint32_t) "Play";
		}
		break;
	case 3:
		break;
	}
}

