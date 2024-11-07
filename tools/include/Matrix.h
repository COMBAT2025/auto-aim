#ifndef MATRIX_H
#define MATRIX_H

#include "Combat2024_AllTeam.h"

template <class T>
class Matrix {


public:
    T** mat;
    int row, col;

//    istream& operator>>(istream&is,Matrix &a)
//    {
//        for(int i=0;i<row;i++)
//            for(int j=0;j<col;j++)
//                cin>>mat[i][j];
//    }
//
//    ostream& operator<<(ostream&is)
//    {
//        for(int i=0;i<row;i++)
//            for(int j=0;j<col;j++)
//                cout>>mat[i][j];
//    }

    Matrix(int r, int c)
    {
        row = r;
        col = c;
        mat = new T * [row];
        for (int i = 0; i < row; i++)
            mat[i] = new T[col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                mat[i][j] = 0;
    }

    Matrix(Matrix& M)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                mat[i][j] = M.mat[i][j];
    }
    ~Matrix()
    {
        for (int i = 0; i < row; i++)
            delete[] mat[i];
        delete[] mat;

    }

    Matrix& operator=(Matrix& M)
    {
        if (this == &M)
        {
            return *this;
        }
        col = M.col;
        row = M.row;
        mat = new T * [row];
        for (int i = 0; i < row; i++)
            mat[i] = new T[col];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                mat[i][j] = M.mat[i][j];
            }
        }
        return *this;
    }

    Matrix operator+(Matrix& M)
    {
        Matrix M1 = *this;
        if (col == M.col && row == M.row)
        {
            for (int i = 0; i < M1.row; i++)
            {
                for (int j = 0; j < M1.col; j++)
                {
                    M1.mat[i][j] = mat[i][j] + M.mat[i][j];
                }
            }
            return M1;
        }
        else
        {
            std::cout << "矩阵行或列数不相等，不能相加" << std::endl;
            return M1;
        }
    }

    Matrix operator-(Matrix& M)
    {
        Matrix M1 = *this;
        if (col == M.col && row == M.row)
        {
            for (int i = 0; i < M1.row; i++)
            {
                for (int j = 0; j < M1.col; j++)
                {
                    M1.mat[i][j] = mat[i][j] - M.mat[i][j];
                }
            }
            return M1;
        }
        else
        {
            std::cout << "矩阵行或列数不相等，不能相减" << std::endl;
            return M1;
        }
    }

    Matrix operator*(Matrix& M)
    {
        Matrix M1(row, M.col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < M.col; j++)
            {
                for (int k = 0; k < col; k++)
                {
                    M1.mat[i][j] += (mat[i][k] * M.mat[k][j]);
                }
            }
        }
        return M1;
    }

    bool operator==(Matrix& M)
    {
        if (col != M.col || row != M.row)
            return false;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (mat[i][j] != M.mat[i][j])
                    return false;
            }
        }
        return true;
    }
};


#endif //MATRIX_H
