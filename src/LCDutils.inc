;*******************************************************************************
; LCDutils.inc:
;
; Bietet Funktionen um das Display zu initialisieren und anzusteuern.
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Initialisiert die SPI-Schnittstelle.

initSPI             proc
                    psha
                    lda       #%00000011
                    sta       PORTD
                    lda       #%00111010
                    sta       DDRD                ; TXD,MOSI,SCK,-SS = out
                    lda       #%01010010          ; 125kHz,Mode=0
                    sta       SPCR                ; enable,no int
                    pula
                    rts

;*******************************************************************************
; Hilfsmethode für LCDSend

SPItransfer         proc
                    sta       SPDR                ; Byte senden
Loop@@              tst       SPSR                ; Warten mal anders
                    bpl       Loop@@              ; auf MSB = SPIF = 1
                    lda       SPDR                ; Antwortbyte holen
                    rts

;*******************************************************************************
; Initialisierung des Displays.

initLCD             proc
                    psha
                    lda       #$A2
                    bsr       LCDcommand
                    lda       #$A0
                    bsr       LCDcommand
                    lda       #$C8
                    bsr       LCDcommand
                    lda       #$24
                    bsr       LCDcommand
                    lda       #$81
                    bsr       LCDcommand
                    lda       #$2F
                    bsr:2     LCDcommand
                    lda       #$AF
                    bsr       LCDcommand
                    bclr      PORTA,%01000000     ; PA6 = DIMM
                    pula                          ; Hintergrundbeleuchtung an
                    rts

;*******************************************************************************
; Sendet ein Kommandobyte an das Display.
; @param Akku a: Das zu sendende Kommandobyte

LCDcommand          proc
                    bclr      PORTD,%00100000     ; PD5, LCD_A0=0
                    bra       LCDSend

;*******************************************************************************
; Sendet ein Datenbyte an das Display.
; @param Akku a: Das zu sendende Datenbyte

LCDdata             proc
                    bset      PORTD,%00100000     ; PD5, LCD_A0=1
;                   bra       LCDSend

;*******************************************************************************
; Hilfsmethode für LCDcommand und LCDdata.
; Sendet ein Daten- oder Kommando an das Display.
; @param Akku a: Das zu sendende Datenbyte

LCDSend             proc
                    psha
                    lda       PIO_C
                    anda      #%00111111          ; SPI_SEL = 0 = LCD
                    sta       PIO_C
                    pula
                    psha
                    bsr       SPItransfer
                    lda       PIO_C               ; Deselect LCD
                    oraa      #%11000000          ; SPI_SEL = 3 = EEPROM
                    sta       PIO_C
                    pula
                    rts

;*******************************************************************************
; Setzt die Zeile des LCD-Cursors.
; @param Akku a: Die Cursorzeile

LCDSetRow           proc
                    psha
                    oraa      pageCmdMsk
                    bsr       LCDcommand
                    pula
                    rts

;*******************************************************************************
; LCDsetColMSB:
;
; Hilfsmethode für LCDSetCol.
; Setzt die ersten vier Bits der Spalte des LCD-Cursors.
;
; @param Akku a: die ersten vier Bits der Spalte des LCD-Cursors

LCDsetColMSB        proc
                    psha
                    oraa      colCmdMskM
                    bsr       LCDcommand
                    pula
                    rts

;*******************************************************************************
; Hilfsmethode für LCDSetCol.
; Setzt die letzten vier Bits der Spalte des LCD-Cursors.
; @param Akku a: die letzten vier Bits der Spalte des LCD-Cursors

LCDsetColLSB        proc
                    psha
                    oraa      colCmdMskL
                    bsr       LCDcommand
                    pula
                    rts

;*******************************************************************************
; Setzt die Spalte des LCD-Cursors.
; @param Akku a: die Spalte des LCD-Cursors

LCDSetCol           proc
                    psha
                    psha
                    anda      #%00001111
                    bsr       LCDsetColLSB
                    pula
                    lsra:4                        ; Shifte um 4 nach links
                    bsr       LCDsetColMSB
                    pula
                    rts

;*******************************************************************************
; Hilfsmethode für LCDClr.

LCDClrRow           proc
                    psha
                    pshb
                    ldb       #LCD_COLS
                    clra
                    bsr       LCDSetCol
Loop@@              bsr       LCDdata
                    decb
                    bne       Loop@@
                    pulb
                    pula
                    rts

;*******************************************************************************
; Löscht alle Pixel des LCD's.

LCDClr              proc
                    psha
                    clra
Loop@@              bsr       LCDSetRow
                    bsr       LCDClrRow
                    inca
                    cmpa      #LCD_ROWS
                    bne       Loop@@
                    clra                          ; Setze Cursor wieder auf 0:0
                    bsr       LCDSetCol
                    bsr       LCDSetRow
                    pula
                    rts

;*******************************************************************************
; Schreibt die Texte für "Player" und "Turn:" auf den LCD.

showText            proc
                    pshd
                    pshx
                    clrd
                    ldx       #turnText           ; Text "Turn:" oben links auf LCD
                    bsr       drawText
                    lda       #1
                    clrb
                    ldx       #playerText         ; Text "Player" in die 2. Zeile, links auf LCD
                    bsr       drawText
                    pulx
                    puld
                    rts

;*******************************************************************************
; Zeichnet den Text an die übergebene Zeile und Spalte des Displays
; @param Akku a: Zeile des LCD's
; @param Akku b: Spalte des LCD's
; @param Register x: Adresse des Textes im Speicher

drawText            proc
                    pshd
                    pshx
                    bsr       LCDSetRow
                    tba                           ; nach setzen der Zeile, Spalte in A schreiben
                    ldb       ,x
                    inx                           ; erstes Byte in X ist die Länge des Texts
Loop@@              bsr       LCDSetCol
                    psha
                    lda       ,x
                    jsr       LCDdata
                    pula
                    inca
                    inx
                    decb
                    bne       Loop@@
                    pulx
                    puld
                    rts

;*******************************************************************************
; Schreibt die Spielernummer des Spielers, welcher gerade am Zug ist.
; @param player: Die Nummer des Spielers, welcher gerade am Zug ist

drawPlayer          proc
                    push
                    lda       #1
                    ldy       #playerText
                    ldb       ,y
                    incb
                    psha
                    lda       player
                    cmpa      #2
                    pula
                    beq       _2@@

                    ldx       #oneText
                    bsr       drawText
                    bra       Done@@

_2@@                ldx       #twoText
                    bsr       drawText

Done@@              pull
                    rts

;*******************************************************************************
; Schreibt eine Nachricht an den Spieler, welcher das Spiel gewonnen hat,
; auf den LCD.
; Löscht alle anderen Texte vom LCD und sperrt alle Tasten außer der
; Reset-Taste.
; @param player: Die Nummer des Spielers, welcher gewonnen hat

drawWinner          proc
                    pshd
                    pshx

                    lda       #6                  ; Zeichne "Spieler"
                    ldb       #BOARD_OFFSET
                    ldx       #playerText
                    bsr       drawText

                    addb      ,x                  ; Zeichne die Spielernummer
                    psha
                    lda       player
                    cmpa      #2
                    pula
                    beq       _2@@                ; bestimmen welcher Spieler gewonnen hat
          ;-------------------------------------- ; Fall Spieler1 gewonnen
                    ldx       #oneText
                    bsr       drawText
                    bra       Done@@
_2@@      ;-------------------------------------- ; Fall Spieler2 gewonnen
                    ldx       #twoText
                    bsr       drawText
          ;--------------------------------------
Done@@              addb      ,x
                    ldx       #wonText
                    bsr       drawText

                    pulx
                    puld
                    rts
