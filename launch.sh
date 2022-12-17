#!/bin/bash

sudo ./color_sense > fifo_color_sense &
sudo ./led_blinker 2 > fifo_led_blinker &

sudo ./combiner fifo_color_sense fifo_led_blinker
