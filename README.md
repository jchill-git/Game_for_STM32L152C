# Game_for_STM32L152C

Welcome to Dragonfly, a simple game written for a STM32L152C as part of a course on computer architecture. 

Instructions:
You control a hungry dragonfly who's trying to eat as many small insects as possible. Holding down the button on your board will allow you to move up the display, while releasing it will cause you to drop back down. At a regular interval you'll recieve an alert about the location of the next insect, whether it will be coming on the top middle or bottom row of the display. The green LED means it'll be on top; the blue, on the bottom; and the combination of both, in the middle. You will then have about a second to manuever your dragonfly into position to catch the incoming insect. Once the insect appears on screen, your dragonfly is locked in. You score one point for each insect consumed, and the game ends when you let one fly by.

Files:
main.c contains the primary program for the game.
stm32l1xx.c and stm32l_discovery_lcd.c contain other firmware that's been costumized to provide the symbols needed to run the game display.
