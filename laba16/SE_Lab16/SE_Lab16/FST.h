#pragma once
#include <cstddef>
namespace FST {

	struct RELATION { 
		char symbol; 
		short nnode; 
		RELATION(
			char c = 0x00, 
			short ns = NULL 
		);
	};

	struct NODE { 
		short n_relation; 
		RELATION  *relations;
		NODE();
		NODE(
			short n, //кол-во ребер
			RELATION rel, ... //список ребер
		);
	};
	struct FST { 
		const char* strin; 
		short position; 
		short nstates; //кол-во состояний автомата
		NODE* nodes; 
		short* rstates; // возможные состояние автомата на данной позиции
		FST(
			const char* s, //цепочка
			short ns, //кол-во состояний автомата 
			NODE n, ... //список состояний
		);
	};
	bool execute( 
		FST& fst 
	);
}