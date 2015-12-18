# Zigbee

#### GenericJson
My zigbee code for smart auto home device 

----------
+ Parse json data through zigbee(cc2530)'s uart

+ default short address is 0xffff to broadcast 

+ Combined with Arduino / STM32 MCU 

+ Long time push (about 7 seconds) the P0_5 button can reset zigbee to search and join new network

+ Some delays (on Coordinator side)  at startup to avoid openwrt's boot message to mess up zigbee ,and the failsafe mode,too

+ Main commands are in guu_json_cb.h/c

+ Baudrate is 38400

+ Two uarts enabled

+ IDE: IAR 8.10 





