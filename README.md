# Zigbee

##GenericJson
My zigbee code for smart auto home device 

----------
Parse json data through zigbee(cc2530)'s uart

default short address is 0xffff to broadcast 

combined with arduino / stm32 MCU 

Long time push the P0_5 button can reset zigbee to search and join new network

some delay at startup to avoid openwrt's boot message to mess up zigbee ,and the failsafe mode,too

commands are in guu_json_cb.h/c

baud rate is 38400

two uarts enabled






