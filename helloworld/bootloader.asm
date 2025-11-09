[bits 16]        ; Set assembler to 16-bit mode, required for BIOS bootloader
[org 0x7c00]     ; Origin: tells assembler the code will be loaded at memory address 0x7C00 by BIOS

start:
    mov si, msg      ; Load the address of the message string into SI register
    mov ah, 0x0e     ; Set AH = 0x0E, BIOS teletype function (prints character in AL to screen)
    cli              ; Disable maskable interrupts (optional, ensures no interrupts occur while printing)

.printchar:
    lodsb            ; Load byte at [SI] into AL and increment SI
    cmp al, 0        ; Compare AL with zero (null terminator of the string)
    je .done         ; If AL is zero, end of string reached, jump to done
    int 0x10         ; Call BIOS video interrupt 0x10, function 0x0E to print character in AL
    jmp .printchar   ; Repeat for next character

.done:
    ret              ; Return from bootloader (in practice, halts or waits for next step)

data:
msg db 'Hello World!', 0  ; The string to print, null-terminated

times 510-($-$$) db 0     ; Fill remaining space up to 510 bytes with zeroes
dw 0xaa55                 ; Boot signature (0xAA55) that BIOS checks to see if this is bootable
