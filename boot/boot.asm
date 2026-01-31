; Intrarea initiala a bootloader-ului
; rulează în real mode → set long mode
[bits 64]
global _start
extern load_main

_start:
    cli
    ; Set stack
    mov rsp, stack_top

    call load_main  ; jump în bootloader C

.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
