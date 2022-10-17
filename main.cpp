#include <iostream>

// есть два класса, Row унаследован от MathPrimit.
// не понимаю, почему в некоторых случаях он нормально использует
// методы родительского класса, а иногда - нет:

// проблема: почему ему, если строки 127-150 закомметированы,
// то строка 177 воспринимается как ошибка,
// а если их все раскомментировать, то код скомпилируется?

// другими словами, почему при обычном присваивании (перемещении и копировании)
// дочерний класс спокойно ходит по строкам кода родительского класса, а при
// вызове метода addition и попытке присваивания результата в row он не идёт
// через оператор присваивания - перемещения?

// при выполнении функций 171 - 176 действия происходят по родительскому классу.


template<typename T>
class MathPrimit {
public:

    T **_primits;
    unsigned _numOfRows;
    unsigned _numOfColumns;

    MathPrimit()
            : _primits(nullptr), _numOfRows(0), _numOfColumns(0) {}

    //конструктор переноса
    MathPrimit(const unsigned &numOfRows, const unsigned &numOfColumns) {
        _numOfRows = numOfRows;
        _numOfColumns = numOfColumns;
        _primits = new T *[_numOfRows];
        for (unsigned i = 0; i < _numOfRows; ++i) {
            _primits[i] = new T[_numOfColumns];
            for (int j = 0; j < _numOfColumns; ++j) {
                _primits[i][j] = 0;
            }
        }
    }

    //конструктор переноса
    MathPrimit(MathPrimit &&other) noexcept {
        _numOfRows = other._numOfRows;
        _numOfColumns = other._numOfColumns;
        _primits = other._primits;

        other._numOfRows = 0;
        other._numOfColumns = 0;
        other._primits = nullptr;
    }

    //конструктор копирования
    MathPrimit(const MathPrimit &other) {
        _numOfRows = other._numOfRows;
        _numOfColumns = other._numOfColumns;

        _primits = new T *[_numOfRows];
        for (unsigned i = 0; i < _numOfRows; ++i) {
            _primits[i] = new T[_numOfColumns];
            for (int j = 0; j < _numOfColumns; ++j) {
                _primits[i][j] = other._primits[i][j];
            }
        }
    }

    //оператор присваивания - копирования
    virtual MathPrimit &operator=(const MathPrimit &other) {
        destroyThis();
        _numOfRows = other._numOfRows;
        _numOfColumns = other._numOfColumns;
        _primits = new T *[_numOfRows];
        for (unsigned i = 0; i < _numOfRows; ++i) {
            _primits[i] = new T[_numOfColumns];
            for (int j = 0; j < _numOfColumns; ++j) {
                _primits[i][j] = other._primits[i][j];
            }
        }
        return *this;
    }

    //оператор присваивания - переноса
    virtual MathPrimit &operator=(MathPrimit &&other) noexcept {
        destroyThis();
        _numOfRows = other._numOfRows;
        _numOfColumns = other._numOfColumns;
        _primits = other._primits;
        other._numOfRows = 0;
        other._numOfColumns = 0;
        other._primits = nullptr;
        return *this;
    }

    //сложение
    MathPrimit<T> addition(const MathPrimit term) const {
        MathPrimit result(this->_numOfRows, this->_numOfColumns);
        for (unsigned i = 0; i < this->_numOfRows; ++i)
            for (unsigned j = 0; j < this->_numOfColumns; ++j) {
                result._primits[i][j] = this->_primits[i][j] + term._primits[i][j];
            }
        return result;
    }

    ~MathPrimit() {
        destroyThis();
    }

    void destroyThis() {
        for (int i = 0; i < _numOfRows; ++i) {
            delete _primits[i];
        }
        delete _primits;
        _numOfRows = 0;
        _numOfColumns = 0;
        _primits = nullptr;
    }
};

template<typename T>
class Row : public MathPrimit<T> {
public:

    explicit Row(const unsigned &numOfElements)
            : MathPrimit<T>(1, numOfElements) {}

//    Row &operator=(MathPrimit<T> const &other) noexcept  {
//        this->destroyThis();
//        this->_numOfRows = other._numOfRows;
//        this->_numOfColumns = other._numOfColumns;
//        this->_primits = new T *[this->_numOfRows];
//        for (unsigned i = 0; i < this->_numOfRows; ++i) {
//            this->_primits[i] = new T[this->_numOfColumns];
//            for (int j = 0; j < this->_numOfColumns; ++j) {
//                this->_primits[i][j] = other._primits[i][j];
//            }
//        }
//        return *this;
//    }
//
//    Row &operator=(MathPrimit<T> &&other) noexcept  {
//        this->destroyThis();
//        this->_numOfRows = other._numOfRows;
//        this->_numOfColumns = other._numOfColumns;
//        this->_primits = other._primits;
//        other._numOfRows = 0;
//        other._numOfColumns = 0;
//        other._primits = nullptr;
//        return *this;
//    }

    MathPrimit<T> toBase() {
        return MathPrimit<T>(*this);
    }

    friend std::ostream &operator<<(std::ostream &strm, Row<T> &row) {
        return strm;
    }
};

int main() {

    MathPrimit<double> mp(1,4);
    MathPrimit<double> mp2(mp);
    MathPrimit<double> mp3(mp2);

    mp = mp2;
    mp = std::move(mp2);
    mp = mp3.addition(mp);

    Row<double> row(4);
    Row<double> row2(row);
    Row<double> row3(row2);

    row = row2;
    row = std::move(row2);
    row = row3.addition(row); //?

    return 0;
}
