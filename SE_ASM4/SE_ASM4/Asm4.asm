.586
.model flat,stdcall
includelib kernel32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.stack 4096
.const
.data

MB_OK	EQU 0  

STR1	DB "�������� �������� ������������ 2 ���� 7 ������", 0 
HW		DD ?  


; ��� ������: short
shortVar DW 12345
; ��� ������: uint16_t
unsignedShortVar DW 54321


.code
main PROC

    ; �������� � �������� ��������
    mov ax, shortVar
    mov bx, unsignedShortVar
    ; ����� ����� �������� �������������� ����������
	

	push 0  
	CALL ExitProcess 

main ENDP 
end main
