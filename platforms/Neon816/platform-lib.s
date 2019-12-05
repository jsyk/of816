; Platform support library for Neon816
; 
.include  "./Neon816-hw.inc"

.proc     _system_interface
          ;wdm 3
          phx
          asl
          tax
          jmp   (.loword(table),x)
table:    .addr _sf_pre_init
          .addr _sf_post_init
          .addr _sf_emit
          .addr _sf_keyq
          .addr _sf_key
          .addr _sf_fcode
          .addr _sf_reset_all
.endproc
.export   _system_interface

.proc     _sf_success
          lda   #$0000
          tay
          clc
          rtl
.endproc

.proc     _sf_fail
          ldy   #.loword(-21)
          lda   #.hiword(-21)
          sec
          rtl
.endproc


.proc     _sf_pre_init
          ; NeonFORTH does this, presumably to initialize the serial port
          ; The code from here to the EOC commment was adapted from code written by Lenore Byron
          sep   #SHORT_A
          .a8
          lda   #$8D
          sta   f:SERctrlA
          lda   #$06
          sta   f:SERctrlB
          lda   #$00
          sta   f:SERctrlC
          rep   #SHORT_A
          .a16
          ; EOC
          plx
          jmp   _sf_success
.endproc

.proc     _sf_post_init
          plx
          jmp   _sf_success
.endproc

.proc     _sf_emit
          plx                   ; get forth SP
          jsr   _popay          ; grab the top item
          phx                   ; and save new SP
          ; The code from here to the EOC commment was adapted from code written by Lenore Byron
          sep   #SHORT_A
          .a8
          tya
          sta   f:SERio
:         lda   f:SERstat
          bit   #$08
          bne   :-
          rep   #SHORT_A
          .a16
          ; EOC
          plx
          jmp   _sf_success
.endproc

.proc     _sf_keyq
          ldy   #$0000          ; anticipate false
          ; The code from here to the EOC commment was adapted from code written by Lenore Byron
          sep   #SHORT_A
          .a8
          lda   f:SERstat       ; b0=1 if data ready
          ror
          bcc   :+
          iny
:         rep   #SHORT_A
          .a16
          ; EOC
          tya
          plx
          jsr   _pushay
          jmp   _sf_success
.endproc

.proc     _sf_key
          ; The code from here to the EOC commment was adapted from code written by Lenore Byron
          sep   #SHORT_A
          .a8
:         lda   f:SERstat
          ror
          bcc   :-
          lda   f:SERio
          rep   #SHORT_A
          .a16
          ; EOC
          and   #$00FF
          tay
          lda   #$0000
          plx
          jsr   _pushay
          jmp   _sf_success
.endproc

.proc     _sf_fcode
.if include_fcode
          ldy   #.loword(list)
          lda   #.hiword(list)
.else
          lda   #$0000
          tay
.endif
          plx
          jsr   _pushay
          jmp   _sf_success
.if include_fcode
list:
          .dword 0
.endif
.endproc

; TODO....
.proc     _sf_reset_all
          plx
          jmp   _sf_fail
.endproc
