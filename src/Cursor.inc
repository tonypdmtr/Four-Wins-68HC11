;*******************************************************************************
; Cursor.inc:
; Verwaltet die Curosrfunktionen.
; Bietet Funktionen um den Cursor zu verschieben und diesen auf dem LCD zu
; Zeichnen.
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Setzt den Cursor auf die Mitte des Spielfelds und aktualisiert
; das Display.
; @return cursorColumn: Die mittlere Spalte des Spielfeldes

resetCursor         proc
                    psha
                    lda       #BOARD_MID_COLUMN
                    adda      #BOARD_OFFSET
                    sta       cursorColumn
                    bsr       drawCursor
                    pula
                    rts

;*******************************************************************************
; Bewegt den Cursor um eine Spalte nach links, bei der äußerst linken
; Spalte wird der Cursor nach ganz rechts gesetzt.
; @param cursorColumn: Die aktuelle Cursorposition
; @return cursorColumn: Die neue Cursorposition um eine Zelle nach links
; geschoben

moveCursorLeft      proc
                    psha
                    jsr       deleteCursor

                    lda       cursorColumn
          ;--------------------------------------
          ; vergleicht die Cursorposition mit dem Wert des äußerst linken
          ; Spielfeldrandes "boardOffset"
          ;--------------------------------------
                    cmpa      #BOARD_OFFSET
                    beq       _1@@

                    suba      #BYTE_SIZE
                    sta       cursorColumn
                    bra       Done@@
_1@@      ;--------------------------------------
          ; setzt den Cursor auf die äußerst rechte Position
          ; (-8 weil die Position immer den Anfang einer Zelle angibt)
          ;--------------------------------------
                    lda       #BOARD_OFFSET+ROW_LENGTH-BYTE_SIZE
                    sta       cursorColumn

Done@@              bsr       drawCursor
                    pula
                    rts

;*******************************************************************************
; Bewegt den Cursor um eine Spalte nach rechts, bei der äußerst rechten
; Spalte wird der Cursor nach ganz links gesetzt.
; @param cursorColumn: Die aktuelle Cursorposition
; @return cursorColumn: Die neue Cursorposition um eine Zelle nach rechts
; geschoben

moveCursorRight     proc
                    psha
                    bsr       deleteCursor

                    lda       cursorColumn
          ;--------------------------------------
          ; vergleicht den Cursor mit der äußerst rechten Position
          ; (-8 weil die Position immer den Anfang einer Zelle angibt)
          ;--------------------------------------
                    cmpa      #BOARD_OFFSET+ROW_LENGTH-BYTE_SIZE
                    beq       _1@@

                    adda      #BYTE_SIZE
                    sta       cursorColumn
                    bra       Done@@

_1@@                lda       #BOARD_OFFSET
                    sta       cursorColumn

Done@@              bsr       drawCursor
                    pula
                    rts

;*******************************************************************************
; Gibt den Cursor an der  aktuelle Cursorposition auf dem Display aus.
; @param cursorColumn: Die aktuelle Cursorposition

drawCursor          proc
                    psha

                    lda       #CURSOR_ROW
                    jsr       LCDSetRow

                    lda       cursorColumn
                    adda      #1
                    jsr       LCDSetCol
                    lda       #%01100000
                    jsr       LCDdata

                    lda       cursorColumn
                    adda      #2
                    jsr       LCDSetCol
                    lda       #%01110000
                    jsr       LCDdata

                    lda       cursorColumn
                    adda      #3
                    jsr       LCDSetCol
                    lda       #%01111100
                    jsr       LCDdata

                    lda       cursorColumn
                    adda      #4
                    jsr       LCDSetCol
                    lda       #%01111100
                    jsr       LCDdata

                    lda       cursorColumn
                    adda      #5
                    jsr       LCDSetCol
                    lda       #%01110000
                    jsr       LCDdata

                    lda       cursorColumn
                    adda      #6
                    jsr       LCDSetCol
                    lda       #%01100000
                    jsr       LCDdata

                    pula
                    rts

;*******************************************************************************
; Löscht die aktuelle Cursorposition auf dem Display
; @param cursorColumn: Die aktuelle Cursorposition

deleteCursor        proc
                    psha
                    lda       #CURSOR_ROW
                    jsr       LCDSetRow
                    lda       #1
Loop@@              bsr       Send@@
                    inca
                    cmpa      #6
                    bls       Loop@@
                    pula
                    rts

Send@@              adda      cursorColumn
                    jsr       LCDSetCol
                    clra
                    jmp       LCDdata
