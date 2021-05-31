/*
 * MenuFilter.c
 *
 *  Created on: May 10, 2021
 *      Author: pdabal
 */

#include "Controller.h"
#include "pdsp/pdsp.h"

/*---------------------------------------------------------------------------------------------------------------------
 * Definicja pozycji w menu filtra
 *-------------------------------------------------------------------------------------------------------------------*/
MenuItem_t miFilterParam1 = { (uint8_t*) "Param. 1", NULL, MENU_PARAM_IS_NUM_INT, 0 };
MenuItem_t miFilterParam2 = { (uint8_t*) "Param. 2", NULL, MENU_PARAM_IS_NUM_INT, 0 };
MenuItem_t miFilterParam3 = { (uint8_t*) "Param. 3", NULL, MENU_PARAM_IS_STRING | MENU_CALL_IS_FUNCTION, (uint32_t) "FIR" };

//
Menu_t mFilter = { (uint8_t*) "Filter", .items = { &miFilterParam1, &miFilterParam2, &miFilterType, 0 }, .parent = NULL };
Menu_t *hFilterMenu = &mFilter;


