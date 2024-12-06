#pragma once
#define ID_MAXSIZE 6
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255
namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2 };
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };

	struct Entry		// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		char		id[ID_MAXSIZE];		// идентификатор (автоматически усекаетс€ до ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE		idtype;				// тип идентификатора
		Entry* scope;
		union
		{
			int		vint;				// значение integer
			struct
			{
				int len;						// кол-во символов в string
				char str[TI_STR_MAXSIZE - 1];	// символы string
			}	vstr[TI_STR_MAXSIZE];			// значение string
		} value;	// значение идентификатора
	};
	struct IdTable			// экземпл€р таблицы идентификаторов
	{
		int maxsize;		// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;			// текущий размер таблицы идентификаторов < maxsize
		Entry* table;		// массив строк таблицы идентификаторов
	};
	IdTable Create(		// создать таблицу идентификаторов	
		int size		// емкость таблицы идентифиторов < TI_MAXSIZE
	);
	void Add(				// получить строку таблицы идентификаторов
		IdTable& idtable,	// экзмепл€р таблицы идентификаторов
		Entry entry			// номер получаемой строки
	);
	int search(
		IdTable& idtable,
		IT::Entry entry);
	Entry GetEntry(			// получить строку таблицы идентификаторов
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		int n				// идентификатор
	);
	int IsId(				// возврат: номер строки (если есть), TI_NULLIDX(если нет)
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		char id[ID_MAXSIZE]	// идентификатор
	);
	void Delete(IdTable& idtable);	// удалить таблицу лексем (освободить пам€ть)
}