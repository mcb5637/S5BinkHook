
makefunc MACRO name, adr
PUBLIC name
name PROC
    mov eax, adr
    jmp eax
name ENDP
ENDM
