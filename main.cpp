#include <iostream>
#include <fstream>
#include "matrix.h"

using namespace std;

// menu options
void  inputMatrix(Matrix& entity);
bool  importMatrix(Matrix& entity);
void  solveEquationSystem();

// functions to interact with user
void  wait();
void  getOptionFromUser(char& option);

// create global matrix to work with
Matrix matrix;
  
int main(int argc, char *argv[])
{
  char selectedOption = 0;
  do{
    // display menu
    cout << "Welcome to matrix calc, select an option:" << endl;
    cout << "1 - input matrix" << endl;
    cout << "2 - import matrix from file" << endl;
    cout << "3 - display matrix" << endl;
    cout << "4 - calculate determinant" << endl;
    cout << "5 - solve a system of linear equations" << endl;
    cout << "6 - exit" << endl;
    cout << "Selected option: ";

    // wait for the correct answer (there is loop inside that gets input unless valid)
    getOptionFromUser(selectedOption);

  // execute the task
    switch (selectedOption)
    {
    case '1':
      // input matrix by hand
      if(matrix.getColumnCount() > 0) matrix.clearMatrix();
      inputMatrix(matrix);
      break;
    
    case '2':
      // import matrix from file
      if(matrix.getColumnCount() > 0) matrix.clearMatrix();
      importMatrix(matrix);
      break;

    case '3':
      // print already imported matrix
      cout << matrix; wait(); break;

    case '4':
      // checki if matrix exists
      if(matrix.getColumnCount() > 0){
        Frac det;
        // calculate determinant
        // if matrix is not square, it should detect it and show error message
        try{ det = matrix.determinant();}catch(const std::invalid_argument &e){ cout << e.what() << endl; wait(); break;}
        cout<< "Determinant of matrix:" << endl << matrix << "is equal to: " << det << endl;
      }else{
        // if matrix doesn't exist show an error
        cout << "Specify the matrix first!";
      }
      wait(); break;

    case '5':
      std::vector<Frac> solutions;
      try{
        // try to get the solutions for the matrix
        solutions = matrix.solve();
      }catch(const std::invalid_argument &e){
        // handle the errors
        cout << e.what() << endl; wait(); break;
      }
      // show the result
      cout << "Your matrix:" << endl << matrix << endl;
      for(size_t i = 0; i < solutions.size();i++)
        cout << "Solution for x" << i << ": " << solutions[i] << endl;
      wait(); break;
    }
  }while(selectedOption != '6');
  return 0;
}

bool  importMatrix(Matrix& entity){
  // get the filename to open
  string fileName = "matrix.csv";
  string defaultName = "matrix.csv";
  cout << "File name with the matrix: ";
  std::cin >> fileName;

  // open the file
  ifstream inFile(fileName,std::ifstream::in);

  // if opened, import the matrix and return if the result
  if(inFile.is_open()){
    inFile >> entity;
    return true;
  }else{
    return false;
  }
}

// Function that allow inputting matrix by hand
void  inputMatrix(Matrix& entity){
  // get the dimensions of the matrix
  int x = 1, y = 1;
  cout << "Number of columns: "; cin >> x;
  cout << "Number of rows: "; cin >> y;

  // adjust the size of the matrix (new matrix has 0 columns and 0 rows)
  entity.addColumn(x);
  entity.addRow(y);

  // get value for every cell by asking matrix[column][row] = ?
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      cout << "Matrix["<<i<<"][" << j <<"] = ";
      cin >> entity[i][j];
    }
  }
}

// wait for the user confirmation
void  wait(){
  cout << "Press Enter to continue" << endl;
  cin.ignore();
  cin.get();
}

// get the option from the user unless it is within specified range
void  getOptionFromUser(char& option)
{
  do{
    cin >> option;
  }while(option < '1'|| option > '6');
};