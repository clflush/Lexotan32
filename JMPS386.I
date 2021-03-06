; redefine commands: jcc label         7x xx  ->  0f 8x xxxx
;                    jcxz label        e3 xx  ->  or cx, cx (or ecx, ecx)
;                                                 jz label
;                    loop label        e2 xx  ->  dec cx
;                                                 jz label
;                    loopd label    66 e2 xx  ->  dec ecx
;                                                 jz label
;                    jmp label         e9 xxxx -> clc
;                                                 jnc label
;                                                 or
;                                                 stc
;                                                 jc  label
;                                                 or
;                                                 jx  label
;                                                 jnx label
;                                                 or
;                                                 jnx label
;                                                 jx  label
;                    rep                       -> loop

j_o             equ     0000b
j_no            equ     0001b
j_b             equ     0010b
j_nae           equ     j_b
j_nb            equ     0011b
j_ae            equ     j_nb
j_c             equ     j_b
j_nc            equ     j_nb
j_e             equ     0100b
j_z             equ     j_e
j_ne            equ     0101b
j_nz            equ     j_ne
j_be            equ     0110b
j_na            equ     j_be
j_nbe           equ     0111b
j_a             equ     j_nbe
j_s             equ     1000b
j_ns            equ     1001b
j_p             equ     1010b
j_pe            equ     j_p
j_np            equ     1011b
j_po            equ     j_np
j_l             equ     1100b
j_nge           equ     j_l
j_nl            equ     1101b
j_ge            equ     j_nl
j_le            equ     1110b
j_ng            equ     j_le
j_nle           equ     1111b
j_g             equ     j_nle

jjj             macro   label, j_j
                db      0fh,80h + j_j
                dd      label-$-4
                endm

jo              macro   label
                jjj     label, j_o
                endm

jno             macro   label
                jjj     label, j_no
                endm

jb              macro   label
                jjj     label, j_b
                endm

jnae            macro   label
                jjj     label, j_nae
                endm

jnb             macro   label
                jjj     label, j_nb
                endm

jc              macro   label
                jjj     label, j_c
                endm

jnc             macro   label
                jjj     label, j_nc
                endm


jae             macro   label
                jjj     label, j_ae
                endm

je              macro   label
                jjj     label, j_e
                endm

jz              macro   label
                jjj     label, j_z
                endm

jne             macro   label
                jjj     label, j_ne
                endm

jnz             macro   label
                jjj     label, j_nz
                endm

jbe             macro   label
                jjj     label, j_be
                endm

jna             macro   label
                jjj     label, j_na
                endm

jnbe            macro   label
                jjj     label, j_nbe
                endm

ja              macro   label
                jjj     label, j_a
                endm

js              macro   label
                jjj     label, j_s
                endm

jns             macro   label
                jjj     label, j_ns
                endm

jp              macro   label
                jjj     label, j_p
                endm

jpe             macro   label
                jjj     label, j_pe
                endm

jnp             macro   label
                jjj     label, j_np
                endm

jpo             macro     label
                jjj     label, j_po
                endm

jl              macro   label
                jjj     label, j_l
                endm

jnge            macro   label
                jjj     label, j_nge
                endm

jnl             macro   label
                jjj     label, j_nl
                endm

jge             macro   label
                jjj     label, j_ge
                endm

jle             macro   label
                jjj     label, j_le
                endm

jng             macro   label
                jjj     label, j_ng
                endm

jnle            macro   label
                jjj     label, j_nle
                endm

jg              macro   label
                jjj      label, j_g
                endm

jmp             macro   label
                db      0e9h
                dd      label-$-4
                endm

jecxz           macro label
                test ecx,ecx
                jz label
                endm

loop            macro label
                dec ecx
                jnz label
                endm

;rep             macro instr
;  LOCAL L1,L2
;  L1:
;                jecxz L2
;                instr
;                dec ecx
;                jmp L1
;  L2:
;                endm

