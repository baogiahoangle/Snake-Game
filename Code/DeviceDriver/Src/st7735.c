#include "st7735.h"

void st7735_write_cmd(uint8_t cmd) {
  gpio_write_pin(ST7735_PORT_CS, ST7735_PIN_CS, 0);
  gpio_write_pin(ST7735_PORT_A0, ST7735_PIN_A0, 0);
  spi1_send(cmd);
  gpio_write_pin(ST7735_PORT_CS, ST7735_PIN_CS, 1);
}

void st7735_write_data(uint8_t data) {
  gpio_write_pin(ST7735_PORT_CS, ST7735_PIN_CS, 0);
  gpio_write_pin(ST7735_PORT_A0, ST7735_PIN_A0, 1);
  spi1_send(data);
  gpio_write_pin(ST7735_PORT_CS, ST7735_PIN_CS, 1);
}

void st7735_send_cmd_list(const uint8_t *cmd_list) {
  uint8_t index = 0;
  uint8_t cmd = 0;
  uint8_t num = 0;
  while (1) {
    cmd = *cmd_list++;
    num = *cmd_list++;
    if (cmd == ST7735_CMD_END) {
      break;
    } else {
      st7735_write_cmd(cmd);
      for (index = 0; index < num; index++) {
        st7735_write_data(*cmd_list++);
      }
    }
  }
}

void st7735_set_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  st7735_write_cmd(0x2A);
  st7735_write_data(0x00);
  st7735_write_data(x1);
  st7735_write_data(0x00);
  st7735_write_data(x2);
  st7735_write_cmd(0x2B);
  st7735_write_data(0x00);
  st7735_write_data(y1);
  st7735_write_data(0x00);
  st7735_write_data(y2);
}

void st7735_full_display(uint16_t color) {
  st7735_write_cmd(0x2C);
  int i;
  for (i = 0; i < 128 * 160; i++) {
    st7735_write_data(color & 0xFF);
    st7735_write_data(color >> 8);
  }
}

void st7735_draw_pixel(uint8_t x, uint8_t y, uint16_t color) {
  if (x >= 128 || y >= 160) {
    return;
  }
  st7735_set_pos(x, y, x + 1, y + 1);
  st7735_write_cmd(0x2C);
  st7735_write_data(color & 0xFF);
  st7735_write_data(color >> 8);
}

void st7735_init(void) {
  gpio_config(ST7735_PORT_A0, ST7735_PIN_A0, GPIO_MODE_OUTPUT_PP);
  gpio_config(ST7735_PORT_CS, ST7735_PIN_CS, GPIO_MODE_OUTPUT_PP);
  gpio_config(ST7735_PORT_RESET, ST7735_PIN_RESET, GPIO_MODE_OUTPUT_PP);
  gpio_write_pin(ST7735_PORT_RESET, ST7735_PIN_RESET, 0);
  delay_ms(20);
  gpio_write_pin(ST7735_PORT_RESET, ST7735_PIN_RESET, 1);
  delay_ms(150);
  st7735_write_cmd(0x01);
  delay_ms(150);
  st7735_write_cmd(0x11);
  delay_ms(255);

  st7735_send_cmd_list(st7735_cmd_list);

  st7735_write_cmd(0x36);
  st7735_write_data(0x08);

  st7735_write_cmd(0x3A);
  st7735_write_data(0x05);

  st7735_write_cmd(0x20);

  st7735_set_pos(0, 0, 128, 160);

  st7735_write_cmd(0x29);
  delay_ms(100);
}

void st7735_draw_char(uint8_t x, uint8_t y, char ch, font_t font, uint16_t color, uint16_t bg) {
  uint8_t i, j;
  uint16_t pixel_data;
  for (i = 0; i < font.height; i++) {
    pixel_data = font.data[(ch - 32) * font.height + i];
    for (j = 0; j < font.width; j++) {
      if ((pixel_data << j) & 0x8000) {
        st7735_draw_pixel(x + j, y + i, color);
      } else {
        st7735_draw_pixel(x + j, y + i, bg);
      }
    }
  }
}

void st7735_draw_string(uint8_t x, uint8_t y, char *str, font_t font, uint16_t color, uint16_t bg) {
  while (*str) {
    st7735_draw_char(x, y, *str, font, color, bg);
    x += font.width;
    str++;
  }
}
