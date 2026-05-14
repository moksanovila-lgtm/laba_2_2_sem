#include "SequenceController.hpp"
#include "exceptions.hpp"
#include <QString>

SequenceController::SequenceController(QObject* parent) : QObject(parent) {}

SequenceController::~SequenceController() {
    delete seq;
    delete bitSeq;
}

void SequenceController::copyFrom(const SequenceController* other) {
    if (other->type == TYPE_ARRAY_MUTABLE || other->type == TYPE_ARRAY_IMMUTABLE) {
        createArray(other->isMutable());
        IEnumerator<int>* it = other->getEnumerator();
        while (it->MoveNext()) {
            append(QString::number(it->Current()));
        }
        delete it;
    } else if (other->type == TYPE_LIST_MUTABLE || other->type == TYPE_LIST_IMMUTABLE) {
        createList(other->isMutable());
        IEnumerator<int>* it = other->getEnumerator();
        while (it->MoveNext()) {
            append(QString::number(it->Current()));
        }
        delete it;
    } else if (other->type == TYPE_BIT_MUTABLE || other->type == TYPE_BIT_IMMUTABLE) {
        createBit(other->isMutable());
        IEnumerator<Bit>* it = other->getBitEnumerator();
        while (it->MoveNext()) {
            append(QString::number(it->Current() ? 1 : 0));
        }
        delete it;
    }
}

void SequenceController::createArray(bool mutableFlag) {
    delete seq;
    delete bitSeq;
    seq = nullptr;
    bitSeq = nullptr;
    
    if (mutableFlag) {
        type = TYPE_ARRAY_MUTABLE;
        seq = new ArraySequence<int>();
    } else {
        type = TYPE_ARRAY_IMMUTABLE;
        seq = new ImmutableArraySequence<int>();
    }
    emit dataChanged();
}

void SequenceController::createList(bool mutableFlag) {
    delete seq;
    delete bitSeq;
    seq = nullptr;
    bitSeq = nullptr;
    
    if (mutableFlag) {
        type = TYPE_LIST_MUTABLE;
        seq = new ListSequence<int>();
    } else {
        type = TYPE_LIST_IMMUTABLE;
        seq = new ImmutableListSequence<int>();
    }
    emit dataChanged();
}

void SequenceController::createBit(bool mutableFlag) {
    delete seq;
    delete bitSeq;
    seq = nullptr;
    bitSeq = nullptr;
    
    if (mutableFlag) {
        type = TYPE_BIT_MUTABLE;
        bitSeq = new BitSequence();
    } else {
        type = TYPE_BIT_IMMUTABLE;
        bitSeq = new ImmutableBitSequence();
    }
    emit dataChanged();
}

bool SequenceController::isMutable() const {
    return (type == TYPE_ARRAY_MUTABLE || 
            type == TYPE_LIST_MUTABLE || 
            type == TYPE_BIT_MUTABLE);
}

size_t SequenceController::getSize() const {
    if (type == TYPE_BIT_MUTABLE || type == TYPE_BIT_IMMUTABLE) {
        return bitSeq ? bitSeq->GetCount() : 0;
    }
    return seq ? seq->GetCount() : 0;
}

IEnumerator<int>* SequenceController::getEnumerator() const {
    return seq ? seq->GetEnumerator() : nullptr;
}

IEnumerator<Bit>* SequenceController::getBitEnumerator() const {
    return bitSeq ? bitSeq->GetEnumerator() : nullptr;
}

void SequenceController::append(const QString& value) {
    try {
        if (type == TYPE_BIT_IMMUTABLE) {
            if (!bitSeq) { emit error("BitSequence не создана"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Бит должен быть равен 0 или 1"); return; }
            
            BitSequence* mutableCopy = new BitSequence();
            for (size_t i = 0; i < bitSeq->GetCount(); ++i) {
                mutableCopy->Append(bitSeq->Get(i));
            }
            mutableCopy->Append(v == 1);
            
            SequenceController* newController = new SequenceController();
            newController->bitSeq = mutableCopy;
            newController->type = TYPE_BIT_MUTABLE;
            
            emit newSequenceCreated(newController, "Append result (mutable)");
            return;
        }
        
        if (type == TYPE_ARRAY_IMMUTABLE || type == TYPE_LIST_IMMUTABLE) {
            if (!seq) { emit error("Последовательность не создана"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Недопустимое целое значение"); return; }
            
            Sequence<int>* mutableCopy = new ArraySequence<int>();
            for (size_t i = 0; i < seq->GetCount(); ++i) {
                mutableCopy->Append(seq->Get(i));
            }
            mutableCopy->Append(v);
            
            SequenceController* newController = new SequenceController();
            newController->seq = mutableCopy;
            newController->type = (type == TYPE_ARRAY_IMMUTABLE) ? TYPE_ARRAY_MUTABLE : TYPE_LIST_MUTABLE;
            
            emit newSequenceCreated(newController, "Append result (mutable)");
            return;
        }
        
        if (type == TYPE_BIT_MUTABLE) {
            if (!bitSeq) { emit error("BitSequence не создана"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Бит должен быть равен 0 или 1"); return; }
            bitSeq->Append(v == 1);
            emit dataChanged();  
        } else if (seq) {
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Недопустимое целое значение"); return; }
            seq->Append(v);
            emit dataChanged();  
        } else {
            emit error("Sequence не создана");
            return;
        }
        
    } catch (const std::exception& e) {
        emit error(QString("Append error: ") + e.what());
    }
}

void SequenceController::prepend(const QString& value) {
    try {
        if (type == TYPE_BIT_IMMUTABLE) {
            if (!bitSeq) { emit error("BitSequence не создана"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Бит должен быть равен 0 или 1"); return; }
            
            BitSequence* mutableCopy = new BitSequence();
            mutableCopy->Append(v == 1);
            for (size_t i = 0; i < bitSeq->GetCount(); ++i) {
                mutableCopy->Append(bitSeq->Get(i));
            }
            
            SequenceController* newController = new SequenceController();
            newController->bitSeq = mutableCopy;
            newController->type = TYPE_BIT_MUTABLE;
            
            emit newSequenceCreated(newController, "Prepend result (mutable)");
            return;
        }
        
        if (type == TYPE_ARRAY_IMMUTABLE || type == TYPE_LIST_IMMUTABLE) {
            if (!seq) { emit error("Последовательность не создана"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Недопустимое целое значение"); return; }
            
            Sequence<int>* mutableCopy = new ArraySequence<int>();
            mutableCopy->Append(v);
            for (size_t i = 0; i < seq->GetCount(); ++i) {
                mutableCopy->Append(seq->Get(i));
            }
            
            SequenceController* newController = new SequenceController();
            newController->seq = mutableCopy;
            newController->type = (type == TYPE_ARRAY_IMMUTABLE) ? TYPE_ARRAY_MUTABLE : TYPE_LIST_MUTABLE;
            
            emit newSequenceCreated(newController, "Prepend result (mutable)");
            return;
        }
        
        if (type == TYPE_BIT_MUTABLE) {
            if (!bitSeq) { emit error("BitSequence не создана"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Бит должен быть равен 0 или 1"); return; }
            bitSeq->Prepend(v == 1);
            emit dataChanged();
        } else if (seq) {
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Недопустимое целое значение"); return; }
            seq->Prepend(v);
            emit dataChanged();
        } else {
            emit error("Последовательность не создана");
            return;
        }
        
    } catch (const std::exception& e) {
        emit error(QString("Prepend error: ") + e.what());
    }
}

void SequenceController::insertAt(int index, const QString& value) {
    try {
        if (type == TYPE_BIT_IMMUTABLE) {
            if (!bitSeq) { emit error("BitSequence не создана"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Бит должен быть равен 0 или 1"); return; }
            
            BitSequence* mutableCopy = new BitSequence();
            for (size_t i = 0; i < bitSeq->GetCount(); ++i) {
                if (i == static_cast<size_t>(index)) {
                    mutableCopy->Append(v == 1);
                }
                mutableCopy->Append(bitSeq->Get(i));
            }
            if (index == static_cast<int>(bitSeq->GetCount())) {
                mutableCopy->Append(v == 1);
            }
            
            SequenceController* newController = new SequenceController();
            newController->bitSeq = mutableCopy;
            newController->type = TYPE_BIT_MUTABLE;
            
            emit newSequenceCreated(newController, QString("Insert at %1 result (mutable)").arg(index));
            return;
        }
        
        if (type == TYPE_ARRAY_IMMUTABLE || type == TYPE_LIST_IMMUTABLE) {
            if (!seq) { emit error("Последовательность не создана"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Недопустимое целое значение"); return; }
            
            if (index < 0 || index > static_cast<int>(seq->GetCount())) {
                emit error("Индекс вне диапазона");
                return;
            }
            
            Sequence<int>* mutableCopy = new ArraySequence<int>();
            for (size_t i = 0; i < seq->GetCount(); ++i) {
                if (i == static_cast<size_t>(index)) {
                    mutableCopy->Append(v);
                }
                mutableCopy->Append(seq->Get(i));
            }
            if (index == static_cast<int>(seq->GetCount())) {
                mutableCopy->Append(v);
            }
            
            SequenceController* newController = new SequenceController();
            newController->seq = mutableCopy;
            newController->type = (type == TYPE_ARRAY_IMMUTABLE) ? TYPE_ARRAY_MUTABLE : TYPE_LIST_MUTABLE;
            
            emit newSequenceCreated(newController, QString("Insert at %1 result (mutable)").arg(index));
            return;
        }
        
        if (type == TYPE_BIT_MUTABLE) {
            if (!bitSeq) { emit error("BitSequence не создана"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Бит должен быть равен 0 или 1"); return; }
            bitSeq->InsertAt(v == 1, index);
            emit dataChanged();
        } else if (seq) {
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Недопустимое целое значение"); return; }
            seq->InsertAt(v, index);
            emit dataChanged();
        } else {
            emit error("Последовательность не создана");
            return;
        }
        
    } catch (const std::exception& e) {
        emit error(QString("Insert error: ") + e.what());
    }
}

void SequenceController::map() {
    if (!seq) { emit error("Последовательность не создана"); return; }
    
    try {
        auto doubleFunc = [](const int& x) { return x * 2; };
        Sequence<int>* result = seq->Map(doubleFunc);
        
        SequenceController* newController = new SequenceController();
        newController->seq = result;
        newController->type = TYPE_ARRAY_MUTABLE;  
        
        emit newSequenceCreated(newController, "Map result (mutable)");
    } catch (const std::exception& e) {
        emit error(QString("Map error: ") + e.what());
    }
}

void SequenceController::where() {
    if (!seq) { emit error("Последовательность не создана"); return; }
    
    try {
        auto isEven = [](const int& x) { return x % 2 == 0; };
        Sequence<int>* result = seq->Where(isEven);
        
        SequenceController* newController = new SequenceController();
        newController->seq = result;
        newController->type = TYPE_ARRAY_MUTABLE;
        
        emit newSequenceCreated(newController, "Where (Чётные)");
    } catch (const std::exception& e) {
        emit error(QString("Where error: ") + e.what());
    }
}

void SequenceController::reduce() {
    if (!seq) { emit error("Последовательность не создана"); return; }
    
    try {
        auto sum = [](const int& a, const int& b) { return a + b; };
        int result = seq->Reduce(sum, 0);
        emit error(QString("Reduce (сумма): %1").arg(result));
    } catch (const std::exception& e) {
        emit error(QString("Reduce error: ") + e.what());
    }
}

void SequenceController::concat(const QString& secondSeq) {
    if (!seq) { emit error("Последовательность не создана"); return; }
    
    try {
        QStringList parts = secondSeq.split(' ', Qt::SkipEmptyParts);
        ArraySequence<int> tempSeq;
        for (const QString& part : parts) {
            bool ok;
            int val = part.toInt(&ok);
            if (!ok) { emit error("Некорректное число: " + part); return; }
            tempSeq.Append(val);
        }
        
        Sequence<int>* result = seq->Concat(&tempSeq);
        
        SequenceController* newController = new SequenceController();
        newController->seq = result;
        newController->type = TYPE_ARRAY_MUTABLE;
        
        emit newSequenceCreated(newController, "Concat result");
    } catch (const std::exception& e) {
        emit error(QString("Concat error: ") + e.what());
    }
}

void SequenceController::getSubsequence(int start, int end) {
    if (!seq) { emit error("Последовательность не создана"); return; }
    
    try {
        if (start < 0 || end >= static_cast<int>(seq->GetCount()) || start > end) {
            emit error("Недопустимые индексы");
            return;
        }
        
        Sequence<int>* sub = seq->GetSubsequence(start, end);
        
        SequenceController* newController = new SequenceController();
        newController->seq = sub;
        newController->type = TYPE_ARRAY_MUTABLE;
        
        emit newSequenceCreated(newController, QString("Подпоследовательность [%1..%2]").arg(start).arg(end));
    } catch (const std::exception& e) {
        emit error(QString("Subsequence error: ") + e.what());
    }
}

void SequenceController::mapBit() {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        auto invert = [](const Bit& b) { return !b; };
        Sequence<Bit>* result = bitSeq->Map(invert);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = static_cast<BitSequence*>(result);
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "Map (NOT)");
    } catch (const std::exception& e) {
        emit error(QString("MapBit error: ") + e.what());
    }
}

void SequenceController::whereBit() {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        auto isOne = [](const Bit& b) { return b == true; };
        Sequence<Bit>* result = bitSeq->Where(isOne);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = static_cast<BitSequence*>(result);
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "Where (фильтр единиц)");
    } catch (const std::exception& e) {
        emit error(QString("WhereBit error: ") + e.what());
    }
}

void SequenceController::reduceBit() {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        auto xorFunc = [](const Bit& a, const Bit& b) { return a != b; };
        Bit result = bitSeq->Reduce(xorFunc, Bit(0));
        emit error(QString("Reduce XOR result: %1").arg(result ? 1 : 0));
    } catch (const std::exception& e) {
        emit error(QString("ReduceBit error: ") + e.what());
    }
}

void SequenceController::andBit(const QString& secondSeq) {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        BitSequence other;
        for (QChar ch : secondSeq) {
            if (ch == '0') other.Append(false);
            else if (ch == '1') other.Append(true);
            else { emit error("Некорректные биты: " + secondSeq); return; }
        }
        
        if (bitSeq->GetCount() != other.GetCount()) {
            emit error("Размеры не совпадают");
            return;
        }
        
        BitSequence* result = new BitSequence(*bitSeq);
        result->And(other);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = result;
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "AND result");
    } catch (const std::exception& e) {
        emit error(QString("AND error: ") + e.what());
    }
}

void SequenceController::orBit(const QString& secondSeq) {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        BitSequence other;
        for (QChar ch : secondSeq) {
            if (ch == '0') other.Append(false);
            else if (ch == '1') other.Append(true);
            else { emit error("Некорректные биты: " + secondSeq); return; }
        }
        
        if (bitSeq->GetCount() != other.GetCount()) {
            emit error("Размеры не совпадают");
            return;
        }
        
        BitSequence* result = new BitSequence(*bitSeq);
        result->Or(other);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = result;
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "OR result");
    } catch (const std::exception& e) {
        emit error(QString("OR error: ") + e.what());
    }
}

void SequenceController::xorBit(const QString& secondSeq) {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        BitSequence other;
        for (QChar ch : secondSeq) {
            if (ch == '0') other.Append(false);
            else if (ch == '1') other.Append(true);
            else { emit error("Некорректные биты: " + secondSeq); return; }
        }
        
        if (bitSeq->GetCount() != other.GetCount()) {
            emit error("Размеры не совпадают");
            return;
        }
        
        BitSequence* result = new BitSequence(*bitSeq);
        result->Xor(other);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = result;
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "XOR result");
    } catch (const std::exception& e) {
        emit error(QString("XOR error: ") + e.what());
    }
}

void SequenceController::notBit() {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        BitSequence* result = new BitSequence(*bitSeq);
        result->Not();
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = result;
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "NOT result");
    } catch (const std::exception& e) {
        emit error(QString("NOT error: ") + e.what());
    }
}

void SequenceController::getSubsequenceBit(int start, int end) {
    if (!bitSeq) { emit error("BitSequence не создана"); return; }
    
    try {
        if (start < 0 || end >= static_cast<int>(bitSeq->GetCount()) || start > end) {
            emit error("Некорректные индексы");
            return;
        }
        
        Sequence<Bit>* sub = bitSeq->GetSubsequence(start, end);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = static_cast<BitSequence*>(sub);
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, QString("Подпоследовательность [%1..%2]").arg(start).arg(end));
    } catch (const std::exception& e) {
        emit error(QString("Subsequence error: ") + e.what());
    }
}

void SequenceController::concatBit(const QString& secondSeq) {
    if (!bitSeq) { 
        emit error("BitSequence не создана"); 
        return; 
    }
    
    try {
        BitSequence tempSeq;
        for (QChar ch : secondSeq) {
            if (ch == '0') {
                tempSeq.Append(Bit(0));
            } else if (ch == '1') {
                tempSeq.Append(Bit(1));
            } else {
                emit error("Некорректный бит: " + QString(ch) + ". Используйте только 0 и 1");
                return;
            }
        }
        
        Sequence<Bit>* result = bitSeq->Concat(&tempSeq);
        
        SequenceController* newController = new SequenceController();
        newController->bitSeq = static_cast<BitSequence*>(result);
        newController->type = TYPE_BIT_MUTABLE;
        
        emit newSequenceCreated(newController, "Concat result");
        
    } catch (const std::exception& e) {
        emit error(QString("Concat error: ") + e.what());
    }
}

void SequenceController::initializeFromString(const QString& data) {
    if (type == TYPE_BIT_IMMUTABLE) {
        BitSequence* temp = new BitSequence();
        for (QChar ch : data) {
            if (ch == '0') temp->Append(false);
            else if (ch == '1') temp->Append(true);
        }
        delete bitSeq;
        bitSeq = temp;
        emit dataChanged();
    } else if (type == TYPE_ARRAY_IMMUTABLE || type == TYPE_LIST_IMMUTABLE) {
        Sequence<int>* temp = new ArraySequence<int>();
        QStringList parts = data.split(' ', Qt::SkipEmptyParts);
        for (const QString& part : parts) {
            temp->Append(part.toInt());
        }
        delete seq;
        seq = temp;
        emit dataChanged();
    }
}

void SequenceController::set(int index, const QString& value) {
    try {
        if (type == TYPE_BIT_MUTABLE) {
            if (!bitSeq) { emit error("BitSequence not created"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Bit must be 0 or 1"); return; }
            bitSeq->Set(index, v == 1);
            emit dataChanged();
            return;
        }
        
        if (type == TYPE_BIT_IMMUTABLE) {
            if (!bitSeq) { emit error("BitSequence not created"); return; }
            int v = value.toInt();
            if (v != 0 && v != 1) { emit error("Bit must be 0 or 1"); return; }
            
            BitSequence* mutableCopy = new BitSequence();
            for (size_t i = 0; i < bitSeq->GetCount(); ++i) {
                mutableCopy->Append(bitSeq->Get(i));
            }
            mutableCopy->Set(index, v == 1);
            
            SequenceController* newController = new SequenceController();
            newController->bitSeq = mutableCopy;
            newController->type = TYPE_BIT_MUTABLE;
            emit newSequenceCreated(newController, "Edit result");
            return;
        }
        
        if (type == TYPE_ARRAY_MUTABLE) {
            if (!seq) { emit error("Sequence not created"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Invalid integer value"); return; }
            
            ArraySequence<int>* arrSeq = dynamic_cast<ArraySequence<int>*>(seq);
            if (arrSeq) {
                arrSeq->Set(index, v);
                emit dataChanged();
            }
            return;
        }
        
        if (type == TYPE_LIST_MUTABLE) {
            if (!seq) { emit error("Sequence not created"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Invalid integer value"); return; }
            
            ListSequence<int>* listSeq = dynamic_cast<ListSequence<int>*>(seq);
            if (listSeq) {
                listSeq->Set(index, v);
                emit dataChanged();
            }
            return;
        }
        
        if (type == TYPE_ARRAY_IMMUTABLE) {
            if (!seq) { emit error("Sequence not created"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Invalid integer value"); return; }
            
            ArraySequence<int>* mutableCopy = new ArraySequence<int>();
            for (size_t i = 0; i < seq->GetCount(); ++i) {
                mutableCopy->Append(seq->Get(i));
            }
            mutableCopy->Set(index, v);
            
            SequenceController* newController = new SequenceController();
            newController->seq = mutableCopy;
            newController->type = TYPE_ARRAY_MUTABLE;
            emit newSequenceCreated(newController, "Edit result");
            return;
        }
        
        if (type == TYPE_LIST_IMMUTABLE) {
            if (!seq) { emit error("Sequence not created"); return; }
            bool ok;
            int v = value.toInt(&ok);
            if (!ok) { emit error("Invalid integer value"); return; }
            
            ListSequence<int>* mutableCopy = new ListSequence<int>();
            for (size_t i = 0; i < seq->GetCount(); ++i) {
                mutableCopy->Append(seq->Get(i));
            }
            mutableCopy->Set(index, v);
            
            SequenceController* newController = new SequenceController();
            newController->seq = mutableCopy;
            newController->type = TYPE_LIST_MUTABLE;
            emit newSequenceCreated(newController, "Edit result");
            return;
        }
        
        emit error("Sequence not created");
        
    } catch (const std::exception& e) {
        emit error(QString("Edit error: ") + e.what());
    }
}