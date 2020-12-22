;*******************************************************************************
; Board.inc:
; Verwaltet den Buffer, welcher das Spielfeld repräsentiert.
; Bietet Funktionen um den Buffer zu befüllen und diesen auf dem LCD zu
; Zeichnen.
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Zeichnet eine Zelle auf den LCD-Display.
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers

drawCell            proc
                    push
          ;-------------------------------------- ; Berechne die absolute Adresse der Zelle im Buffer
                    ldd       #board
                    addd      cellAddress
                    tdy
          ;--------------------------------------
          ; Zeile und Spalte der Zelle berechnen und in D (Spalte)
          ; und X (Zeile) Speichern
          ;--------------------------------------
                    ldd       cellAddress
                    ldx       #ROW_LENGTH
                    idiv
          ;-------------------------------------- ; Zeile setzen
                    pshd
                    xgdx
                    tba
                    jsr       LCDSetRow
                    puld
          ;-------------------------------------- ; Bildschirm Versatz mit berücksichtigen
                    tba
                    adda      #BOARD_OFFSET
                    clrb
Loop@@    ;-------------------------------------- ; Spalte setzen
                    jsr       LCDSetCol
          ;-------------------------------------- ; Zeichen (Byte) an LCD senden
                    psha
                    lda       ,y
                    jsr       LCDdata
                    pula

                    iny
                    inca
                    incb
                    cmpb      #8
                    bne       Loop@@

                    pull
                    rts

;*******************************************************************************
; Schreibt eine leere "Zelle" (8 * 8 Bit) an der Position angegben
; durch "cellAdress" in den Buffer "Board".
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers

cellToBuffer        proc
                    pshx
                    psha
          ;-------------------------------------- ; Berechne Adresse der Zelle im Buffer
                    ldx       #board
                    xgdx
                    addd      cellAddress
                    xgdx

                    lda       #%11111111          ; linker Rand der Zelle
                    sta       ,x

                    lda       #%10000001
                    sta       1,x

                    lda       #%10000001
                    sta       2,x

                    lda       #%10000001
                    sta       3,x

                    lda       #%10000001
                    sta       4,x

                    lda       #%10000001
                    sta       5,x

                    lda       #%10000001
                    sta       6,x

                    lda       #%11111111          ; rechter Rand der Zelle
                    sta       7,x

                    pula
                    pulx
                    rts

;*******************************************************************************
; Füllt den komplettenden Buffer "Board" mit leeren Zellen.

createGrid          proc
                    pshd
                    clrd
                    std       cellAddress
Loop@@              bsr       cellToBuffer
                    addd      #BYTE_SIZE
                    std       cellAddress
                    cmpd      #BOARD_SIZE
                    bne       Loop@@
                    puld
                    rts

;*******************************************************************************
; Zeichnet den kompletten Buffer auf den Display.

drawBoard           proc
                    pshd
                    clrd
                    std       cellAddress
Loop@@              jsr       drawCell
                    addd      #BYTE_SIZE
                    std       cellAddress
                    cmpd      #BOARD_SIZE
                    bne       Loop@@
                    puld
                    rts

;*******************************************************************************
; Schreibt einen Spielstein für Spieler1 (gefüllt) in den Buffer,
; der Spielstein wird dabei mit der leeren Zelle oder-Verknüpft.
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers

stone1ToBuffer      proc
                    pshx
                    psha
          ;-------------------------------------- ; Berechne Adresse der Zelle im Buffer
                    ldx       #board
                    xgdx
                    addd      cellAddress
                    xgdx

                    lda       #%00011000          ; Stein für Spieler 1 (gefüllt)
                    ora       2,x
                    sta       2,x

                    lda       #%00111100
                    ora       3,x
                    sta       3,x

                    lda       #%00111100
                    ora       4,x
                    sta       4,x

                    lda       #%00011000
                    ora       5,x
                    sta       5,x

                    pula
                    pulx
                    rts

;*******************************************************************************
; Schreibt einen Spielstein für Spieler2 (leer) in den Buffer,
; der Spielstein wird dabei mit der leeren Zelle oder-Verknüpft.
; @param cellAddress: Relative Zellenadresse vom Anfang des Buffers

stone2ToBuffer      proc
                    pshx
                    pshd
          ;-------------------------------------- ; Berechne Adresse der Zelle im Buffer
                    ldx       #board
                    xgdx
                    addd      cellAddress
                    xgdx

                    lda       #%00011000          ; Stein für Spieler 2 (leer)
                    ora       2,x
                    sta       2,x

                    lda       #%00100100
                    ora       3,x
                    sta       3,x

                    lda       #%00100100
                    ora       4,x
                    sta       4,x

                    lda       #%00011000
                    ora       5,x
                    sta       5,x

                    puld
                    pulx
                    rts
