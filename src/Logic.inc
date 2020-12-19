;*******************************************************************************
; Logic.inc:
; Bietet alle Funktionen zur Spiellogik (Berechnen des Gewinners,
; Zugwechsel, etc.).
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Rechnet eine Spielfeldkoordinate (X und y) in eine Adresse im Buffer um.
; @param Akku a: X-Wert der Spielfeldkoordinate
; @param Akku b: Y-Wert der Spielfeldkoordinate
; @return cellAddress: Die Adresse der Zelle relativ zur Bufferadresse

getCellAddress      proc                          ; Address = (b * 56) + (a * byteSize)
                    pshd
                    pshb
                    ldb       #8
                    mul
                    std       cellAddress
                    pulb
                    lda       #ROW_LENGTH
                    mul
                    addd      cellAddress
                    std       cellAddress
                    puld
                    rts

;*******************************************************************************
; Prüft, ob in die Spalte an der aktuellen Cursorposition ein Stein
; gelegt werden kann. Wenn die Position gültig ist, wird ein Stein
; gesetzt.
; @param cursorColumn: Die Spalte, in die ein Stein gesetzt werden soll

setStone            proc
                    push

                    clra
                    ldb       cursorColumn
                    subb      #BOARD_OFFSET

                    ldx       #BYTE_SIZE
                    idiv
                    xgdx                          ; ergebnis der Division in d
                    tba
                    ldb       #5

                    ldy       #6
Loop@@              bsr       getCellAddress      ; Ergebnis in Variable cellAddress

                    psha
                    bsr       getCellOccupancy
                    tsta                          ; Wenn Leer dann wird der Stein gesetzt
                    pula
                    bne       Cont@@

                    psha
                    lda       player              ; Überprüfe welcher Spieler dran ist
                    cmpa      #1
                    pula
                    bne       _1@@

                    jsr       stone1ToBuffer
                    bra       Done@@

_1@@                jsr       stone2ToBuffer
                    bra       Done@@

Cont@@              decb
                    dey
                    bne       Loop@@
          ;--------------------------------------
          ; speichern der Spielfeldkoordinaten zum prüfen ob gewonnen wurde
          ;--------------------------------------
Done@@              sta       coordx
                    stb       coordy
                    jsr       drawBoard
                    pull
                    rts

;*******************************************************************************
; Gibt die Belegung einer Zeller wieder.
; @param cellAddress: Die Zellenadresse
; @return Akku a: 0 = leer, 1 = Spieler1, 2 = Spieler2

getCellOccupancy    proc                          ; Rückgabewert in a
                    pshb
                    pshx

                    ldx       #board
                    xgdx
                    addd      cellAddress
                    xgdx
          ;-------------------------------------- ; 3. Byte der Zelle: prüfen ob Zelle nur Nullen enthällt (keinen Stein)
                    lda       3,x
          ;-------------------------------------- ; erste und letzte 1 repräsentieren den Zellenrand
                    cmpa      #%10000001
                    beq       Empty?@@
                    cmpa      #%10111101
                    beq       Player1@@
                    cmpa      #%10100101
                    beq       Player2@@

Empty?@@            clra
                    bra       Done@@

Player1@@           lda       #1
                    bra       Done@@

Player2@@           lda       #2
;                   bra       Done@@

Done@@              pulx
                    pulb
                    rts

;*******************************************************************************
; Wechselt den aktuellen Spieler. Wenn Spieler1 am Zug ist, wird auf
; Spieler2 gewechselt und umgekehrt.
; @param player: Der Spieler, der aktuell am Zug ist
; @return player: 1 = Spieler1, 2 = Spieler2

nextPlayer          proc
                    psha

                    lda       player
                    cmpa      #1
                    beq       Player2@@

                    lda       #1
                    sta       player
                    bra       Done@@

Player2@@           lda       #2
                    sta       player

Done@@              pula
                    rts

;*******************************************************************************
; Überprüft, ob der aktuelle Spieler eine Viererreihe erreicht hat
; @param coordx: Die X-Koordinate des gesetzten Steins
; @param coordy: Die Y-Koordinate des gesetzten Steins
; @return Akku a: 0 = False, 1 = True

checkWin            proc
                    pshb
          ;-------------------------------------- ; Vertikal
                    clra                          ; Richtung unten
                    ldb       #1
                    bsr       getLineLength
                    cmpa      #4
                    bhs       True@@
          ;-------------------------------------- ; Horizontal
                    lda       #1                  ; Richtung Rechts
                    clrb
                    bsr       getLineLength
                    tab
                    pshb
                    lda       #-1                 ; Richtung Links
                    clrb
                    bsr       getLineLength
                    pulb
                    deca
                    aba
                    cmpa      #4
                    bhs       True@@
          ;-------------------------------------- ; Diagonal Oben Rechts nach unten Links
                    lda       #1                  ; Richtung Oben Rechts
                    ldb       #-1
                    bsr       getLineLength
                    tab
                    pshb
                    lda       #-1                 ; Richtung unten Links
                    ldb       #1
                    bsr       getLineLength
                    pulb
                    deca
                    aba
                    cmpa      #4
                    bhs       True@@
          ;-------------------------------------- ; Diagonal unten Rechts nach links Oben
                    lda       #-1                 ; Richtung links Oben
                    ldb       #-1
                    bsr       getLineLength
                    tab
                    pshb
                    lda       #1                  ; Richtung unten Rechts
                    ldb       #1
                    bsr       getLineLength
                    pulb
                    deca
                    aba
                    cmpa      #4
                    bhs       True@@
                    bra       False@@

True@@              lda       #1
                    sta       playerWonFlag
                    bra       Done@@

False@@             clra
Done@@              pulb
                    rts

;*******************************************************************************
; Zählt die Anzahl der verbundenen Steine des aktuellen Spielers in einer
; Reihe.
; @param Akku a: X-Richtung in die gelaufen werden soll
; @param Akku b: Y-Richtung in die gelaufen werden soll
; @param player: Der Spieler, der aktuell am Zug ist
; @return Akku a: Anzahl an Steinen die in einer Reihe gefunden wurden

getLineLength       proc
                    pshb
                    pshy
                    pshx

                    sta       xoffset
                    stb       yoffset
                    lda       coordx
                    ldb       coordy

                    clrx                          ; Zähler für die Länge einer Linie

Loop@@              adda      xoffset
                    addb      yoffset

                    psha
                    bsr       isOutOfBorder       ; Überprüfe ob nächster Stein sich noch
                    cmpa      #1                  ; innerhalb des Spielfeldes befindet
                    pula
                    beq       _1@@

                    jsr       getCellAddress
                    psha
                    jsr       getCellOccupancy    ; Überprüft, ob die nächste Zelle
                    cmpa      player              ; den Stein des Spielers enthält.
                    pula
                    bne       _1@@

                    inx                           ; Laufe nur 3 Steine lang
                    cpx       #3
                    bne       Loop@@

_1@@                xgdx
                    tba
                    inca                          ; Inkrementiere A, sodass der neu
          ;-------------------------------------- ;Done
                    pulx
                    puly
                    pulb
                    rts

;*******************************************************************************
; Gibt an ob die übergebenen Koordinaten sich innerhalb oder ausserhalb des
; Spielfeldes befinden.
; @param Akku a: X-Kooridnate des Spielfeldes
; @param Akku b: Y-Kooridnate des Spielfeldes
; @return Akku a: 0 = innerhalb des Spielfeldes, 1 = ausserhalb des Spielfeldes

isOutOfBorder       proc
                    pshb
                    cmpa      #-1
                    beq       True@@
                    cmpa      #7
                    beq       True@@
                    cmpb      #-1
                    beq       True@@
                    cmpb      #6
                    bne       False@@
True@@              lda       #1
                    bra       Done@@
False@@             clra
Done@@              pulb
                    rts

;*******************************************************************************
; Gibt an ob die übergebenen Koordinaten sich innerhalb oder ausserhalb des
; Spielfeldes befinden.
; @param Akku a: X-Kooridnate des Spielfeldes
; @param Akku b: Y-Kooridnate des Spielfeldes
; @return Akku a: 0 = innerhalb des Spielfeldes, 1 = ausserhalb des Spielfeldes

playerwon           proc                          ; Flag setzen das die Eingabe blockiert und nur Reset erlaubt
                    jsr       LCDClr
                    jsr       drawBoard
                    jmp       drawWinner
