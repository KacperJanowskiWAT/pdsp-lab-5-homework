#ifndef __KAMELEON_DISP_H_
#define __KAMELEON_DISP_H_
/* Includes ------------------------------------------------------------------*/
/* Exported Types ------------------------------------------------------------*/
/* Exported Constants --------------------------------------------------------*/
#define SEG_IRQ_Priority						15

/* Exported Functions --------------------------------------------------------*/
void 	BSP_SEG_Init(void);
uint32_t BSP_SEG_Display(int16_t value);
uint32_t BSP_SEG_DisplayFixedPoint(uint16_t value, uint8_t fraction_digits);
void 	BSP_SEG_Callback(void);

/*----------------------------------------------------------------------------*/
#define 	LCD_BKL_PRESCALER			800
#define 	LCD_BKL_PERIOD				100
#define 	LCD_BKL_TRESHOLD			(LCD_BKL_PERIOD / 2)

#define 	LCD_BKL_PWM					1

#define 	LCD_ROWS 					2
#define 	LCD_COLS 					16

#define 	LCD_LINE1 					0x00
#define 	LCD_LINE2 					0x40
/* Exported Functions --------------------------------------------------------*/
/* Initialize HD44780 2x16 LCD display.
 * The function should be called before using any other LCD display function.
 * It initializes GPIO lines and performs HD44780 initialization sequence. */
void BSP_LCD_Init(void);
/* Turn on LDC backlight. */
void BSP_LCD_BacklightOn(void);
/* Turn off LDC backlight. */
void BSP_LCD_BacklightOff(void);
void BSP_LCD_BacklightSet(uint16_t value);
/* Clear the LCD display. */
void BSP_LCD_Clear(void);

/* Set Cursor to a specified location given by row and column information
 * @param  Row Number (1 to 2)
 * @param  Column Number (1 to 16) Assuming a 2 X 16 characters display */
void BSP_LCD_GoTo(uint8_t row, uint8_t column);

void BSP_LCD_WriteChar(uint8_t ch);
void BSP_LCD_WriteText(uint8_t *text);
void BSP_LCD_WriteNumber(uint32_t number);
void BSP_LCD_WriteNumberHex(uint32_t number);
void BSP_LCD_WriteNumberFloat(float number, uint32_t digit, uint32_t fraction);
void BSP_LCD_WriteBinary(uint32_t var, uint8_t bitCount);

void BSP_LCD_SetUserChar(int8_t charNum, uint8_t n, const int8_t *p);

void BSP_LCD_ShiftLeft(void);
void BSP_LCD_ShiftRight(void);

#endif /* __KAMELEON_DISP_H_ */
