#ifndef __FONT_H
#define __FONT_H
#include <stdint.h>
typedef struct {
	
	const uint8_t width;
	const uint8_t height;
	const uint16_t *data;
	
} font_t;


extern const font_t font_7x10 ;

#endif

