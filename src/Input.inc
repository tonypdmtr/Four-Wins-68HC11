;*******************************************************************************
; Input.inc:
; Verarbeitet die Benutzereingaben und reagiert dementsprechend.
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Liest das Datenbyte des Tastenfeldes (PIO_B) aus und reagiert entsprechend
; auf die Taste.
; @param PIO_B: Das Signal Byte der 8 Tasten in 1 aus n Kodierung

readInput           proc
                    pshd

                    lda       PIO_B               ; Lese Button byte ein
                    ldb       buttonFlag          ; Lese ButtonfLag ein

                    cmpb      #1                  ; 1 Wenn Taste gedrückt
                    beq       _4@@
          ;-------------------------------------- ; Wenn Taste 4 gedrückt, reset des Spiels
                    cmpa      #%11101111
                    bne       _1@@
                    bsr       setButtonFlag
                    jsr       initGame
                    bra       Done@@
          ;-------------------------------------- ; Wenn Taste 0 gedrückt, setze Stein
_1@@                tst       playerWonFlag       ; alle Tasten ausser reset sperren wenn das Spiel gewonnen ist
                    bne       Done@@

                    cmpa      #%11111110
                    bne       _2@@
                    bsr       setButtonFlag
                    bsr       setStone

                    ldb       coordy              ; testen ob ein Stein gesetzt werden
                    cmpb      #$FF                ; konnte, wenn nicht KEIN
                    beq       Done@@              ; Spielerwechsel / checkWin

                    jsr       checkWin
                    cmpa      #1
                    beq       SomeoneWon@@

                    jsr       nextPlayer
                    jsr       drawPlayer

                    bra       Done@@
          ;-------------------------------------- ; Wenn Taste 1 gedrückt, setze Flag
_2@@                cmpa      #%11111101          ; und bewege Cursor nach Links
                    bne       _3@@
                    bsr       setButtonFlag
                    jsr       moveCursorLeft
                    bra       Done@@
_3@@      ;-------------------------------------- ; Wenn Taste 3 gedrückt, setze Flag
                    cmpa      #%11110111          ; und bewege Cursor nach Rechts
                    bne       Done@@
                    bsr       setButtonFlag
                    jsr       moveCursorRight
                    bra       Done@@
          ;-------------------------------------- ; entprellen der Tasten mit einem Timer
_4@@                bsr       incTimer
                    ldb       timer
                    cmpb      #250
                    bne       Done@@
                    bsr       resetTimer
                    cmpa      #%11111111          ; Überprüfe ob Taste losgelassen
                    bne       Done@@              ; wurde -> Reset Buttonflag
                    bsr       resetButtonFlag
                    bra       Done@@
SomeoneWon@@        jsr       playerwon
Done@@              puld
                    rts

;*******************************************************************************
; Setzt ein Flag das eine Taste gedrückt -und bisher noch nicht
; losgelassen wurde.
; @return buttonFlag: auf 1 gesetzt

setButtonFlag       proc
                    psha
                    lda       #1
                    sta       buttonFlag
                    pula
                    rts

;*******************************************************************************
; Setzt das Buttonflag zurück.
; @return buttonFlag: auf 0 gesetzt

resetButtonFlag     proc
                    clr       buttonFlag
                    rts

;*******************************************************************************
; Inkrementiert einen Timer zum entprellen der Tasten.
; @param timer: der Timer der inkrementiert werden soll
; @return timer: der inkrementierte Timer

incTimer            proc
                    inc       timer
                    rts

;*******************************************************************************
; Setzt den Timer zum entprellen der Tasten zurück.
; @param timer: der Timer der zurückgesetzt werden soll
; @return timer: der zurückgesetzte Timer

resetTimer          proc
                    clr       timer
                    rts
