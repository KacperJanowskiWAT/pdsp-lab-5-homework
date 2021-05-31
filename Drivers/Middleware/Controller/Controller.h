/*
 * Controller.h
 *
 *  Created on: 22 kwi 2021
 *      Author: pdabal
 */

#ifndef MIDDLEWARE_CONTROLLER_CONTROLLER_H_
#define MIDDLEWARE_CONTROLLER_CONTROLLER_H_

#include "Kameleon.h"

// 0-3 bits
#define MENU_PARAMETER_MASK 		0x0F
#define MENU_PARAM_IS_NUM_INT 		0x01
#define MENU_PARAM_IS_NUM_FLOAT		0x02
#define MENU_PARAM_IS_STRING 		0x04
// 4 - checkbox bit
#define MENU_ITEM_IS_CHECKBOX		0x10
// 5
#define MENU_ITEM_IS_CHECKED		0x20
// 6 - submenu bit
#define MENU_CALLBACK_IS_SUBMENU	0x40
// 7 - callback bit
#define MENU_CALL_IS_FUNCTION 		0x80

// Number of items on one screen - not including title
#define MENU_LINES 1
// Symbol which is displayed in front of the selected item. This symbol doesn't appear when MENU_LINES == 1
#define ARROW_SYMBOL ">"
// How many spaces is between arrow symbol and menu item useful to set zero on smaller displays
#define ARROW_GAP 1

// Clear display
#define DisplayClear()					 	 	 BSP_LCD_Clear()
#define DisplayClearLine(posx)					{BSP_LCD_GoTo(posx, 1); BSP_LCD_WriteText((uint8_t *)"                ");}

#define DisplayCharXY(ch, posx, posy) 			{BSP_LCD_GoTo(posx, posy); BSP_LCD_WriteChar((uint8_t)ch);}
// Display string
#define DisplayString(str) 						{BSP_LCD_WriteText((uint8_t *)str);}
#define DisplayStringXY(str, posx, posy) 		{BSP_LCD_GoTo(posx, posy); BSP_LCD_WriteText((uint8_t *)str);}
// Display number
#define DisplayNumberXY(num, posx, posy) 		{BSP_LCD_GoTo(posx, posy); BSP_LCD_WriteNumber(num);}
#define DisplayNumberFixXY(num, posx, posy) 	{BSP_LCD_GoTo(posx, posy); BSP_LCD_WriteNumberFloat(num, 1, 1);}

#define GEN_CONTROL_TYPE_NUM		4

typedef struct {
	uint8_t *text;
	void (*callback)(void*);
	uint32_t flags;
	uint32_t parameter;
	uint8_t * unit;
} MenuItem_t;


typedef struct {
	uint8_t *title;
	int32_t selectedIndex;
	int32_t refresh;
	int32_t len;
	uint8_t menuItem;
	uint8_t lastMenuItem;
	void * parent;
	MenuItem_t *items[];
} Menu_t;


typedef struct {
	float amplitude;
	float frequency;
	float phase;
	uint16_t type;
	uint16_t status;
} GenControl_t;

extern Menu_t * hMainMenu;

#define	GET_AMPLITUDE()		(float)((PDSP_CODEC_Vpp / 0.002f / BSP_POT_Bres) * BSP_POT_Read())
#define GET_FREQUENCY()		(float)((PDSP_CODEC_Fs / 2.0f / BSP_POT_Bres) * BSP_POT_Read())
#define GET_PHASE()			(float)((PDSP_2PI / BSP_POT_Bres) * BSP_POT_Read())


void MENU_Init(void);
void MENU_Operation(Menu_t *menu, uint16_t GPIO_Pin);
void MENU_Value(Menu_t *menu);

#endif /* MIDDLEWARE_CONTROLLER_CONTROLLER_H_ */
