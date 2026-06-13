# Bare-metal FreeRTOS Snake Game
The project uses FreeRTOS to manage concurrent tasks (input polling and game logic), employing Mutexes to prevent display race conditions and Event Groups for task synchronization.
All peripheral configurations (RCC, GPIO, AFIO, SPI, Timer, I2C, UART, and EXTI) are programmed directly at the bare-metal register level.

## Architecture

![firmware_architecture](Docs/firmware_architecture.jpg)

## Sequence Diagram

##### Mutex LCD

![sequence_diagram_mutex_lcd](Docs/sequence_diagram_mutex_lcd.jpg)

##### Event Group
![sequence_diagram_event_group](Docs/sequence_diagram_event_group.jpg)

## Flowchart
##### Task Snake
![flow_chart_rtos_task_snake](Docs/flow_chart_rtos_task_snake.jpg)