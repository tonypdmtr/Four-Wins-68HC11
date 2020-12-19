;*******************************************************************************
; Utils.inc:
; Hilfsmethoden für das Terminal und den RAM.
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Setzt alle Bits im RAM auf 0.

clrRam              proc
                    pshx
                    ldx       #ramBegin
Loop@@              clr       ,x
                    inx
                    cpx       #ramEnd
                    bne       Loop@@
                    pulx
                    rts

;*******************************************************************************
; Zeichen in A auf dem Terminal ausgeben.
; @param Akku a: Das auszugebende Zeichen

putChar             proc
                    brclr     SCSR,#$80,*         ; Warte, bis der Ausgabepuffer leer ist
                    sta       SCDR                ; Zeichen in A abschicken
                    rts

;*******************************************************************************
; crlf:
; Zeilenumbruch auf dem Terminal ausgeben.

crlf                proc
                    psha
                    lda       #CR
                    bsr       putChar
                    lda       #LF
                    bsr       putChar
                    pula
                    rts

;*******************************************************************************
; Leerzeichen auf dem Terminal ausgeben.

space               proc
                    psha
                    lda       #' '
                    bsr       putChar
                    pula
                    rts
