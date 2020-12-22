;*******************************************************************************
; Registerdefinitionen für den trainer11
;*******************************************************************************

PIO_A               equ       $8000
PIO_B               equ       $8001
PIO_C               equ       $8002
PIO_CONFIG          equ       $8003

MAP_PAGE            def       $00                 ; wahlweise Offsets oder Adressen
          ;-------------------------------------- ; Register
PORTA               equ       $00+MAP_PAGE        ; Port A Data Register

PIOC                equ       $02+MAP_PAGE,1      ; Parallel I/O Control Register
PORTC               equ       $03+MAP_PAGE,1      ; Port C Data Register
PORTB               equ       $04+MAP_PAGE,1      ; Port B Data Register
PORTCL              equ       $05+MAP_PAGE,1      ; Alternate Latched Port C

DDRC                equ       $07+MAP_PAGE,1      ; Port C Data Direction Register
PORTD               equ       $08+MAP_PAGE,1      ; Port D Data Register
DDRD                equ       $09+MAP_PAGE,1      ; Port D Data Direction Register
PORTE               equ       $0A+MAP_PAGE,1      ; Port E Data Register
CFORC               equ       $0B+MAP_PAGE,1      ; Timer Compare Force Register
OC1M                equ       $0C+MAP_PAGE,1      ; Action Mask Register
OC1D                equ       $0D+MAP_PAGE,1      ; Action Data Register
TCNT                equ       $0E+MAP_PAGE,2      ; Timer Counter Register
TIC1                equ       $10+MAP_PAGE,2      ; Timer Input Capture
TIC2                equ       $12+MAP_PAGE,2      ; Timer Input Capture
TIC3                equ       $14+MAP_PAGE,2      ; Timer Input Capture
TOC1                equ       $16+MAP_PAGE,2      ; Timer Output Compare
TOC2                equ       $18+MAP_PAGE,2      ; Timer Output Compare
TOC3                equ       $1A+MAP_PAGE,2      ; Timer Output Compare
TOC4                equ       $1C+MAP_PAGE,2      ; Timer Output Compare
TOC5                equ       $1E+MAP_PAGE,2      ; Timer Output Compare
TCTL1               equ       $20+MAP_PAGE,1      ; Timer Control Register 1
TCTL2               equ       $21+MAP_PAGE,1      ; Timer Control Register 2
TMSK1               equ       $22+MAP_PAGE,1      ; Timer Interrupt Mask Register 1
TFLG1               equ       $23+MAP_PAGE,1      ; Timer Interrupt Flag Register 1
TMSK2               equ       $24+MAP_PAGE,1      ; Timer Interrupt Mask Register 2
TFLG2               equ       $25+MAP_PAGE,1      ; Timer Interrupt Flag Register 2
PACTL               equ       $26+MAP_PAGE,1      ; Pulse Accumulator Control Register
PACNT               equ       $27+MAP_PAGE,1      ; Pulse Accumulator Count Register High
SPCR                equ       $28+MAP_PAGE,1      ; SPI Control Register
SPSR                equ       $29+MAP_PAGE,1      ; SPI Status Register
SPDR                equ       $2A+MAP_PAGE,1      ; SPI Data Register
BAUD                equ       $2B+MAP_PAGE,1      ; SCI Baud Rate Register
SCCR1               equ       $2C+MAP_PAGE,1      ; SCI Control Register 1
SCCR2               equ       $2D+MAP_PAGE,1      ; SCI Control Register 2
SCSR                equ       $2E+MAP_PAGE,1      ; SCI Status Register
SCDR                equ       $2F+MAP_PAGE,1      ; SCI Data Register
ADCTL               equ       $30+MAP_PAGE,1      ; A/D Control Register
ADR1                equ       $31+MAP_PAGE,1      ; A/D Converter Result Register 1
ADR2                equ       $32+MAP_PAGE,1      ; A/D Converter Result Register 2
ADR3                equ       $33+MAP_PAGE,1      ; A/D Converter Result Register 3
ADR4                equ       $34+MAP_PAGE,1      ; A/D Converter Result Register 4

OPTION              equ       $39+MAP_PAGE,1      ; System Configuration Options
COPRST              equ       $3A+MAP_PAGE,1      ; Arm/Reset COP Timer register
PPROG               equ       $3B+MAP_PAGE,1      ; EEPROM Programming Control Register
HPRIO               equ       $3C+MAP_PAGE,1      ; Highest Priority Interrupt Register
INIT                equ       $3D+MAP_PAGE,1      ; RAM and I/O Mapping
TEST1               equ       $3E+MAP_PAGE,1      ; Factory TEST Control Register
CONFIG              equ       $3F+MAP_PAGE,1      ; COP, ROM and EEPROM Enables
          ;-------------------------------------- ; Vektoren
Vsci                equ       $FFD6
Vspi                equ       $FFD8
Vpai                equ       $FFDA
Vpao                equ       $FFDC
Vtof                equ       $FFDE
Vtic4               equ       $FFE0
Vtoc4               equ       $FFE2
Vtoc3               equ       $FFE4
Vtoc2               equ       $FFE6
Vtoc1               equ       $FFE8
Vtic3               equ       $FFEA
Vtic2               equ       $FFEC
Vtic1               equ       $FFEE
Vrti                equ       $FFF0
Virq                equ       $FFF2
Vxirq               equ       $FFF4
Vswi                equ       $FFF6
Viot                equ       $FFF8
Vcop                equ       $FFFA
Vclk                equ       $FFFC
Vreset              equ       $FFFE
