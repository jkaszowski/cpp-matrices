#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "frac.h"

class Matrix{
    friend std::ostream& operator<<(std::ostream& os, const Matrix& entity);
    friend std::istream& operator>>(std::istream& is, Matrix& entity);

    private:
        struct dimensions{ int x; int y; } size {0,0};
        std::vector< std::vector<Frac> > grid; // 2D dynamic array

    public:
        Matrix() = default;
        Matrix(int,int); // creates an empty array of specified size
        Matrix(Matrix&,int); // allows you to delete a row from the matrix
        Matrix(Matrix&,int, std::vector<Frac>&); //replaces selected column with vector of constants
        int getColumnCount(); // access to private member size.x
        int getRowCount(); // access to private member size.y
        int addColumn(int howMany = 1); // add columns to an existing array
        int addRow(int howMany = 1); // add columns to an existing array
        void updateDimnensions();
        bool isSquare(); // check if square (if square then you can calculate the determinant)
        void clearMatrix(); // clear the matrix, prepare the object to be re-used with new data
        Frac determinant(); // Get determinant of a square array
        std::vector<Frac> solve(); // get vector with solutions
        std::vector<Frac>& operator[](int position); // provide easy access to every cell in form Matrix[column][row]
};

#endif