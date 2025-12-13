#pragma once
// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <cassert> 
#include <stdexcept>
#include <algorithm>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;
const int MAX_SIZE_T = 4294967295;



// Динамический вектор - 
// шаблонный вектор на динамической памяти
#pragma region vector header
template<typename T = int>
class TDynamicVector
{
protected:
    size_t sz;
    T* pMem;

    /*виртуальные функции чтения из потока и записи в
      поток; их необходимо переопределить в производных
      классах для обеспечения возможности использования
      единых перегруженных операторов << и >> */
    virtual void In(istream&);
    virtual void Out(ostream&);

public:
    TDynamicVector(size_t size = 1);
    TDynamicVector(T* arr, size_t s);
    TDynamicVector(const TDynamicVector<T>& v); // конструктор копирования
    TDynamicVector(TDynamicVector<T>&& v) noexcept;
    ~TDynamicVector();
    TDynamicVector<T>& operator=(const TDynamicVector<T>& v);
    TDynamicVector<T>& operator=(TDynamicVector<T>&& v) noexcept;

    size_t size() const noexcept { return sz; }

    // индексация
    T& operator[](size_t ind);
    const T& operator[](size_t ind) const;
    // индексация с контролем
    T& at(size_t ind);
    const T& at(size_t ind) const;

    // сравнение
    bool IsEqual(void* x);

    bool operator==(const TDynamicVector<T>& v) const noexcept;
    bool operator!=(const TDynamicVector<T>& v) const noexcept;

    // унарные операции
    TDynamicVector<T> operator+() const;
    TDynamicVector<T> operator-() const;

    // скалярные операции
    TDynamicVector<T> operator+(T val);
    TDynamicVector<T> operator-(T val);
    TDynamicVector<T> operator*(T val);

    // векторные операции ВНУТРИ КЛАССА
    TDynamicVector<T> operator+(const TDynamicVector<T>& v);
    TDynamicVector<T> operator-(const TDynamicVector<T>& v);
    T operator*(const TDynamicVector& v);

    template<typename T>
    friend void swap(TDynamicVector<T>& lhs, TDynamicVector<T>& rhs) noexcept;

    // ввод/вывод
    template<typename T>
    friend istream& operator>>(istream& istr, TDynamicVector<T>& v);
    template<typename T>
    friend ostream& operator<<(ostream& ostr, TDynamicVector<T>& v);

    // функция попытки выделения памяти для вектора
    inline void memory_try(size_t size);
};
#pragma endregion

#pragma region vector realization
template<typename T>
TDynamicVector<T>::TDynamicVector(size_t size) : sz(size)
{
    if (sz == 0)
        throw out_of_range("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)
        throw out_of_range("Size exceeds the max allowed size");

    memory_try(sz);

}

template<typename T>
TDynamicVector<T>::TDynamicVector(T* arr, size_t s) : sz(s)
{
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");

    //try {
    //    pMem = new T[sz];
    //}
    //catch (bad_alloc) {
    //    throw("Не хватает памяти \n");
    //}
    memory_try(sz);

    std::copy(arr, arr + sz, pMem);
}

template<typename T>
TDynamicVector<T>::TDynamicVector(const TDynamicVector<T>& v) : sz(v.sz)    // конструктор копирования
{
    /* try {
         pMem = new T[sz];
     }
     catch (bad_alloc) {
         throw("Не хватает памяти \n");
     }*/
    memory_try(sz);
    for (size_t i = 0; i < sz; i++) {
        pMem[i] = v[i];
    }
}

template<typename T>
TDynamicVector<T>::TDynamicVector(TDynamicVector<T>&& v) noexcept : sz(v.sz), pMem(v.pMem)
{
    v.sz = 0;
    v.pMem = nullptr;
}

template<typename T>
TDynamicVector<T>::~TDynamicVector()
{
    delete[] pMem;    // уничтожение динамического массива
}

template<typename T>
T&
TDynamicVector<T>::operator[](size_t ind)
{
    if (ind >= sz)
        throw out_of_range("Index out of range");
    return pMem[ind];

}

template<typename T>
const T&
TDynamicVector<T>::operator[](size_t ind) const
{
    if (ind >= sz)
        throw out_of_range("Index out of range");
    return pMem[ind];

}

// индексация с контролем
template<typename T>
T&
TDynamicVector<T>::at(size_t ind)
{
    if (ind <= 0 && ind >= sz)
        throw out_of_range("Index out of range");
    return pMem[ind];
}

template<typename T>
const T&
TDynamicVector<T>::at(size_t ind) const
{
    if (ind <= 0 && ind >= sz)
        throw out_of_range("Index out of range");
    return pMem[ind];
}

template<typename T>
TDynamicVector<T>&
TDynamicVector<T>::operator=(const TDynamicVector<T>& v)
{
    if (this == &v)
        return *this;
    if (v.sz == 0)
        throw out_of_range("size should be greater than zero");
    TDynamicVector tmp(v);
    swap(*this, tmp);
    return *this;
}

template<typename T>
TDynamicVector<T>&
TDynamicVector<T>::operator=(TDynamicVector<T>&& v) noexcept
{
    if (this == &v)
        return *this;
    delete[] pMem;
    sz = v.sz;
    pMem = v.pMem;
    v.sz = 0;
    v.pMem = nullptr;
    return *this;
}

template<typename T>
bool
TDynamicVector<T>::IsEqual(void* x) {
    if (sz != ((TDynamicVector<T>*)x)->sz)    // при несовпадении размерностей
        return 0;
    for (size_t i = 0; i < sz; i++)
    {
        if ((*this)[i] != (*(TDynamicVector<T>*)x)[i])
            return 0;
    }
    return 1;
}

template<typename T>
bool
TDynamicVector<T>::operator==(const TDynamicVector<T>& v) const noexcept
{
    //return (*this).IsEqual(&v);
    return (sz == v.sz) && std::equal(pMem, pMem + sz, v.pMem);
}

template<typename T>
bool
TDynamicVector<T>::operator!=(const TDynamicVector<T>& v) const noexcept
{
    return !((*this) == v);
}

// унарные операции
template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator+() const {
    return *this;
}

template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator-() const {
    TDynamicVector<T> temp(sz);

    for (size_t i = 0; i < sz; i++)
    {
        temp[i] = -(*this)[i];
    }
    return temp;
}

// скалярные операции
template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator+(T val)
{
    TDynamicVector result(*this);
    for (size_t i = 0; i < sz; ++i)
        result[i] += val;
    return result;
}

template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator-(T val)
{
    TDynamicVector temp(*this);
    for (size_t i = 0; i < sz; ++i)
        temp[i] -= static_cast<T>(val);
    return temp;
}

template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator*(T val)
{
    TDynamicVector temp(*this);
    for (size_t i = 0; i < sz; ++i)
        temp[i] *= static_cast<T>(val);
    return temp;
}

// векторные операции 
template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator+(const TDynamicVector<T>& v)
{
    if (sz != v.sz)
        throw("слагаемые вектора имеют различные длины \n");
    TDynamicVector<T> temp(sz);
    for (size_t i = 0; i < sz; i++)
    {
        temp[i] = pMem[i] + v.pMem[i];
    }
    return temp;
}

template<typename T>
TDynamicVector<T>
TDynamicVector<T>::operator-(const TDynamicVector<T>& v)
{
    return (*this) + (-v);
}

template<typename T>
T
TDynamicVector<T>::operator*(const TDynamicVector<T>& v)
{
    if (sz != v.sz)
        throw  invalid_argument("Vectors must be same size");
    T temp = 0;
    for (size_t i = 0; i < sz; ++i)
        temp += pMem[i] * v.pMem[i];
    return temp;
}

template<typename T>
void swap(TDynamicVector<T>& lhs, TDynamicVector<T>& rhs) noexcept
{
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
}


// виртуальные I/O функции
template<typename T>
void
TDynamicVector<T>::In(istream& is)
{
    for (size_t i = 0; i < sz; i++) {
        is >> (*this)[i];
    }
}

template<typename T>
void
TDynamicVector<T>::Out(ostream& os)
{
    for (size_t i = 0; i < sz; i++) {
        os.precision(100);
        os << (*this)[i] << ' ';
    }
}

// ввод/вывод
template<typename T>
istream& operator>>(istream& istr, TDynamicVector<T>& v)
{
    //for (size_t i = 0; i < v.sz; i++)
    //    istr >> v.pMem[i]; // требуется оператор>> для типа T
    v.In(istr);
    return istr;
}

template<typename T>
ostream& operator<<(ostream& ostr, TDynamicVector<T>& v)
{
    //for (size_t i = 0; i < v.sz; i++)
    //    ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    v.Out(ostr);
    return ostr;
}

// функция попытки выделения памяти для вектора
template<typename T>
void
TDynamicVector<T>::memory_try(size_t size) {
    try {
        pMem = new T[size]();
    }
    catch (bad_alloc) {
        throw ("Не хватает памяти \n");
    }
}
#pragma endregion





// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;

private:
    //TDynamicVector<T>* mtr;


public:
    TDynamicMatrix(size_t s = 1);
    TDynamicMatrix(const TDynamicMatrix& m);

    using TDynamicVector<TDynamicVector<T>>::operator[];
    using TDynamicVector<TDynamicVector<T>>::at;

    // сравнение
    bool operator==(const TDynamicMatrix<T>& m) const noexcept;

    // матрично-скалярные операции
    TDynamicMatrix<T> operator*(const T& val) const;

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v) const;

    // матрично-матричные операции
    TDynamicMatrix<T> operator+(const TDynamicMatrix<T>& m);
    TDynamicMatrix<T> operator-(const TDynamicMatrix<T>& m);
    TDynamicMatrix<T> operator*(const TDynamicMatrix<T>& m);

    // ввод/вывод
    void In(istream&) override;
    void Out(ostream&) override;
    template<typename T>
    friend istream& operator>>(istream& istr, TDynamicMatrix<T>& v);
    template<typename T>
    friend ostream& operator<<(ostream& ostr, TDynamicMatrix<T>& v);
};

#endif

template<typename T>
TDynamicMatrix<T>::TDynamicMatrix(size_t s) : TDynamicVector<TDynamicVector<T>>(s)
{
    if (s == 0)
        throw out_of_range("size should be greater than zero");
    if (s > MAX_MATRIX_SIZE)
        throw out_of_range("The matrix size is larger");
    for (size_t i = 0; i < sz; i++)
        pMem[i] = TDynamicVector<T>(sz);
}

template<typename T>
TDynamicMatrix<T>::TDynamicMatrix(const TDynamicMatrix& m) : TDynamicVector<TDynamicVector<T>>(m.sz) {
    for (size_t i = 0; i < sz; i++) {
        pMem[i] = m.pMem[i];
    }
}

// сравнение
template<typename T>
bool
TDynamicMatrix<T>::operator==(const TDynamicMatrix<T>& m) const noexcept
{
    if (sz != m.sz)
        return false;
    for (size_t i = 0; i < sz; i++) {
        if (pMem[i] != m.pMem[i])
            return false;
    }
    return true;
}

// матрично-скалярные операции
template<typename T>
TDynamicMatrix<T>
TDynamicMatrix<T>::operator*(const T& val) const
{
    TDynamicMatrix<T> temp(sz);
    for (size_t i = 0; i < sz; ++i) {
        temp[i] = pMem[i] * val;
    }
    return temp;
}

// матрично-векторные операции
template<typename T>
TDynamicVector<T>
TDynamicMatrix<T>::operator*(const TDynamicVector<T>& v) const
{
    if (sz != v.size())
        throw invalid_argument("Matrix and vector sizes mismatch");
    TDynamicVector<T> temp(sz);
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = 0; j < sz; j++)
        {
            temp[i] += (*this)[i][j] * v[i];
        }

    return temp;
}

// матрично-матричные операции
template<typename T>
TDynamicMatrix<T>
TDynamicMatrix<T>::operator+(const TDynamicMatrix<T>& m)
{
    if (sz != m.sz)
        throw invalid_argument("Matrix sizes mismatch");
    TDynamicMatrix<T> temp(sz);
    for (size_t i = 0; i < sz; ++i)
        temp[i] = (*this)[i] + m[i];
    return temp;
}

template<typename T>
TDynamicMatrix<T>
TDynamicMatrix<T>::operator-(const TDynamicMatrix<T>& m)
{
    if (sz != m.sz)
        throw invalid_argument("Matrix sizes mismatch");
    TDynamicMatrix<T> temp(sz);
    for (size_t i = 0; i < sz; ++i)
        temp[i] = (*this)[i] - m[i];
    return temp;
}

template<typename T>
TDynamicMatrix<T>
TDynamicMatrix<T>::operator*(const TDynamicMatrix<T>& m)
{
    if (sz != m.sz)
        throw invalid_argument("Matrix sizes mismatch");
    TDynamicMatrix<T> temp(sz);
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = 0; j < sz; ++j) {
            temp[i][j] = T();
            for (size_t k = 0; k < sz; ++k)
                temp[i][j] += (*this)[i][k] * m[k][j];
        } // for j k
    return temp;
}

// ввод/вывод
template<typename T>
void
TDynamicMatrix<T>::In(istream& istr)
{
    for (size_t i = 0; i < sz; ++i)
        istr >> pMem[i];
}

template<typename T>
void
TDynamicMatrix<T>::Out(ostream& ostr)
{
    ostr.precision(100);
    for (size_t i = 0; i < sz; ++i)
        ostr << pMem[i] << endl;
}

template<typename T>
istream& operator>>(istream& istr, TDynamicMatrix<T>& v)
{
    v.In(istr);
    return istr;
}

template<typename T>
ostream& operator<<(ostream& ostr, TDynamicMatrix<T>& v)
{
    v.Out(ostr);
    return ostr;
}