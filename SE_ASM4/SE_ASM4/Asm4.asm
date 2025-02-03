.586
.model flat,stdcall
includelib kernel32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.stack 4096
.const
.data

MB_OK	EQU 0  

STR1	DB "Угоренко Вкитория Владимировна 2 курс 7 группа", 0 
HW		DD ?  


; Тип данных: short
shortVar DW 12345
; Тип данных: uint16_t
unsignedShortVar DW 54321


.code
main PROC

    ; Загрузка и проверка значений
    mov ax, shortVar
    mov bx, unsignedShortVar
    ; Здесь можно добавить дополнительный функционал
	

	push 0  
	CALL ExitProcess 

main ENDP 
end main
