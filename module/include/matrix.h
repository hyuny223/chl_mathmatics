#pragma once
#include <iostream>
#include <ostream>
#include <vector>

#include "compute.h"


namespace type
{
    template <typename T>
    class matrix_t
    {
        private:
            std::vector<std::vector<T>> container;

        public:
            matrix_t(const int& row, const int& col)
            : rows(row), cols(col)
            {
                container.resize(row, std::vector<T>(col, 0));
            }

            std::vector<std::vector<T>> operator= (std::initializer_list<T> l)
            {
                int r = 0, c = 0;

                for(auto itr = l.begin(); itr != l.end(); ++itr)
                {
                    container[r][c] = *itr;
                    if(r < rows)
                    {
                        if(c < cols - 1)
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

            T& operator() (const int& row, const int& col)
            {
                return container.at(row).at(col);
            }

            friend type::matrix_t<T> operator* (type::matrix_t<T>& a, type::matrix_t<T>& b)
            {
                type::matrix_t<T> res(a.rows, b.cols);
                for(int lrow = 0; lrow < a.rows; ++lrow)
                {
                    for(int rcol = 0; rcol < b.cols; ++rcol)
                    {
                        T elem = 0;
                        for(int rrow = 0; rrow < b.rows; ++rrow)
                        {
                            elem += a(lrow, rrow) * b(rrow, rcol);
                        }
                        res(lrow, rcol) = elem;
                    }
                }
                return res;
            }

            std::vector<T>& operator[] (const int& index)
            {
                return container.at(index);
            }

            friend std::ostream& operator<< (std::ostream &out, type::matrix_t<T>& con)
            {
                out << "[";
                for(int row = 0; row < con.rows; ++row)
                {
                    if(row == 0)
                    {
                        out << "[";
                    }
                    else
                    {
                        out << " [";
                    }
                    for(int col = 0; col < con.cols; ++col)
                    {
                        out << con(row, col);

                        if(col+1 != con.cols)
                        {
                            out << ", ";
                        }
                    }
                    out << "]";
                    if(row+1 != con.rows)
                    {
                        out << ",\n";
                    }
                }
                out << "]";

                return out;
            }

            std::ostream& endl(std::ostream& end) // 개행
            {
                end << '\n';
                return end;
            }

            int rows, cols;

    };
}
