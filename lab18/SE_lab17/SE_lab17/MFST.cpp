#include "MFST.h"

namespace MFST
{
    // Конструктор по умолчанию для состояния автомата (MfstState)
    MfstState::MfstState()
    {
        // Инициализация полей: позиция ленты, номер правила и номер цепочки правила
        lenta_position = 0;
        nrule = -1;
        nrulechain = -1;
    };

    // Конструктор с параметрами: позиция ленты, стек состояний автомата и номер цепочки правила
    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
    {
        lenta_position = pposition;
        st = pst;
        nrulechain = pnrulechain;
    };

    // Конструктор с параметрами: позиция ленты, стек, номер правила и номер цепочки
    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
    {
        lenta_position = pposition;
        st = pst;
        nrule = pnrule;
        nrulechain = pnrulechain;
    };

    // Конструктор по умолчанию для диагностики синтаксического анализа
    Mfst::MfstDiagnosis::MfstDiagnosis()
    {
        lenta_position = -1; // Позиция на ленте: -1 означает, что не установлена
        rc_step = SURPRISE;  // Этап обработки: SURPRISE по умолчанию
        nrule = -1;          // Номер правила по умолчанию
        nrule_chain = -1;    // Номер цепочки правила по умолчанию
    };

    // Конструктор диагностики с параметрами
    Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
    {
        lenta_position = plenta_position;
        rc_step = prc_step;
        nrule = pnrule;
        nrule_chain = pnrule_chain;
    };

    // Конструктор по умолчанию для автомата Mfst
    Mfst::Mfst()
    {
        lenta = 0;
        lenta_size = lenta_position = 0;
    };

    // Конструктор с инициализацией лексической таблицы и грамматики Грейбах (Greibach)
    Mfst::Mfst(LT::LexTable& lextable, GRB::Greibach pgrebach)
    {
        grebach = pgrebach; // Инициализация грамматики
        lex = lextable; // Инициализация лексической таблицы
        lenta = new short[lenta_size = lex.size]; // Выделение памяти для ленты

        // Преобразуем лексемы в элементы алфавита Грейбах
        for (int k = 0; k < lenta_size; k++)
            lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema[0]);

        lenta_position = 0; // Начальная позиция на ленте
        st.push(grebach.stbottomT); // Помещаем на стек нижний терминальный символ
        st.push(grebach.startN); // Помещаем стартовый нетерминал грамматики
        nrulechain = -1; // Начальное значение для цепочки правил
    }

    // Основной метод для выполнения одного шага автомата
    Mfst::RC_STEP Mfst::step()
    {
        RC_STEP rc = SURPRISE; // Инициализируем результат выполнения (SURPRISE как неизвестное состояние)

        // Если позиция на ленте меньше её размера (проверка на конец ленты)
        if (lenta_position < lenta_size)
        {
            // Если вершина стека является нетерминалом
            if (GRB::Rule::Chain::isN(st.top()))
            {
                GRB::Rule rule; // Объявляем объект для хранения текущего правила

                // Получаем правило для текущего нетерминала из стека
                if ((nrule = grebach.getRule(st.top(), rule)) >= 0)
                {
                    GRB::Rule::Chain chain; // Объявляем объект для хранения цепочки

                    // Получаем цепочку правила, соответствующую текущему символу на ленте
                    if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
                    {
                        MFST_TRACE1 // Отладочный вывод состояния (макрос)
                            savestate(); // Сохраняем текущее состояние
                        st.pop(); // Убираем текущий нетерминал из стека
                        push_chain(chain); // Помещаем цепочку в стек
                        rc = NS_OK; // Возвращаем статус успешного завершения шага
                        MFST_TRACE2 // Отладочный вывод состояния (макрос)
                    }
                    else
                    {
                        // Если цепочка не найдена
                        MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE") // Отладочное сообщение
                            savediagnosis(NS_NORULECHAIN); // Сохраняем диагностику
                        rc = resetstate() ? NS_NORULECHAIN : NS_NORULE; // Пытаемся сбросить состояние
                    };
                }
                else
                {
                    rc = NS_ERROR; // Если правило не найдено, возвращаем статус ошибки
                }
            }
            // Если вершина стека соответствует текущему символу на ленте
            else if ((st.top() == lenta[lenta_position]))
            {
                lenta_position++; // Переходим к следующему символу на ленте
                st.pop(); // Убираем символ из стека
                nrulechain = -1; // Сбрасываем индекс цепочки правил
                rc = TS_OK; // Возвращаем статус успешного завершения шага для терминала
                MFST_TRACE3 // Отладочный вывод состояния (макрос)
            }
            else
            {
                // Если текущее состояние не соответствует правилам
                MFST_TRACE4(TS_NOK / NS_NORULECHAIN) // Отладочное сообщение
                    rc = resetstate() ? TS_NOK : NS_NORULECHAIN; // Пытаемся сбросить состояние
            }
        }
        else
        {
            rc = LENTA_END; // Если достигли конца ленты
            MFST_TRACE4(LENTA_END) // Отладочное сообщение
        };
        return rc; // Возвращаем результат выполнения шага
    };

    // Метод для помещения цепочки правил в стек
    bool Mfst::push_chain(GRB::Rule::Chain chain)
    {
        for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]); // Помещаем символы цепочки в стек
        return true;
    };

    // Метод для сохранения текущего состояния автомата
    bool Mfst::savestate()
    {
        storestate.push(MfstState(lenta_position, st, nrule, nrulechain)); // Сохраняем состояние: позиция на ленте, стек, номера правил
        MFST_TRACE6("SAVESTATE:", storestate.size()); // Отладочное сообщение
        return true;
    };

    // Метод для восстановления предыдущего состояния автомата (сброс)
    bool Mfst::resetstate()
    {
        bool rc = false; // Инициализируем результат
        MfstState state; // Объявляем объект для состояния

        // Если есть сохраненные состояния
        if (rc = (storestate.size() > 0))
        {
            state = storestate.top(); // Извлекаем последнее сохраненное состояние
            lenta_position = state.lenta_position; // Восстанавливаем позицию на ленте
            st = state.st; // Восстанавливаем стек
            nrule = state.nrule; // Восстанавливаем номер правила
            nrulechain = state.nrulechain; // Восстанавливаем номер цепочки
            storestate.pop(); // Убираем сохраненное состояние из стека

            MFST_TRACE5("RESSTATE") // Отладочное сообщение
                MFST_TRACE2 // Дополнительное отладочное сообщение
        }
        return rc; // Возвращаем результат: true, если состояние восстановлено, иначе false
    };

    // Метод для сохранения диагноза в процессе разбора
    bool Mfst::savediagnosis(RC_STEP prc_step)
    {
        bool rc = false;
        short k = 0;

        // Найти подходящую позицию для сохранения диагноза
        while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
            k++;

        if (rc = (k < MFST_DIAGN_NUMBER))
        {
            diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain); // Сохраняем текущий диагноз

            // Обнуляем следующие позиции в диагнозе
            for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
                diagnosis[i].lenta_position = -1;
        }

        return rc;
    };

    // Метод запуска синтаксического анализа
    bool Mfst::start()
    {
        bool rc = false; // Инициализация результата
        RC_STEP rc_step = SURPRISE; // Инициализация состояния как неизвестного
        char buf[MFST_DIAGN_MAXSIZE]{}; // Буфер для диагностики

        rc_step = step(); // Выполняем первый шаг анализа

        // Продолжаем выполнение шагов, пока результаты показывают допустимые состояния
        while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
            rc_step = step();

        // Обработка результата по завершению анализа
        switch (rc_step)
        {
        case LENTA_END:
        {
            MFST_TRACE4("------>LENTA_END") // Вывод отладочного сообщения
                std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
            sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lex.table[lex.size - 1].sn);
            std::cout << std::setw(4) << std::left << 0 << "всего строк " << lex.table[lex.size - 1].sn << ", синтаксический анализ выполнен без ошибок" << std::endl;
            rc = true;
            break;
        }

        case NS_NORULE:
        {
            MFST_TRACE4("------>NS_NORULE") // Вывод отладочного сообщения
                std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
            std::cout << getDiagnosis(0, buf) << std::endl; // Вывод диагностики
            std::cout << getDiagnosis(1, buf) << std::endl;
            std::cout << getDiagnosis(2, buf) << std::endl;
            break;
        }

        case NS_NORULECHAIN:
            MFST_TRACE4("------>NS_NORULECHAIN");
            break;
        case NS_ERROR:
            MFST_TRACE4("------>NS_ERROR");
            break;
        case SURPRISE:
            MFST_TRACE4("------>NS_SURPRISE");
            break;
        }

        return rc; // Возвращаем результат: true, если синтаксический анализ завершен без ошибок
    };

    // Метод для получения содержимого стека в виде строки
    char* Mfst::getCSt(char* buf)
    {
        short p;
        for (int k = (signed)st.size() - 1; k >= 0; --k)
        {
            p = st.c[k]; // Получаем элемент из стека
            buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p); // Преобразуем его в символ
        }
        buf[st.size()] = '\0'; // Завершаем строку
        return buf;
    }

    // Метод для получения части ленты (некоторого диапазона) в виде строки
    char* Mfst::getCLenta(char* buf, short pos, short n)
    {
        short i, k = (pos + n < lenta_size) ? pos + n : lenta_size; // Определяем границу диапазона
        for (i = pos; i < k; i++)
            buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]); // Преобразуем элементы в символы
        buf[i - pos] = 0x00; // Завершаем строку
        return buf;
    }

    // Метод для получения строки с описанием ошибки (диагноза) по её индексу
    char* Mfst::getDiagnosis(short n, char* buf)
    {
        char* rc = new char[200] {}; // Выделение памяти для строки
        int errid = 0;
        int lpos = -1;

        // Если диагноз существует и позиция ленты указана
        if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
        {
            errid = grebach.getRule(diagnosis[n].nrule).iderror; // Получаем идентификатор ошибки
            Error::ERROR err = Error::geterror(errid); // Получаем описание ошибки
            sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.table[lpos].sn, err.message); // Формируем строку с ошибкой
            rc = buf; // Возвращаем её
        }

        return rc;
    }

    // Метод для вывода всех правил, которые были применены в процессе разбора
    void Mfst::printrules()
    {
        MfstState state; // Для хранения состояния
        GRB::Rule rule; // Для хранения правила

        // Перебираем все сохраненные состояния
        for (unsigned short i = 0; i < storestate.size(); i++)
        {
            state = storestate.c[i]; // Получаем состояние
            rule = grebach.getRule(state.nrule); // Получаем правило для этого состояния
            MFST_TRACE7 // Отладочный вывод состояния и правил
        };
    };

    // Метод для сохранения дедукции (последовательности правил и цепочек)
    bool Mfst::savededucation()
    {
        MfstState state;
        GRB::Rule rule;

        deducation.nrules = new short[deducation.size = storestate.size()]; // Выделение памяти под правила
        deducation.nrulechains = new short[deducation.size]; // Выделение памяти под цепочки правил

        // Записываем применённые правила и их цепочки
        for (unsigned short i = 0; i < storestate.size(); i++)
        {
            state = storestate.c[i];
            deducation.nrules[i] = state.nrule; // Сохраняем номер правила
            deducation.nrulechains[i] = state.nrulechain; // Сохраняем номер цепочки
        }
        return true;
    }
}
