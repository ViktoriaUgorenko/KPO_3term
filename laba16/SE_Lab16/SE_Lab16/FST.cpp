#include "FST.h"
#include <iostream>
#include < string.h  > 
#include <stdarg.h>

namespace FST
{
	RELATION::RELATION(char c, short ns)
	{
		symbol = c;
		nnode = ns;
	};


	NODE::NODE() { 
		n_relation = 0; 
		RELATION* relations = nullptr;
	}
	NODE::NODE(short n, RELATION re1, ...) { // конструктор для узла с параметрами
		n_relation = n; 
		relations = new RELATION[n]; // выделяем память под массив связей
		va_list args;
		va_start(args, re1); // нач список аргументов с re1
		relations[0] = re1;
		// заполняем массив оставшимися элементами
		for (int i = 1; i < n; i++) {
			relations[i] = va_arg(args, RELATION); // получаем следующий RELATION из списка аргументов
		}

		va_end(args);
	}

	FST::FST(const char* s, short ns, NODE n, ...) { // конструктор конечного автомата
		strin = s; 
		nstates = ns; // кол-во состояний
		nodes = new NODE[ns]; // выделяем память для массива узлов
		va_list args;
		va_start(args, n); 
		nodes[0] = n;
		for (int k = 1; k < ns; k++) {
			nodes[k] = va_arg(args, NODE); // получаем следующий NODE из списка аргументов
		}
	
		va_end(args);
		rstates = new short[nstates]; // выделяем память для массива состояний
		memset(rstates, 0xff, sizeof(short) * nstates); // заполняем его значениями 0xff
		rstates[0] = 0; // устанавливаем начальное значение
		position = -1; // задаем текущую позицию
	}

	bool step(FST& fst, short*& rstates)
	{
		bool rc = false; //успеш ли переход

		std::swap(rstates, fst.rstates); // меняем местами указатели на массивы состояний

		for (short i = 0; i < fst.nstates; i++)
		{
			// проверяем было ли состояние i активным на предыдущем шаге
			if (rstates[i] == fst.position)
			{
				// проходим по всем переходам текущего состояния i
				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					//  совпадает ли символ перехода с текущим символом входной строки
					if (fst.nodes[i].relations[j].symbol == fst.strin[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true; // был совершен успешный переход
					}
				}
			}
		}

		return rc;
	}


	bool execute(FST& fst)
	{
		//память для массива возможных состояний на данном шаге
		short* rstates = new short[fst.nstates];

		// никакие состояния не активны
		memset(rstates, 0xff, sizeof(short) * fst.nstates);

		// длина входной строки
		short lstring = strlen(fst.strin);

		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;

			rc = step(fst, rstates);
		}

		delete[] rstates;

		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}


}