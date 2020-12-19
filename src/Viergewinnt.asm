;*******************************************************************************
; Viergewinnt.asm:
;
; @Author: Michael Persiehl (tinf102296), Guillaume Fournier-Mayer (tinf101922)
; Implementiert das Spiel "4-Gewinnt" für 2 Spieler auf dem Motorola 68HC11.
;*******************************************************************************

LCD_COLS            equ       128                 ; Horizontale Pixeln des LCD's
LCD_ROWS            equ       8                   ; Vertikalen Bytes des LCD's
BOARD_SIZE          equ       336                 ; Byteanzahl des Buffers
BYTE_SIZE           equ       8                   ; Größe eines Bytes in Bit
ROW_LENGTH          equ       56                  ; Zeilenlänge des Spielfeldes in Pixeln

CURSOR_ROW          equ       6                   ; Vertikale Position des Cursors in Byte
BOARD_OFFSET        equ       36                  ; horizontaler Versatz des Spielfeldes
BOARD_MID_COLUMN    equ       24                  ; Die mittlere Spalte des Spielfeldes

CR                  equ       13
LF                  equ       10

                    #Uses     trainer11Register.inc ; Register- und Makrodefinitionen

;*******************************************************************************
                    #RAM                          ; Definition von Variablen im RAM
;*******************************************************************************
                    org       $0040

ramBegin            equ       *                   ; Anfang des Rams
board               rmb       BOARD_SIZE          ; Der Buffer für das Spielfeld
cellAddress         rmb       2                   ; Die Adresse für eine Zeile des Spielfeldes (als Funktionsparameter)
cursorColumn        rmb       1                   ; Horizontale Position des Cursors in Pixeln
debugCellAdress     rmb       2                   ; Die Adresse für eine Zeile des
                                                  ; Spielfeldes (Funktionsparameter
                                                  ; fürs Debugging)
buttonFlag          rmb       1                   ; Flag zur Flankenerkennung -> 1 = gedrückt, 0 = nicht gedrückt
timer               rmb       1                   ; Timer zum entprellen der Tasten
player              rmb       1                   ; Der Spieler der aktuell am Zug ist (1 = Spieler 1, 2 = Spieler 2)
coordx              rmb       1                   ; X-Koodrindate auf dem Spielfed
coordy              rmb       1                   ; Y-Koodrindate auf dem Spielfed
xoffset             rmb       1                   ; X-Offset für Linienerkennung
yoffset             rmb       1                   ; Y-Offset für Linienerkennung
playerWonFlag       rmb       1                   ; Flag um Tasten zu sperren wenn ein Spieler gewonnen hat

;*******************************************************************************
                    #ROM
;*******************************************************************************
                    org       $C000

                    #Uses     utils.inc
                    #Uses     lcdutils.inc
                    #Uses     board.inc
                    #Uses     cursor.inc
                    #Uses     input.inc
                    #Uses     logic.inc

ramEnd              equ       :RAM                ; Ende des Rams

;*******************************************************************************
                    #ROM                          ; Beginn des Programmcodes im schreibgeschuetzten Teil des Speichers
;*******************************************************************************

pageCmdMsk          fcb       %10110000           ; Steuerkommando fürs Display
colCmdMskM          fcb       %00010000           ; Steuerkommando fürs Display
colCmdMskL          fcb       %00000000           ; Steuerkommando fürs Display
adrMask             fcb       %00000111           ; Maske

;*******************************************************************************
; Konstanten für Textausgabe auf dem LCD
; der erste Wert jeder Konstante repräsentiert deren Länge in Byte

turnText            fcb       21,$02,$7E,$02,$00  ; T
                    fcb       $3C,$40,$40,$7C,$00 ; u
                    fcb       $7C,$08,$04,$08,$00 ; r
                    fcb       $7C,$08,$04,$78,$00 ; n
                    fcb       $28,$00             ; :

playerText          fcb       30,$7E,$12,$12,$0C,$00  ; P
                    fcb       $02,$7E,$00         ; l
                    fcb       $20,$54,$54,$78,$00 ; a
                    fcb       $1C,$A0,$A0,$7C,$00 ; y
                    fcb       $38,$54,$54,$48,$00 ; e
                    fcb       $7C,$08,$04,$08,$00 ; r
                    fcb       $00,$00

oneText             fcb       4,$04,$7E,$00,$00   ; 1

twoText             fcb       5,$64,$52,$52,$4C,$00 ; 2

wonText             fcb       17,$00,$00,$3C,$40,$20,$40,$3C,$00 ; w
                    fcb       $38,$44,$44,$38,$00 ; o
                    fcb       $7C,$08,$04,$78,$00 ; n

;*******************************************************************************
; Hauptprogramm

initGame            proc
                    psha
                    lda       #$0C                ; Löscht das Terminal
                    jsr       putChar
                    jsr       clrRam              ; Überschreibt den Ram mit Nullen
                    jsr       initSPI             ; Initialisierung der SPI-Schnittstelle
                    jsr       initLCD             ; Initialisierung des Displays
                    jsr       LCDClr              ; Löscht das Display
                    jsr       createGrid          ; Schreibt das Spielfeld in den Buffer
                    jsr       drawBoard           ; Schreibt den Bufferinhalt auf den LCDs
                    jsr       resetCursor         ; Setz den Cursor in die Mitte des Spielfeldes
                    lda       #1
                    sta       player              ; wechselt zu Spieler 1
                    jsr       showText
                    jsr       drawPlayer
                    pula
                    rts

;*******************************************************************************

Start               proc
                    lds       #$3FFF              ; Einstiegspunkt des Spieles
                    bsr       initGame
Loop@@              jsr       readInput           ; Hauptschleife
                    bra       Loop@@

;*******************************************************************************
                    #VECTORS
;*******************************************************************************

                    org       VRESET              ; Reset-Vektor
                    dw        Start               ; Programm-Startadresse
