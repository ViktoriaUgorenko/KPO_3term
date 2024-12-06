#include "stdafx.h"

namespace LT
{
    // —оздать таблицу лексем заданного размера
    LexTable Create(int size)
    {
        LexTable* tabl = new LexTable;

        // ѕроверить, не превышено ли максимальное количество лексем в таблице
        if (size > LT_MAXSIZE)
        {
            throw ERROR_THROW(2); // ¬ызвать ошибку с кодом 2, если превышено максимальное количество лексем (> 4096)
        }

        tabl->maxsize = size; // ”становить максимальный размер таблицы
        tabl->size = 0; // ”становить текущий размер таблицы
        tabl->table = new Entry[size]; // —оздать новую запись в таблице

        return *tabl; // ¬ернуть созданную таблицу
    }

    // ƒобавить запись в таблицу лексем
    void Add(LexTable& lextable, Entry entry)
    {
        // ѕроверить, не превышено ли максимальное количество лексем в таблице
        if (lextable.size + 1 > lextable.maxsize)
        {
            throw ERROR_THROW(114); // ¬ызвать ошибку с кодом 114, если превышено максимальное количество лексем в таблице
        }

        lextable.table[lextable.size] = entry; // ƒобавить запись в таблицу
        lextable.size += 1; // ”величить текущий размер таблицы
    }

    // ѕолучить запись из таблицы лексем по индексу
    Entry GetEntry(LexTable& lextable, int n)
    {
        return lextable.table[n]; // ¬ернуть запись по указанному индексу
    }

    // ”далить таблицу лексем
    void Delete(LexTable& lextable)
    {
        delete[] lextable.table; // ќсвободить пам€ть, занимаемую таблицей
        //delete &lextable; // ¬озможно, эту строку можно удалить, так как она закомментирована и не выполн€ет никаких действий
    }
};
