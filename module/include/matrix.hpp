#pragma once
#include <iostream>
#include <ostream>
#include <vector>
#include <cassert>

#include "compute.hpp"

namespace type
{
    template <typename T>
    class matrix
    {
    private:
        static constexpr size_t DEFAULT_CAP = 32;
        T *container;
        int m_size;
        int m_capacity;

    public:
        matrix(const int &cap = DEFAULT_CAP)
            : container(new T[cap]), m_size(0), m_capacity(cap){};
        matrix(const int &size, const int &want)
        {
            container = new T[size];
            for (int i = 0; i < size; ++i)
            {
                container[i] = want;
            }
            m_size = size;
        }
        auto operator=(std::initializer_list<T> l)
        {
            int index = 0;

            for(auto itr = l.begin(); itr != l.end(); ++itr)
            {
                container[index] = *itr;
                ++index;
            }
            return container;
        }

        int size()
        { // vector.size()
            return m_size;
        }
        int capacity()
        { // vector.capacty()
            return m_capacity;
        }
        T &front()
        { // vector.front() 맨 앞 원소 위치
            return container[0];
        }
        T &back()
        { // vector.back()
            return container[m_size - 1];
        }
        T *begin()
        { // vector.begin()
            return container;
        }
        T *end()
        { // vector.end()
            return container + m_size;
        }
        void push_back(const T &elem)
        {
            if (m_capacity <= m_size)
            {
                m_capacity *= 2;
                T *temp_container = new T[m_capacity];

                for (int i = 0; i < m_size; ++i)
                {
                    temp_container[i] = container[i];
                }
                delete[] container;
                container = temp_container;
            }
            container[++m_size] = elem;
        }
        void pop_back()
        {
            m_size = m_size > 0 ? m_size - 1 : 0; //사이즈만 줄이면 어차피 push때 vec[size]값에 넣으므로
        }
        void resize(const int &resize, T val = 0)
        {
            T *temp_container = new T[resize];
            m_size = m_size > resize ? resize : m_size;
            m_capacity = resize;

            for (int i = 0; i < m_size; ++i)
            {
                temp_container[i] = container[i];
            }
            for (int i = m_size; i < m_capacity; ++i)
            {
                temp_container[i] = val;
            }
            delete[] container;
            container = temp_container;
            m_size = resize;
        }
        void reserve(const int &size)
        {
            if (size < m_capacity)
                return;

            T *tmp_container = new T[size];
            for (int i = 0; i < m_size; ++i)
            {
                tmp_container[i] = container[i];
            }
            delete[] container;
            container = tmp_container;
            m_capacity = size;
        }
        void swap(matrix &other)
        {
            swap(container, other.vec);
            swap(m_size, other.m_size);
            swap(m_capacity, other.m_capacity);
        }

        friend std::ostream &operator<<(std::ostream &out, matrix<T> con)
        {
            out << "[";
            for (int i = 0; i < con.size(); ++i)
            {
                out << con[i];

                if (i + 1 != con.size())
                {
                    out << ", ";
                }
            }
            out << "]";

            return out;
        }
        std::ostream &endl(std::ostream &end)
        {
            end << '\n';
            return end;
        }
        // auto &operator=(matrix<T> other)
        // {
        //     if (this != &other)
        //     {
        //         if (m_capacity < other.m_capacity)
        //         {
        //             delete[] container;
        //             container = new T[other.m_capacity];
        //         }
        //     }
        //     auto size = other.m_size;
        //     for (int i = 0; i < size; ++i)
        //     {
        //         container[i] = other[i];
        //     }
        //     return *this;
        // }
        T &operator[](const int &index)
        {
            // index = index >= 0 ? index : this->m_size + index;
            return container[index];
        }
        bool operator!=(matrix &other)
        {
            return (!(*this == other));
        }
    };

    template <typename T>
    class matrix_t
    {
    private:
        static constexpr size_t DEFAULT_CAP = 32;
        matrix<T> *container;
        int m_capacity;

    public:
        matrix_t(const int &cap = DEFAULT_CAP)
            : container(new matrix<T>[cap]), rows(0), m_capacity(cap){};
        matrix_t(const int &row, const int &col)
        {
            container = new matrix<T>[row];
            for (int i = 0; i < col; ++i)
            {
                container[i] = matrix<T>(col);
            }

            rows = row;
            cols = col;
        }

        auto size()
        {
            return rows;
        }
        auto capacity()
        {
            return m_capacity;
        }
        auto &front()
        {
            return container[0];
        }
        auto &back()
        {
            return container[rows - 1];
        }
        auto begin()
        { // vector.begin()
            return container;
        }
        auto end()
        { // vector.end()
            return container + rows;
        }
        void push_back(const auto &elem)
        {
            if (m_capacity <= rows)
            {
                m_capacity *= 2;
                auto temp_container = new T *[m_capacity];

                for (int i = 0; i < rows; ++i)
                {
                    temp_container[i] = container[i];
                }
                delete[] container;
                container = temp_container;
            }
            container[++rows] = elem;
        }
        // void pop_back()
        // {
        //     m_size = m_size > 0 ? m_size-1 : 0;				//사이즈만 줄이면 어차피 push때 vec[size]값에 넣으므로
        // }
        void resize(const int &resize, matrix<T> val)
        {
            auto temp_container = new matrix<T>[resize];
            rows = rows > resize ? resize : rows;
            m_capacity = resize;

            for (int r = 0; r < m_capacity; ++r)
            {
                temp_container[r] = matrix<T>(val.size());
                if (r < rows)
                {
                    for (int c = 0; c < val.size(); ++c)
                    {
                        if (c < cols)
                        {
                            temp_container[r][c] = container[r][c];
                        }
                        else
                        {
                            temp_container[r][c] = val[c];
                        }
                    }
                }
                else
                {
                    temp_container[r] = val;
                }
            }

            delete[] container;
            container = temp_container;
            rows = resize;
            cols = val.size();
        }
        // void reserve(const int& size)
        // {
        //     if (size < m_capacity) return;
        //     T* tmp_container = new T[size];
        //     for (int i = 0; i < m_size; ++i) {
        //         tmp_container[i] = container[i];
        //     }
        //     delete[] container;
        //     container = tmp_container;
        //     m_capacity = size;
        // }
        // void swap(matrix& other)
        // {
        //     swap(container, other.vec);
        //     swap(m_size, other.m_size);
        //     swap(m_capacity, other.m_capacity);
        // }

        friend std::ostream &operator<<(std::ostream &out, matrix_t<T> con)
        {
            out << "[";
            for (int row = 0; row < con.rows; ++row)
            {
                if (row == 0)
                {
                    out << "[";
                }
                else
                {
                    out << " [";
                }
                for (int col = 0; col < con.cols; ++col)
                {
                    out << con[row][col];

                    if (col + 1 != con.cols)
                    {
                        out << ", ";
                    }
                }
                out << "]";
                if (row + 1 != con.rows)
                {
                    out << ",\n";
                }
            }
            out << "]";

            return out;
        }
        std::ostream &endl(std::ostream &end)
        {
            end << '\n';
            return end;
        }

        // matrix_t<T>& operator=(const matrix_t<T>& other)
        // {
        //     if (this != &other)
        //     {
        //         if (m_capacity < other.m_capacity)
        //         {
        //             delete[] container;
        //             container = new matrix<T>[other.m_capacity];
        //         }
        //     }
        //     rows = other.rows;
        //     for (int i = 0; i < rows; ++i)
        //     {
        //         container[i] = other[i];
        //     }
        //     return *this;
        // }

        // auto& operator=(matrix_t<T>&& other)
        // {
        //     std::swap(container, other);
        //     std::swap(rows, other.rows);
        //     std::swap(m_capacity, other.m_capacity);
        //     other.m_size = 0;
        //     return *this;
        // }
        auto& operator[](const int &index)
        {
            // index = index >= 0 ? index : this->rows + index;
            if (0 <= index && index < rows)
                return *(container + index);
            throw std::out_of_range("Out of bounds element access");
        }
        bool operator==(const matrix<T> &other)
        {
            if (m_capacity != other.capacity)
            {
                return false;
            }
            for (int i = 0; i < rows; i++)
            {
                if (container[i] != other[i])
                    return false;
            }
            return true;
        }
        bool operator!=(const matrix<T> &other)
        {
            return (!(*this == other));
        }

        auto operator=(std::initializer_list<T> l)
        {
            int r = 0, c = 0;

            for (auto itr = l.begin(); itr != l.end(); ++itr)
            {
                container[r][c] = *itr;
                if (r < rows)
                {
                    if (c < cols - 1)
                    {
                        ++c;
                    }
                    else
                    {
                        c = 0;
                        ++r;
                    }
                }
            }
            return container;
        }
        auto &operator()(int row, int col)
        {
            return container[row][col];
        }

        friend auto operator*(matrix_t<T> a, matrix_t<T> b)
        {
            assert(a.cols == b.rows);

            matrix_t<T> res(a.rows, b.cols);
            for (int lrow = 0; lrow < a.rows; ++lrow)
            {
                for (int rcol = 0; rcol < b.cols; ++rcol)
                {
                    T elem = 0;
                    for (int rrow = 0; rrow < b.rows; ++rrow)
                    {
                        elem += a(lrow, rrow) * b(rrow, rcol);
                    }
                    res(lrow, rcol) = elem;
                }
            }
            return res;
        }
        friend auto operator+(matrix_t<T> a, matrix_t<T> b)
        {
            assert(a.rows == b.rows && a.cols == b.cols);
            matrix_t<T> res(a.rows, a.cols);

            for (int r = 0; r < a.rows; ++r)
            {
                for (int c = 0; c < a.cols; ++c)
                {
                    res(r, c) = a(r, c) + b(r, c);
                }
            }
            return res;
        }
        friend auto operator-(matrix_t<T> a, matrix_t<T> b)
        {
            assert(a.rows == b.rows && a.cols == b.cols);
            matrix_t<T> res(a.rows, a.cols);

            for (int r = 0; r < a.rows; ++r)
            {
                for (int c = 0; c < a.cols; ++c)
                {
                    res(r, c) = a(r, c) - b(r, c);
                }
            }
            return res;
        }
        friend auto operator*(matrix_t<T> a, auto scalar)
        {
            matrix_t<T> res(a.rows, a.cols);

            for (int r = 0; r < a.rows; ++r)
            {
                for (int c = 0; c < a.cols; ++c)
                {
                    res(r, c) = a(r, c) * scalar;
                }
            }
            return res;
        }
        friend auto operator*(auto scalar, matrix_t<T> a)
        {
            matrix_t<T> res(a.rows, a.cols);

            for (int r = 0; r < a.rows; ++r)
            {
                for (int c = 0; c < a.cols; ++c)
                {
                    res(r, c) = a(r, c) * scalar;
                }
            }
            return res;
        }
        auto slice(int rs, int re, int cs, int ce)
        {
            rs = rs >= 0 ? rs : this->rows + rs;
            re = re >= 0 ? re : this->rows + re;
            cs = cs >= 0 ? cs : this->cols + cs;
            ce = ce >= 0 ? ce : this->cols + ce;

            type::matrix_t<T> slice_container(re - rs, ce - cs);

            for (int r = 0; r < slice_container.rows; ++r)
            {
                for (int c = 0; c < slice_container.cols; ++c)
                {
                    slice_container[r][c] = container[rs + r][cs + c];
                }
            }
            return slice_container;
        }
        auto t()
        {
            assert(this->rows != 0);

            matrix_t<T> trans_container(this->cols, this->rows);

            for (int row = 0; row < this->rows; ++row)
            {
                for (int col = 0; col < this->cols; ++col)
                {
                    trans_container(col, row) = container[row][col];
                }
            }
            return trans_container;
        }
        auto inv()
        {
            auto row_count = this->rows;
            assert(row_count == this->cols);

            auto col_count = row_count * 2;

            matrix_t<T> tmp(row_count, col_count);

            for (std::size_t i = 0; i < row_count; ++i)
            {
                for (std::size_t j = 0; j < row_count; ++j)
                {
                    tmp[i][j] = container[i][j];
                }
            }

            for (std::size_t i = 0; i < row_count; ++i)
            {
                tmp[i][row_count + i] = 1;
            }
            upper_triangular_matrix<matrix_t<T>>(tmp);
            lower_triangular_matrix<matrix_t<T>>(tmp);

            matrix_t<T> inv(row_count, row_count);

            for (std::size_t i = 0; i < row_count; ++i)
            {
                for (std::size_t j = 0; j < row_count; ++j)
                {
                    inv[i][j] = tmp[i][row_count + j];
                }
            }

            return inv;
        }
        auto shape()
        {
            std::cout << "( " << this->rows << ", " << this->cols << " )" << std::endl; 
            return std::tuple{this->rows, this->cols};
        }

        int rows, cols;
    };
}
