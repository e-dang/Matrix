#pragma once

#include <exception>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#ifndef __clang__
    #include <assert.h>

    #include <algorithm>
#endif

template <typename T>
class Matrix
{
public:
    template <bool Const = false>
    class ForwardRowIterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = std::remove_cv_t<T>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = typename std::conditional_t<Const, const T*, T*>;
        using reference         = typename std::conditional_t<Const, const T&, T&>;

        ForwardRowIterator() : p_iter(nullptr) {}

        ForwardRowIterator(pointer ptr) : p_iter(ptr) {}

        ForwardRowIterator operator++()
        {
            ForwardRowIterator temp = *this;
            p_iter++;
            return temp;
        }

        ForwardRowIterator& operator++(int)
        {
            ++p_iter;
            return *this;
        }

        bool operator==(const ForwardRowIterator& rhs) const { return p_iter == rhs.p_iter; }

        bool operator!=(const ForwardRowIterator& rhs) const { return p_iter != rhs.p_iter; }

        template <bool _Const = Const>
        std::enable_if_t<_Const, reference> operator*() const
        {
            assert(p_iter != nullptr);
            return *p_iter;
        }

        template <bool _Const = Const>
        std::enable_if_t<!_Const, reference> operator*()
        {
            assert(p_iter != nullptr);
            return *p_iter;
        }

    private:
        pointer p_iter;
    };

    template <bool Const = false>
    class ForwardColumnIterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = std::remove_cv_t<T>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = typename std::conditional_t<Const, const T*, T*>;
        using reference         = typename std::conditional_t<Const, const T&, T&>;

        ForwardColumnIterator() : p_iter(nullptr), m_cols(0) {}

        ForwardColumnIterator(pointer ptr, int64_t cols) : p_iter(ptr), m_cols(cols) {}

        ForwardColumnIterator operator++()
        {
            ForwardColumnIterator temp = *this;
            p_iter += m_cols;
            return temp;
        }

        ForwardColumnIterator& operator++(int)
        {
            p_iter += m_cols;
            return *this;
        }

        bool operator==(const ForwardColumnIterator& rhs) const { return p_iter == rhs.p_iter; }

        bool operator!=(const ForwardColumnIterator& rhs) const { return p_iter != rhs.p_iter; }

        template <bool _Const = Const>
        std::enable_if_t<_Const, reference> operator*() const
        {
            assert(p_iter != nullptr);
            return *p_iter;
        }

        template <bool _Const = Const>
        std::enable_if_t<!_Const, reference> operator*()
        {
            assert(p_iter != nullptr);
            return *p_iter;
        }

    private:
        pointer p_iter;
        int64_t m_cols;
    };

    typedef T value_type;
    typedef ForwardRowIterator<false> row_iterator;
    typedef ForwardRowIterator<true> const_row_iterator;
    typedef ForwardColumnIterator<false> col_iterator;
    typedef ForwardColumnIterator<true> const_col_iterator;

    Matrix();

    Matrix(const int64_t rows, const int64_t cols, const bool autoReserve = false, const T fillVal = 0.0);

    Matrix(const Matrix& other);

    Matrix(Matrix&& other);

    virtual ~Matrix();

    Matrix& operator=(const Matrix& rhs);

    Matrix& operator=(Matrix&& rhs);

    bool operator==(const Matrix& rhs) const;

    bool operator!=(const Matrix& rhs) const;

    template <typename Container>
    inline void append(const Container& datapoint);

    template <typename Container>
    inline void append(const Container&& datapoint);

    template <typename Iter>
    inline void append(Iter begin, Iter end);

    template <typename Container>
    inline void set(const int32_t row, const Container& datapoint);

    template <typename Container>
    inline void set(const int32_t row, const Container&& datapoint);

    template <typename Iter>
    inline void set(const int32_t row, Iter begin, Iter end);

    inline void resize(const int64_t elements);

    inline void fill(const T val);

    inline void clear();

    inline bool empty() const noexcept;

    inline T* at(const int64_t row);

    inline T& at(const int64_t row, const int64_t col);

    inline const T* at(const int64_t row) const;

    inline const T& at(const int64_t row, const int64_t col) const;

    inline std::pair<int32_t, int32_t> find(const T element) const;

    inline row_iterator begin();

    inline const_row_iterator begin() const;

    inline const_row_iterator cbegin() const;

    inline row_iterator rowBegin(const int64_t row);

    inline col_iterator colBegin(const int64_t col);

    inline const_row_iterator crowBegin(const int64_t row) const;

    inline const_col_iterator ccolBegin(const int64_t col) const;

    inline row_iterator end();

    inline const_row_iterator end() const;

    inline const_row_iterator cend() const;

    inline row_iterator rowEnd(const int64_t row);

    inline col_iterator colEnd(const int64_t col);

    inline const_row_iterator crowEnd(const int64_t row) const;

    inline const_col_iterator ccolEnd(const int64_t col) const;

    inline std::vector<T> extractRow(const int64_t row) const;

    inline T* data();

    inline const T* data() const;

    inline int64_t numRows() const noexcept;

    inline int64_t rows() const noexcept;

    inline int64_t cols() const noexcept;

    inline int64_t size() const noexcept;

    inline int64_t capacity() const noexcept;

    inline int64_t bytes() const noexcept;

    inline char* serialize() const noexcept;

protected:
    inline void checkAtCapacity();

private:
    inline void validateDimensions() const;

    inline void validateRows() const;

    inline void validateCols() const;

    inline void allocate(const bool autoSize, const T fillVal);

protected:
    int64_t m_rows;
    int64_t m_cols;
    int64_t m_capacity;
    int64_t m_numRows;
    int64_t m_size;
    T* p_data;
};

template <typename T>
Matrix<T>::Matrix() : m_rows(0), m_cols(0), m_capacity(0), m_numRows(0), m_size(0), p_data(nullptr)
{
}

template <typename T>
Matrix<T>::Matrix(const int64_t rows, const int64_t cols, const bool autoResize, const T fillVal) :
    m_rows(rows), m_cols(cols), m_capacity(rows * cols), m_numRows(0), m_size(0), p_data(nullptr)
{
    validateDimensions();
    allocate(autoResize, fillVal);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) :
    m_rows(other.m_rows),
    m_cols(other.m_cols),
    m_capacity(other.m_capacity),
    m_numRows(other.m_numRows),
    m_size(other.m_size),
    p_data(new T[m_capacity])
{
    std::copy(other.cbegin(), other.cend(), begin());
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) : Matrix(0, 0, false)
{
    *this = std::move(other);
}

template <typename T>
Matrix<T>::~Matrix()
{
    if (p_data != nullptr)
    {
        delete[] p_data;
        p_data = nullptr;
    }
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs)
{
    if (this != &rhs)
    {
        if (p_data != nullptr)
            delete[] p_data;

        m_rows     = rhs.m_rows;
        m_cols     = rhs.m_cols;
        m_capacity = rhs.m_capacity;
        m_numRows  = rhs.m_numRows;
        m_size     = rhs.m_size;
        p_data     = new T[m_capacity];
        std::copy(rhs.cbegin(), rhs.cend(), begin());
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& rhs)
{
    if (this != &rhs)
    {
        if (p_data != nullptr)
            delete[] p_data;

        m_rows     = rhs.m_rows;
        m_cols     = rhs.m_cols;
        m_capacity = rhs.m_capacity;
        m_numRows  = rhs.m_numRows;
        m_size     = rhs.m_size;
        p_data     = rhs.p_data;

        rhs.m_rows     = 0;
        rhs.m_cols     = 0;
        rhs.m_capacity = 0;
        rhs.m_numRows  = 0;
        rhs.m_size     = 0;
        rhs.p_data     = nullptr;
    }

    return *this;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& rhs) const
{
    if (m_rows != rhs.m_rows)
        return false;

    if (m_cols != rhs.m_cols)
        return false;

    if (m_capacity != rhs.m_capacity)
        return false;

    if (m_numRows != rhs.m_numRows)
        return false;

    if (m_size != rhs.m_size)
        return false;

    for (int64_t i = 0; i < size(); ++i)
    {
        if (p_data[i] != rhs.p_data[i])
            return false;
    }

    return true;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& rhs) const
{
    return !(*this == rhs);
}

template <typename T>
template <typename Container>
void Matrix<T>::append(const Container& datapoint)
{
    checkAtCapacity();

    std::copy(datapoint.cbegin(), datapoint.cend(), p_data + m_size);
    ++m_numRows;
    m_size += m_cols;
}

template <typename T>
template <typename Container>
void Matrix<T>::append(const Container&& datapoint)
{
    checkAtCapacity();

    std::copy(datapoint.cbegin(), datapoint.cend(), p_data + m_size);
    ++m_numRows;
    m_size += m_cols;
}

template <typename T>
template <typename Iter>
void Matrix<T>::append(Iter begin, Iter end)
{
    checkAtCapacity();

    std::copy(begin, end, p_data + m_size);
    ++m_numRows;
    m_size += m_cols;
}

template <typename T>
template <typename Container>
void Matrix<T>::set(const int32_t row, const Container& datapoint)
{
    std::copy(datapoint.cbegin(), datapoint.cend(), rowBegin(row));
}

template <typename T>
template <typename Container>
void Matrix<T>::set(const int32_t row, const Container&& datapoint)
{
    std::copy(datapoint.cbegin(), datapoint.cend(), rowBegin(row));
}

template <typename T>
template <typename Iter>
void Matrix<T>::set(const int32_t row, Iter begin, Iter end)
{
    std::copy(begin, end, rowBegin(row));
}

template <typename T>
void Matrix<T>::resize(const int64_t rows)
{
    if (rows < 0 || rows > m_rows)
        throw std::length_error("Cannot resize to size less than zero or greater than the capacity.");

    m_numRows = rows;
    m_size    = rows * m_cols;
}

template <typename T>
void Matrix<T>::fill(const T val)
{
    std::fill(begin(), end(), val);
}

template <typename T>
void Matrix<T>::clear()
{
    m_size    = 0;
    m_numRows = 0;
}

template <typename T>
bool Matrix<T>::empty() const noexcept
{
    return !static_cast<bool>(m_size);
}

template <typename T>
T* Matrix<T>::at(const int64_t row)
{
    return const_cast<T*>(static_cast<const Matrix<T>&>(*this).at(row));
}

template <typename T>
T& Matrix<T>::at(const int64_t row, const int64_t col)
{
    return const_cast<T&>(static_cast<const Matrix<T>&>(*this).at(row, col));
}

template <typename T>
const T* Matrix<T>::at(const int64_t row) const
{
#ifdef DEBUG
    if (row >= m_rows || row < 0)
        throw std::out_of_range("Row index " + std::to_string(row) + " out of range [0, " + std::to_string(m_rows - 1) +
                                "].");
#endif

    return p_data + (row * m_cols);
}

template <typename T>
const T& Matrix<T>::at(const int64_t row, const int64_t col) const
{
#ifdef DEBUG
    if (col >= m_cols || col < 0)
        throw std::out_of_range("Column index " + std::to_string(col) + " out of range [0, " +
                                std::to_string(m_cols - 1) + "].");
#endif

    return *(at(row) + col);
}

template <typename T>
std::pair<int32_t, int32_t> Matrix<T>::find(const T element) const
{
    auto iter     = std::find(cbegin(), cend(), element);
    auto distance = std::distance(cbegin(), iter);
    return std::make_pair<int32_t, int32_t>(distance / m_cols, distance % m_cols);  // row, col
}

template <typename T>
typename Matrix<T>::row_iterator Matrix<T>::begin()
{
    return Matrix<T>::row_iterator(p_data);
}

template <typename T>
typename Matrix<T>::const_row_iterator Matrix<T>::begin() const
{
    return Matrix<T>::const_row_iterator(p_data);
}

template <typename T>
typename Matrix<T>::const_row_iterator Matrix<T>::cbegin() const
{
    return Matrix<T>::const_row_iterator(p_data);
}

template <typename T>
typename Matrix<T>::col_iterator Matrix<T>::colBegin(const int64_t col)
{
    return Matrix<T>::col_iterator(p_data + col, m_cols);
}

template <typename T>
typename Matrix<T>::row_iterator Matrix<T>::rowBegin(const int64_t row)
{
    return Matrix<T>::row_iterator(at(row));
}

template <typename T>
typename Matrix<T>::const_row_iterator Matrix<T>::crowBegin(const int64_t row) const
{
    return Matrix<T>::const_row_iterator(at(row));
}

template <typename T>
typename Matrix<T>::const_col_iterator Matrix<T>::ccolBegin(const int64_t col) const
{
    return Matrix<T>::const_col_iterator(p_data + col, m_cols);
}

template <typename T>
typename Matrix<T>::row_iterator Matrix<T>::end()
{
    return Matrix<T>::row_iterator(p_data + capacity());
}

template <typename T>
typename Matrix<T>::const_row_iterator Matrix<T>::end() const
{
    return Matrix<T>::const_row_iterator(p_data + capacity());
}

template <typename T>
typename Matrix<T>::const_row_iterator Matrix<T>::cend() const
{
    return Matrix<T>::const_row_iterator(p_data + capacity());
}

template <typename T>
typename Matrix<T>::row_iterator Matrix<T>::rowEnd(const int64_t row)
{
    return Matrix<T>::row_iterator(at(row) + m_cols);
}

template <typename T>
typename Matrix<T>::col_iterator Matrix<T>::colEnd(const int64_t col)
{
    return Matrix<T>::col_iterator(p_data + col + (m_numRows * m_cols), m_cols);
}

template <typename T>
typename Matrix<T>::const_row_iterator Matrix<T>::crowEnd(const int64_t row) const
{
    return Matrix<T>::const_row_iterator(at(row) + m_cols);
}

template <typename T>
typename Matrix<T>::const_col_iterator Matrix<T>::ccolEnd(const int64_t col) const
{
    return Matrix<T>::const_col_iterator(p_data + col + (m_numRows * m_cols), m_cols);
}

template <typename T>
std::vector<T> Matrix<T>::extractRow(const int64_t row) const
{
    return std::vector<T>(crowBegin(row), crowEnd(row));
}

template <typename T>
T* Matrix<T>::data()
{
    return p_data;
}

template <typename T>
const T* Matrix<T>::data() const
{
    return p_data;
}

template <typename T>
int64_t Matrix<T>::numRows() const noexcept
{
    return m_numRows;
}

template <typename T>
int64_t Matrix<T>::rows() const noexcept
{
    return m_rows;
}

template <typename T>
int64_t Matrix<T>::cols() const noexcept
{
    return m_cols;
}

template <typename T>
int64_t Matrix<T>::size() const noexcept
{
    return m_size;
}

template <typename T>
int64_t Matrix<T>::capacity() const noexcept
{
    return m_capacity;
}

template <typename T>
int64_t Matrix<T>::bytes() const noexcept
{
    return m_capacity * static_cast<int64_t>(sizeof(T));
}

template <typename T>
char* Matrix<T>::serialize() const noexcept
{
    return reinterpret_cast<char*>(p_data);
}

template <typename T>
void Matrix<T>::validateDimensions() const
{
    validateRows();
    validateCols();
}

template <typename T>
void Matrix<T>::validateRows() const
{
    if (m_rows < 0)
        throw std::length_error("The number of rows in a matrix must be positive. Provided number of rows: " +
                                std::to_string(m_rows));
}

template <typename T>
void Matrix<T>::validateCols() const
{
    if (m_cols < 0)
        throw std::length_error("The number of cols in a matrix must be positive. Provided number of cols: " +
                                std::to_string(m_cols));
}

template <typename T>
void Matrix<T>::allocate(const bool autoresize, const T fillVal)
{
    p_data = new T[m_capacity];
    if (autoresize)
    {
        resize(m_rows);
        fill(fillVal);
    }
}

template <typename T>
void Matrix<T>::checkAtCapacity()
{
    if (m_size >= m_capacity)
        throw std::length_error("Cannot add data to full Matrix.");
}