/*
 * Controller.c
 *
 *  Created on: 22 kwi 2021
 *      Author: pdabal
 */

#include "main.h"
#include "pdsp/pdsp.h"

extern Menu_t mGen;
extern Menu_t mFilter;
extern Menu_t mSound;

void MENU_Callback(void *m);

void MENU_Callback(void *m) {
	Menu_t *menu = ((Menu_t*) m);
	DisplayClear();
	DisplayStringXY("Item callback", 1, 1);
	DisplayStringXY("Index:", 2, 1);
	DisplayNumberXY(menu->selectedIndex, 2, 8);
	DisplayStringXY("Item name:", 2, 0);
}

MenuItem_t miMenuBaud = { (uint8_t*) "Baud", MENU_Callback, MENU_PARAM_IS_NUM_INT, 0 };
MenuItem_t miSubMenuGen = { (uint8_t*) "Generator", (void*) &mGen, MENU_CALLBACK_IS_SUBMENU, 0 };
MenuItem_t miSubMenuFilter = { (uint8_t*) "Filter", (void*) &mFilter, MENU_CALLBACK_IS_SUBMENU, 0 };
MenuItem_t miSubMenuSound = { (uint8_t*) "Sound", (void*) &mSound, MENU_CALLBACK_IS_SUBMENU, 0 };
//
Menu_t mMain = { (uint8_t*) "Main", .items = { &miMenuBaud, &miSubMenuGen, &miSubMenuFilter, &miSubMenuSound, 0 }, .parent = NULL };
Menu_t *hMainMenu = &mMain;

void MENU_Refresh(Menu_t *menu);

void MENU_Init(void) {
	uint32_t i = 0;
	Menu_t *menu[] = { &mMain, &mGen, &mFilter, &mSound, NULL };

	while (menu[i] != NULL) {
		menu[i]->menuItem = 0;
		menu[i]->lastMenuItem = -1;
		menu[i]->len = 0;

		MenuItem_t **iList = menu[i]->items;
		for (; *iList != 0; ++iList)
			menu[i]->len++;
		i++;
	}

	mGen.parent = (void*) &mMain;
	mFilter.parent = (void*) &mMain;
	mSound.parent = (void*) &mMain;

	// Inicjalizacja domyślnych wartości pozycji menu
	menu[0]->items[0]->parameter = PDSP_CODEC_BAUDRATE;

}

void MENU_Operation(Menu_t *menu, uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case JOY_DOWN_EXTI_LINE:
		(menu->menuItem != menu->len - 1) ? (menu->menuItem++) : (menu->menuItem = 0);
		break;
	case JOY_UP_EXTI_LINE:
		(menu->menuItem != 0) ? (menu->menuItem--) : (menu->menuItem = menu->len - 1);
		break;
	case JOY_LEFT_EXTI_LINE:
		menu->selectedIndex = 0;
		if (hMainMenu != (Menu_t*) menu->parent && menu->parent != NULL)
			hMainMenu = (Menu_t*) menu->parent;
		MENU_Operation(hMainMenu, 0);
		break;
	case JOY_RIGHT_EXTI_LINE:
	case JOY_OK_EXTI_LINE: {
		menu->selectedIndex = menu->menuItem;
		int flags = menu->items[menu->selectedIndex]->flags;

		if (flags & MENU_ITEM_IS_CHECKBOX) {
			menu->items[menu->selectedIndex]->flags ^= MENU_ITEM_IS_CHECKED;
			menu->lastMenuItem = -1;
		}
		if (flags & MENU_CALLBACK_IS_SUBMENU && menu->items[menu->selectedIndex]->callback) {
			hMainMenu = ((Menu_t*) menu->items[menu->selectedIndex]->callback);
			MENU_Operation(hMainMenu, 0);
		}
		if (flags & MENU_CALL_IS_FUNCTION && menu->items[menu->selectedIndex]->callback) {
			(*menu->items[menu->selectedIndex]->callback)(menu);
			menu->lastMenuItem = -1;
		}
		if ((menu->items[menu->selectedIndex]->callback == NULL) && ((flags & MENU_ITEM_IS_CHECKBOX) == 0)) {

		}
	}
		break;
	default:
		menu->lastMenuItem = -1;
		break;
	}
	MENU_Refresh(menu);
}

void MENU_Value(Menu_t *menu) {
	uint16_t value = BSP_POT_Read();
	switch (menu->menuItem) {
	case 0:
//		menu->items[0]->parameter = PDSP_CODEC_BAUDRATE;
		break;
	case 1:
		value = (uint16_t) GET_AMPLITUDE();
		BSP_SEG_Display(value);
		break;
	case 2:
		value = (uint16_t) GET_FREQUENCY();
		BSP_SEG_Display(value);
		break;
	case 3:
		value = (uint16_t) GET_PHASE();
		BSP_SEG_Display(value);
		break;
	default:
		BSP_SEG_Display(10000);
		break;
	}
}

void MENU_Refresh(Menu_t *menu) {
	// If menu item changed -> refresh screen
	if (menu->lastMenuItem != menu->menuItem) {
		uint32_t index = menu->menuItem;
		DisplayClear();
		DisplayStringXY(menu->title, 1, 1);
		DisplayStringXY((menu->items[index]->text), 2, 1);

		int posx = strlen((char*) menu->items[index]->text) + 2;
		switch (menu->items[index]->flags & MENU_PARAMETER_MASK) {
		case MENU_PARAM_IS_NUM_INT:
			DisplayNumberXY(menu->items[index]->parameter, 2, posx)
			;
			break;
		case MENU_PARAM_IS_NUM_FLOAT:
			DisplayNumberFixXY(menu->items[index]->parameter, 2, posx)
			;
			break;
		case MENU_PARAM_IS_STRING:
			DisplayStringXY(menu->items[index]->parameter, 2, posx)
			;
			break;
		}

		if (menu->items[index]->flags & MENU_ITEM_IS_CHECKBOX) {
			if (menu->items[index]->flags & MENU_ITEM_IS_CHECKED) {
				DisplayCharXY(0x78, 2, posx);
			} else {
				DisplayCharXY(0xDB, 2, posx);
			}
		}
		menu->lastMenuItem = menu->menuItem;
	}
}

