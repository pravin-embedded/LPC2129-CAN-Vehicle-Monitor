LPC2129 CAN VEHICLE MONITOR  -  3 nodes, one folder, three Keil projects
=======================================================================

OPEN / BUILD (one project per node, Device = LPC2129 in all three)
  CAN.uvproj      -> MAIN node     (CAN.hex)     LCD + DS18B20 + switches
  REVERSE.uvproj  -> REVERSE node  (REVERSE.hex) LEDs + ultrasonic
  FUEL.uvproj     -> FUEL node     (FUEL.hex)    ADC + LCD
  Rebuild all three (Project > Rebuild all target files).
  Keep every file in this one folder: can_defines.h / can_ids.h are SHARED,
  so all three nodes always get identical CAN timing and IDs.

CLOCK / CAN
  FOSC 12 MHz, CCLK 60 MHz, PCLK 15 MHz (Startup.s: VPBDIV_SETUP = 0)
  CAN 125 kbps: BRP 6, 20 tq/bit, sample point 70 %, BTR = 0x005CC005
  Timer0 prescaler T0PR = 14  ->  1 us per tick

CAN FRAMES (DLC = 1, only Data1 used, Data2 = 0)
  0x100  Main    -> Reverse   Data1 = 1 LEFT / 2 RIGHT
  0x101  Fuel    -> Main      Data1 = fuel %
  0x102  Reverse -> Main      Data1 = 0 SAFE / 1 WARNING / 2 STOP

PINS
  Switches : P0.1 EINT0 = MODE, P0.3 EINT1 = LEFT, P0.7 EINT2 = RIGHT
  LCD      : D0-D7 = P0.8-P0.15, RS = P0.16, EN = P0.18, RW = GND
  DS18B20  : DQ = P0.<OW_PIN> (onewire.h, currently 17)  <-- confirm wiring
  Ultrasonic (Reverse node): TRIG P0.16, ECHO P0.17
  Indicator LEDs (Reverse node): P0.4-P0.7, active low
  CAN1: RX = P0.25, TX = dedicated pin

DEBUG SWITCH
  can_defines.h:  #define CAN_OVERRUN_RECOVERY 1   (set 0 to disable the
  overrun fallback and get the plain reference CAN behaviour)

ULTRASONIC RESULT (reverse_def.c)
  0   = sensor never answered -> STOP (fail-safe)
  999 = echo never ended / nothing in range -> SAFE
