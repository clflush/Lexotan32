;DEBUGOFF

rnd    PROC
       pushad
       mov eax,[seed]
       mov ecx,41c64e6dh
       mul ecx
       add eax,3039h
       and eax,7ffffffh
       mov [seed], eax
       mov ecx,[esp.cPushad.Arg1]
       jecxz @@nolimit
       cdq
       div ecx
       xchg eax, edx
  @@nolimit:
       mov [esp.Pushad_eax], eax
       popad
       ret 4
rnd    ENDP