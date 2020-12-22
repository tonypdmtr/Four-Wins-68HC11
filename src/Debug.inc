;*******************************************************************************
; Debug.inc:
; Bietet Funktionen zum Debugging. Gibt Akkumulatoren, Register und CCR aus.
; Basisfunktionen von J.Voelker.
; Ergänzt um debugBuffer und debugCell.
;*******************************************************************************

;*******************************************************************************
                    #ROM
;*******************************************************************************

;*******************************************************************************
; Vier Bits - ein Nibble - aus B als Hexadezimalzahl ausgeben

nibbletohex         fcb       "0123456789ABCDEF"

putNibble           proc
                    pshx
                    pshd
                    andb      #$0F                ; Nibble maskieren,
                    ldx       #nibbletohex        ; aus der Zeichentabelle
                    abx                           ; das passende Zeichen waehlen
                    lda       ,x                  ; und in A laden.
                    jsr       putChar
                    puld
                    pulx
                    rts

;*******************************************************************************
; Inhalt von B als Hexadezimalzahl ausgeben

putByte             proc
                    pshb
                    rorb:4                        ; Oberes Nibble von B zur Ausgabe vorbereiten
                    jsr       putNibble
                    pulb
                    jmp       putNibble           ; Unteres Nibble von B ausgeben

;*******************************************************************************
; Inhalt von D als Hexadezimalzahl ausgeben

putHex              proc
                    psha
                    lda       #'$'
                    jsr       putChar
                    pula
                    pshb
                    tab                           ; Zuerst A ausgeben, den oberen Teil von D
                    jsr       putByte
                    pulb
                    jsr       putByte             ; Anschliessend B ausgeben, den unteren
          ;-------------------------------------- ; Teil von D
                    jsr       space
                    rts

;*******************************************************************************
; Inhalt von D als Hexadezimalzahl ausgeben

putHexBuffer        proc
                    pshb
                    tab                           ; Zuerst A ausgeben, den oberen Teil von D
                    jsr       putByte
                    pulb
                    jsr       putByte             ; Anschliessend B ausgeben, den unteren
          ;-------------------------------------- ; Teil von D
                    jmp       space

;*******************************************************************************
; Carry-Flag als Binaerzahl ausgeben

putCarry            proc
                    psha
                    bcs       _1@@                ; War das Carry-Flag gesetzt ?
                    lda       #'0'
                    bra       Send@@
_1@@                lda       #'1'
Send@@              jsr       putChar
                    pula
                    rts

;*******************************************************************************
; Wert in A als Binaerzahl ausgeben

putBinary           proc
                    psha
                    lda       #'%'
                    jsr       putChar
                    pula
                    psha
                    rola                          ; Das oberste Bit ins Carry-Flag rotieren
                    bsr       putCarry            ; und als Binaerzahl ausgeben.
                    rola
                    bsr       putCarry
                    rola
                    bsr       putCarry
                    rola
                    bsr       putCarry
                    rola
                    bsr       putCarry
                    rola
                    bsr       putCarry
                    rola
                    bsr       putCarry
                    rola
                    bsr       putCarry
                    lda       #32                 ; Leerzeichen anfuegen
                    jsr       putChar
                    pula
                    rts

;*******************************************************************************
; Alle Register ausgeben, dabei nichts veraendern, auch die Flags
; bleiben erhalten

debug               proc                          ; Alter PC (Stackpointer + 7)
                    push
                    tpa                           ; Flags in A holen
                    psha                          ; (Stackpointer + 0) und ebenfalls sichern.
          ;--------------------------------------
                    tsx                           ; Stackpointer in X holen

                    lda       #'P'                ; PC=
                    jsr       putChar
                    lda       #'C'
                    jsr       putChar
                    lda       #'='
                    jsr       putChar

                    ldd       7,x                 ; Den alten PC vom Stack in D laden
                    jsr       putHex
          ;--------------------------------------
                    lda       #'D'                ; D=
                    jsr       putChar
                    lda       #'='
                    jsr       putChar

                    ldd       1,x                 ; Gesicherten Inhalt von D vom Stack laden
                    jsr       putHex
          ;--------------------------------------
                    lda       #'X'                ; X=
                    jsr       putChar
                    lda       #'='
                    jsr       putChar

                    ldd       3,x                 ; Gesicherten Inhalt von X vom Stack laden
                    jsr       putHex
          ;--------------------------------------
                    lda       #'Y'                ; Y=
                    jsr       putChar
                    lda       #'='
                    jsr       putChar

                    ldd       5,x                 ; Gesicherten Inhalt von Y vom Stack laden
                    jsr       putHex
          ;--------------------------------------
                    lda       #'S'                ; SP=
                    jsr       putChar
                    lda       #'P'
                    jsr       putChar
                    lda       #'='
                    jsr       putChar

                    xgdx                          ; Den Stackpointer selbst in D laden
                    jsr       putHex
          ;--------------------------------------
                    lda       #'C'                ; CCR=
                    jsr       putChar
                    lda       #'C'
                    jsr       putChar
                    lda       #'R'
                    jsr       putChar
                    lda       #'='
                    jsr       putChar

                    tsx                           ; Nochmal den Stackpointer in X holen
                    lda       ,x                  ; Die auf dem Stack gesicherten Flags in A laden
                    jsr       putBinary
          ;--------------------------------------
                    jsr       crlf

                    pula                          ; Alten Inhalt
                    tap                           ; der Flags zurueckholen
                    pull                          ; Alle Register wiederhergestellt.
                    rts                           ; Nach dem Ruecksprung ist auch der
                                                  ; Stackpointer wieder wie vorher.
;*******************************************************************************
; Gibt den gesamten Buffer in Hexadezimal auf dem Terminal aus.
; @param board: Das Board (Buffer)

debugBuffer         proc
                    pshx
                    pshd
                    ldx       #board
                    ldd       #336
Loop@@              pshd
                    ldd       ,x
                    jsr       putHexBuffer
                    puld
                    inx:2
                    subd      #2
                    bne       Loop@@
                    puld
                    pulx
                    rts

;*******************************************************************************
; Gibt eine Zelle des Buffers in Hexadezimal auf dem Terminal aus.
; @param debugCellAdress: Relative Zellenadresse vom Anfang des Buffers
; @param board: Das Board (Buffer)

debugCell           proc
                    pshx
                    pshd
                    ldx       #board
                    xgdx
                    addd      debugCellAdress
                    xgdx
                    ldd       #8
Loop@@              pshd
                    ldd       ,x
                    jsr       putHexBuffer
                    puld
                    inx:2
                    subd      #2
                    bne       Loop@@
                    jsr       crlf
                    puld
                    pulx
                    rts
