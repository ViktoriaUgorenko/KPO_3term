//магазинный автомат для синтаксического анализа
#pragma once
#include "stdafx.h"
#include<iomanip>

// Класс-наследник от стандартного стека с элементами типа short
class my_stack_SHORT : public std::stack<short> {
public:
    using std::stack<short>::c; // Делаем доступным внутренний контейнер стека
};

// Макросы для диагностики и трассировки
// Эти макросы используются для вывода информации о работе автомата на каждом шаге

#define MFST_DIAGN_MAXSIZE 2* ERROR_MAXSIZE_MESSAGE // Максимальный размер строки диагностики
#define MFST_DIAGN_NUMBER 3 // Количество сохраняемых сообщений диагностики

// Переменные для отладки
static int FST_TRACE_n = -1; // Счетчик шагов
static char rbuf[205], sbuf[205], lbuf[1024]; // Буферы для вывода

// Макрос для вывода заголовка трассировки
#define MFST_TRACE_START std::cout << std::setw(4) << std::left << "Шаг" << ":" \
                                   << std::setw(20) << std::left << " Правило" \
                                   << std::setw(30) << std::left << " Входная лента" \
                                   << std::setw(20) << std::left << " Стек" \
                                   << std::endl;

// Макрос для вывода информации о текущем шаге
#define MFST_TRACE1 std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " \
                               << std::setw(20) << std::left << rule.getCRule(rbuf,nrulechain) \
                               << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
                               << std::setw(20) << std::left << getCSt(sbuf) \
                               << std::endl;

// Макрос для вывода информации о текущем состоянии ленты и стека без изменения правила
#define MFST_TRACE2 std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " \
                               << std::setw(20) << std::left << " " \
                               << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
                               << std::setw(20) << std::left << getCSt(sbuf) \
                               << std::endl;

// Аналогично, другие макросы выводят разные аспекты состояния автомата
#define MFST_TRACE3 std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " \
                               << std::setw(20) << std::left << " " \
                               << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
                               << std::setw(20) << std::left << getCSt(sbuf) \
                               << std::endl;

#define MFST_TRACE4(c) std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl;
#define MFST_TRACE5(c) std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl;
#define MFST_TRACE6(c, k) std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << k << std::endl;
#define MFST_TRACE7 std::cout << std::setw(4) << std::left << state.lenta_position << ": " \
                               << std::setw(20) << std::left << rule.getCRule(rbuf, state.nrulechain) \
                               << std::endl;

// Класс для работы с состояниями автомата
typedef my_stack_SHORT MFSTSTSTACK;

namespace MFST
{
    // Структура, описывающая состояние автомата
    struct MfstState {
        short lenta_position;  // Текущая позиция на ленте
        short nrule;           // Номер текущего правила
        short nrulechain;      // Номер текущей цепочки правила
        MFSTSTSTACK st;        // Стек автомата

        MfstState(); // Конструктор по умолчанию
        MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain); // Инициализация позиции и стека
        MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain); // Инициализация всех полей
    };

    // Класс, который расширяет стек для хранения состояний автомата
    class my_stack_MfstState : public std::stack<MfstState> {
    public:
        using std::stack<MfstState>::c; // Открываем доступ к внутреннему контейнеру
    };

    // Основной класс магазина автомата (parser)
    struct Mfst {
        // Перечисление для кодов возврата функции step (шаг автомата)
        enum RC_STEP {
            NS_OK,            // Найдено правило и цепочка, цепочка записана в стек
            NS_NORULE,        // Не найдено правило грамматики
            NS_NORULECHAIN,   // Не найдена подходящая цепочка правила
            NS_ERROR,         // Неизвестный нетерминал
            TS_OK,            // Текущий символ ленты равен вершине стека, стек обновлен
            TS_NOK,           // Текущий символ ленты не равен вершине стека
            LENTA_END,        // Достигнут конец ленты
            SURPRISE,         // Неожиданный код возврата (ошибка)
        };

        // Структура для диагностики ошибок в автомате
        struct MfstDiagnosis {
            short lenta_position; // Позиция на ленте
            RC_STEP rc_step;      // Код завершения шага
            short nrule;          // Номер правила
            short nrule_chain;    // Номер цепочки правила

            MfstDiagnosis(); // Конструктор по умолчанию
            MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain); // Инициализация полей
        } diagnosis[MFST_DIAGN_NUMBER]; // Хранение последних трех сообщений диагностики

        GRBALPHABET* lenta;      // Лента, перекодированная из лексического анализатора
        short lenta_position;    // Текущая позиция на ленте
        short nrule;             // Номер текущего правила
        short nrulechain;        // Номер текущей цепочки правила
        short lenta_size;        // Размер ленты
        GRB::Greibach grebach;   // Грамматика Грейбах
        LT::LexTable lex;        // Лексическая таблица
        MFSTSTSTACK st;          // Стек автомата
        my_stack_MfstState storestate; // Стек для сохранения состояний автомата

        Mfst(); // Конструктор по умолчанию
        Mfst(LT::LexTable& plex, GRB::Greibach pgrebach); // Конструктор инициализирующий автомат лексической таблицей и грамматикой

        char* getCSt(char* buf); // Метод для получения содержимого стека в виде строки
        char* getCLenta(char* buf, short pos, short n = 25); // Лента: n символов с позиции pos
        char* getDiagnosis(short n, char* buf); // Получение строки диагностики для n-го шага

        bool savestate(); // Сохранение состояния автомата
        bool resetstate(); // Восстановление состояния автомата

        bool push_chain(GRB::Rule::Chain chain); // Помещает цепочку правила в стек
        RC_STEP step(); // Выполняет шаг автомата
        bool start(); // Запуск автомата
        bool savediagnosis(RC_STEP pprc_step); // Сохранение кода завершения шага

        void printrules(); // Вывод последовательности правил, по которым работает автомат

        // Вспомогательная структура для дедукции (построение дерева вывода)
        struct Deducation {
            short size; // Количество правил
            short* nrules; // Массив номеров правил
            short* nrulechains; // Массив номеров цепочек правил

            Deducation() {
                size = 0;
                nrules = 0;
                nrulechains = 0;
            };
        } deducation;

        bool savededucation(); // Сохранение информации для построения дерева вывода
    };
}

