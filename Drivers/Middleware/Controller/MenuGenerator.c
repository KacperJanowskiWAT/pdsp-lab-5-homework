/*
 * MenuGenerator.c
 *
 *  Created on: 6 maj 2021
 *      Author: pdabal
 */

#include "Controller.h"
#include "pdsp/pdsp.h"


static void CallbackGen(void *m);

uint8_t GenCtrlSelect = 0;
GenControl_t GenCtrl1 = { 0 };
GenControl_t GenCtrl2 = { 0 };
GenControl_t *GenControls[] = { &GenCtrl1, &GenCtrl2 };
/*---------------------------------------------------------------------------------------------------------------------
 * Definicja pozycji w menu generatora
 *-------------------------------------------------------------------------------------------------------------------*/
// @formatter:off
//
MenuItem_t miGenNumber = { (uint8_t*) "Number", CallbackGen, MENU_PARAM_IS_NUM_INT | MENU_CALL_IS_FUNCTION, 0 };
MenuItem_t miGenAmplitude = { (uint8_t*) "Ampl.", CallbackGen, MENU_PARAM_IS_NUM_INT | MENU_CALL_IS_FUNCTION, 1000 };
MenuItem_t miGenFrequency = { (uint8_t*) "Freq.", CallbackGen, MENU_PARAM_IS_NUM_INT | MENU_CALL_IS_FUNCTION, 100 };
MenuItem_t miGenPhase = { (uint8_t*) "Phase", CallbackGen, MENU_PARAM_IS_NUM_INT | MENU_CALL_IS_FUNCTION, 0 };
MenuItem_t miGenType =
		{ (uint8_t*) "Type", CallbackGen, MENU_PARAM_IS_STRING | MENU_CALL_IS_FUNCTION, (uint32_t) "SIN" };
MenuItem_t miGenOutput = { (uint8_t*) "Output", CallbackGen, MENU_PARAM_IS_STRING | MENU_CALL_IS_FUNCTION,
		(uint32_t) "Off" };
//
Menu_t mGen = {
		(uint8_t*) "Generator",
		.items = { &miGenNumber, &miGenAmplitude, &miGenFrequency, &miGenPhase, &miGenType, &miGenOutput, 0 },
		.parent = NULL
};

Menu_t *hGenMenu = &mGen;
//
// @formatter:on

static void CallbackGen(void *m) {

	Menu_t *menu = ((Menu_t*) m);
	int32_t index = menu->selectedIndex;
	MenuItem_t *selectedItem = menu->items[menu->selectedIndex];

	switch (index) {
	case 0:
		GenCtrlSelect++;
		if(GenCtrlSelect > 2)
			GenCtrlSelect = 0;
		selectedItem->parameter = GenCtrlSelect;
		break;
	case 1:
		GenControls[GenCtrlSelect]->amplitude = GET_AMPLITUDE();
		selectedItem->parameter = (uint32_t) GenControls[GenCtrlSelect]->amplitude;
		break;
	case 2:
		GenControls[GenCtrlSelect]->frequency = GET_FREQUENCY();
		selectedItem->parameter = (uint32_t) GenControls[GenCtrlSelect]->frequency;
		break;
	case 3:
		GenControls[GenCtrlSelect]->phase = GET_PHASE();
		selectedItem->parameter = (uint32_t) GenControls[GenCtrlSelect]->phase;
		break;
	case 4:
		GenControls[GenCtrlSelect]->type++;
		if (GenControls[GenCtrlSelect]->type == GEN_CONTROL_TYPE_NUM)
			GenControls[GenCtrlSelect]->type = 0;
		switch (GenControls[GenCtrlSelect]->type) {
		case 0:
			selectedItem->parameter = (uint32_t) "SIN";
			break;
		case 1:
			selectedItem->parameter = (uint32_t) "TRIANGLE";
			break;
		case 2:
			selectedItem->parameter = (uint32_t) "SQUARE";
			break;
		case 3:
			selectedItem->parameter = (uint32_t) "SAWTOOTH";
			break;
		}
		GenControls[GenCtrlSelect]->type = (uint16_t) (selectedItem->parameter);
		break;
	case 5:
		if (selectedItem->parameter == (uint32_t) "Off") {
			selectedItem->parameter = (uint32_t) "On";
			GenControls[GenCtrlSelect]->status = 1;
		} else {
			selectedItem->parameter = (uint32_t) "Off";
			GenControls[GenCtrlSelect]->status = 0;
		}
		break;
	}
}

