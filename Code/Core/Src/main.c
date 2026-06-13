#include "main.h"
#include "st7735.h"

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       160
#define MAX_SNAKE_LENGTH    40

#define EVENT_DIRECTION_UP      (1 << 0)
#define EVENT_DIRECTION_DOWN    (1 << 1)
#define EVENT_DIRECTION_LEFT    (1 << 2)
#define EVENT_DIRECTION_RIGHT   (1 << 3)

typedef enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} direction_t;

typedef struct {
  uint8_t x;
  uint8_t y;
} point_t;

typedef struct {
  point_t body[MAX_SNAKE_LENGTH];
  uint8_t length;
  direction_t dir;
} snake_t;

snake_t snake_play;
point_t food;

SemaphoreHandle_t mutex_snake;
SemaphoreHandle_t mutex_lcd;

EventGroupHandle_t event_direction;

volatile int is_growing = 0;

void safe_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
  xSemaphoreTake(mutex_lcd, portMAX_DELAY);
  st7735_draw_pixel(x, y, color);
  xSemaphoreGive(mutex_lcd);
}

void draw_pixel_block(uint8_t x, uint8_t y, uint16_t color) {
  for (int dx = 0; dx < 2; dx++) {
    for (int dy = 0; dy < 2; dy++) {
      safe_draw_pixel(x * 2 + dx, y * 2 + dy, color);
    }
  }
}

void draw_food(void) {
  draw_pixel_block(food.x, food.y, 0xF800);
}

void draw_head(point_t p) {
  draw_pixel_block(p.x, p.y, 0x07E0);
}

void clear_tail(point_t tail) {
  draw_pixel_block(tail.x, tail.y, 0x0000);
}

void generate_food(void) {
  food.x = rand() % (SCREEN_WIDTH / 2);
  food.y = rand() % (SCREEN_HEIGHT / 2);
}

void init_game(void) {
  gpio_config(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT_PU);
  gpio_config(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT_PU);
  gpio_config(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT_PU);
  gpio_config(GPIOA, GPIO_PIN_4, GPIO_MODE_INPUT_PU);

  snake_play.length = 3;
  snake_play.dir = DIRECTION_RIGHT;
  snake_play.body[0] = (point_t){5, 5};
  snake_play.body[1] = (point_t){4, 5};
  snake_play.body[2] = (point_t){3, 5};

  generate_food();
  st7735_full_display(0x0000);
  draw_food();
  draw_head(snake_play.body[0]);
  draw_pixel_block(snake_play.body[1].x, snake_play.body[1].y, 0x07E0);
  draw_pixel_block(snake_play.body[2].x, snake_play.body[2].y, 0x07E0);
}

void check_collision(void) {
  point_t head = snake_play.body[0];

  if (head.x >= SCREEN_WIDTH / 2 || head.y >= SCREEN_HEIGHT / 2 || head.x < 0 || head.y < 0) {
    xSemaphoreTake(mutex_lcd, portMAX_DELAY);
    st7735_draw_string(0, 0, "GAME OVER", font_7x10, 0xFFFF, 0x0000);
    xSemaphoreGive(mutex_lcd);
    while (1) {}
  }

  if (head.x == food.x && head.y == food.y) {
    if (snake_play.length < MAX_SNAKE_LENGTH) {
      snake_play.body[snake_play.length] = snake_play.body[snake_play.length - 1];
      snake_play.length++;
      is_growing = 1;
      generate_food();
      draw_food();
    } else {
      is_growing = 0;
    }
  } else {
    is_growing = 0;
  }
}

void move_snake(void) {
  point_t tail = snake_play.body[snake_play.length - 1];
  for (int i = snake_play.length - 1; i > 0; i--) {
    snake_play.body[i] = snake_play.body[i - 1];
  }

  switch (snake_play.dir) {
    case DIRECTION_UP:
      snake_play.body[0].y--;
      break;
    case DIRECTION_DOWN:
      snake_play.body[0].y++;
      break;
    case DIRECTION_LEFT:
      snake_play.body[0].x--;
      break;
    case DIRECTION_RIGHT:
      snake_play.body[0].x++;
      break;
  }

  if (!is_growing) {
    clear_tail(tail);
  }
}

void rtos_task_snake(void *param) {
  (void)param;
  while (1) {
    EventBits_t bits = xEventGroupWaitBits(event_direction,
      EVENT_DIRECTION_UP | EVENT_DIRECTION_DOWN |
      EVENT_DIRECTION_LEFT | EVENT_DIRECTION_RIGHT,
      pdTRUE, pdFALSE, pdMS_TO_TICKS(0));

    xSemaphoreTake(mutex_snake, portMAX_DELAY);

    if ((bits & EVENT_DIRECTION_UP) && snake_play.dir != DIRECTION_DOWN) {
      snake_play.dir = DIRECTION_UP;
    }
    if ((bits & EVENT_DIRECTION_DOWN) && snake_play.dir != DIRECTION_UP) {
      snake_play.dir = DIRECTION_DOWN;
    }
    if ((bits & EVENT_DIRECTION_LEFT) && snake_play.dir != DIRECTION_RIGHT) {
      snake_play.dir = DIRECTION_LEFT;
    }
    if ((bits & EVENT_DIRECTION_RIGHT) && snake_play.dir != DIRECTION_LEFT) {
      snake_play.dir = DIRECTION_RIGHT;
    }

    check_collision();
    move_snake();
    draw_head(snake_play.body[0]);

    xSemaphoreGive(mutex_snake);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

int button_up_pressed(void) {
  return gpio_read_pin(GPIOA, GPIO_PIN_4) == 0;
}

int button_down_pressed(void) {
  return gpio_read_pin(GPIOA, GPIO_PIN_0) == 0;
}

int button_left_pressed(void) {
  return gpio_read_pin(GPIOA, GPIO_PIN_2) == 0;
}

int button_right_pressed(void) {
  return gpio_read_pin(GPIOA, GPIO_PIN_3) == 0;
}

void rtos_task_input(void *param) {
  (void)param;
  while (1) {
    if (button_up_pressed()) {
      xEventGroupSetBits(event_direction, EVENT_DIRECTION_UP);
    }
    if (button_down_pressed()) {
      xEventGroupSetBits(event_direction, EVENT_DIRECTION_DOWN);
    }
    if (button_left_pressed()) {
      xEventGroupSetBits(event_direction, EVENT_DIRECTION_LEFT);
    }
    if (button_right_pressed()) {
      xEventGroupSetBits(event_direction, EVENT_DIRECTION_RIGHT);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

int main(void) {
  rcc_enable_spi1();
  rcc_enable_port_a();
  rcc_enable_port_b();
  rcc_enable_afio();
  rcc_enable_tim2();

  spi_init_master();
  st7735_init();
  st7735_full_display(0x0000);

  mutex_snake = xSemaphoreCreateMutex();
  mutex_lcd = xSemaphoreCreateMutex();
  event_direction = xEventGroupCreate();

  init_game();

  if (xTaskCreate(rtos_task_snake, "Snake", 256, NULL, 2, NULL) != pdPASS ||
      xTaskCreate(rtos_task_input, "Input", 128, NULL, 2, NULL) != pdPASS) {
    xSemaphoreTake(mutex_lcd, portMAX_DELAY);
    st7735_draw_string(0, 0, "TASK FAILED", font_7x10, 0xFFFF, 0x0000);
    xSemaphoreGive(mutex_lcd);
    while (1) {}
  }

  vTaskStartScheduler();

  while (1) {}
  return 0;
}