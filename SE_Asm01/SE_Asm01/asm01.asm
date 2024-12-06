.586P ; система команд
														
.MODEL FLAT, STDCALL			; модель память , соглашение о вызовах								
														
includelib kernel32.lib									
														
ExitProcess PROTO : DWORD								
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD	
														
.STACK 4096								; выделение стека				
														
.CONST													; сегмент констант
														
.DATA													; сегмент данных
MB_OK EQU 0												; определяем константу
STR1 DB "Моя первая программа", 0						
STR2 DB "Привет всем!", 0								
HW   DD ?												
														
.CODE							; сегмент кода						
														
main PROC					; точка входа main							

START:										; метка			
		
		INVOKE MessageBoxA, HW, OFFSET STR2, OFFSET STR1, MB_OK

	push - 1			; код возврата процесса Windows								
	call ExitProcess			;завершение процесса						
main ENDP												
														
end main ; конец модуля main