#include "matrix.h"

std::ostream& operator<<(std::ostream& os, const Matrix& entity){
    for(int i = 0; i < entity.size.y; i++){
        for(int j = 0; j < entity.size.x; j++){
            os << entity.grid.at(j).at(i) << '\t';
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& entity){
    int y = 0;
    while( !is.eof() ){
        int x = 0;
        std::vector<Frac> row;
        std::string importedLine;
        is >> std::ws;

        // check if line is a comment (comments start with a semicolon). If so then skip the line
        while(is.peek() == ';'){ std::getline(is,importedLine); is>>std::ws; }
        
        // maybe the comment was in the last line of the file, we need to check for such a case
        if(!is.eof()){
            std::getline(is,importedLine);
            std::stringstream ss_line(importedLine);
            Frac tmp_frac;
            while(ss_line){
                ss_line >> tmp_frac;

                // adjust the size of the array if it is too small
                if(entity.size.y < y+1)
                    entity.addRow();
                if(entity.size.x < x+1)
                    entity.addColumn();
                entity.grid[x][y] = tmp_frac;
                x++;
                char c = is.peek();
            };
            y++;
        }
    };
    return is;
}

// clear the matrix
void Matrix::clearMatrix(){
    for(auto & column : grid){
        column.clear();
    }
    grid.clear();
    updateDimnensions();
}

// get the vector with the solutions
std::vector<Frac> Matrix::solve(){
    std::vector<Frac> solutions;
    if(this->size.x != (this->size.y+1)){
        throw std::invalid_argument("It is not an augmented matrix!");
    }else{
        Frac common_determinant = Matrix(*this,this->size.y).determinant(); // delete last column and calculate the determinant
        for(int i = 0; i < this->size.y; i++){
            Frac detXi = Matrix(*this,i,(*this)[this->size.y]).determinant(); // calculate Xi determinant
            solutions.push_back(detXi/common_determinant); // calculate and store the solution
        }
    }
    return solutions;
}

// update dimensions to match the internal grid size
void Matrix::updateDimnensions(){
    size.x = grid.size();
    size.y = grid[0].size();
}

// check if matrix is square
bool Matrix::isSquare(){
    return (grid.size() == grid[0].size());
}

// add a column to an already existing matrix
int Matrix::addColumn(int howMany){
    for(int i = 0; i < howMany; i++){
        grid.push_back(std::vector<Frac>(size.y,0));
        size.x++;
    }
    return howMany;
}

// add row to an already existing matrix
int Matrix::addRow(int howMany){
    for(int j = 0; j < howMany; j++){
        for(int i = 0; i < size.x; i++){
            grid[i].push_back(0);
        }
        size.y++;
    }
    return howMany;
}

// calculate the determinant of an already existing matrix
Frac Matrix::determinant(){
    if(this->isSquare()){

        // if matrix size is two, use shortcut (muliplying and subtracting diagonals)
        if(size.x == 2){
            Frac pos, neg, res;
            pos  = grid[0][0]*grid[1][1];
            neg = grid[0][1]*grid[1][0];
            res = pos - neg;
            return res;
        }
        
        // if matrix size is three, also use ready made formula 
        else if(size.x == 3)
        {
            Frac posMult(0);
            Frac negMult(0);
            for(int i = 0; i < size.x; i++){
            Frac diagonalMultiplier(1);
                for (int j = 0; j < size.y; j++)
                {
                    if(j+i < size.x){
                        // std::cout << diagonalMultiplier << " * " << grid[j][j+i] << std::endl;
                        diagonalMultiplier *= grid[j][j+i];
                    }else{
                        // std::cout << diagonalMultiplier << " * " << grid[j][j+i-size.x] << std::endl;
                        diagonalMultiplier *= grid[j][j+i-size.x];
                    }
                }
                posMult += diagonalMultiplier;
            }
            for(int i = size.x - 1; i >= 0; i--){
            Frac diagonalMultiplier(1);
                for (int j = 0; j < size.y; j++)
                {
                    if(i-j >= 0){
                        // std::cout << diagonalMultiplier << " * " << grid[j][i-j] << std::endl;
                        diagonalMultiplier *= grid[j][i-j];
                    }else{
                        // std::cout << diagonalMultiplier << " * " << grid[j][i-j+size.x] << std::endl;
                        diagonalMultiplier *= grid[j][i-j+size.x];
                    }
                }
                negMult += diagonalMultiplier;
            }
            return posMult - negMult;
        }else{
            // if size is big than transform matrix to the upper triangular form and take matrix as multiplication of the diagonal

            // create array of fracs pointers to columns and allocate appropriate memory
            Frac **array;
            array = (Frac**) calloc(this->size.x,sizeof(Frac*));
            for(int i = 0; i < this->size.x; i++){
                array[i] = (Frac*) calloc(this->size.y, sizeof(Frac));
                if(array[i] == nullptr){
                    std::cout << "Error" << std::endl;
                }
            }

            // copy from vector to temporary array
            for(int i = 0; i < this->size.x; i++){
                for(int j = 0; j < this->size.y; j++){
                    array[i][j] = this->grid[i][j];
                }
            }

            // Turn temporary matrix to upper triangulate form
            //for every row
            for(int i = 0; i<this->size.y; i++){
                // go to every lower row
                for(int j = i+1; j < this->size.y; j++){
                    //calculate ratio upper row/this row
                    Frac ratio = array[i][j]/array[i][i];
                    // for every column multiply that ratio by the most upper and subtract
                    for(int z = i; z < this->size.x; z++)
                        array[z][j] -= (ratio * array[z][i]);
                }
            }
            
            //Get determinant by multiplying terms on the diagonal
            for(int i = 1; i < this->size.x; i++)
                array[0][0] *= array[i][i];
            Frac det = array[0][0];


            //deallocate memory
            for(int i = 0; i < this->size.x; i++)
                free(array[i]);
            free(array);

            return det;
        }
    }
    // throw an exception if matrix is not square
    throw std::invalid_argument( "Matrix needs to be square!" );
}

// allow to use Matrix[x][y] access to all cells
std::vector<Frac>& Matrix::operator[](int position){
    return grid.at(position);
}

// access the size of the matrix
int Matrix::getColumnCount(){
    return this->size.x;
}

// access the size of the matrix
int Matrix::getRowCount(){
    return this->size.x;
}

// constructor making an empty matrix of specified size
Matrix::Matrix(int x, int y){
    grid = std::vector<   std::vector<Frac>   >(x,std::vector<Frac>(y,0));
    size.x = x;
    size.y = y;
}

// specific constructor
// it deletes one column and shift the contents
// the main purpose of this function is to help with calculating determinants
Matrix::Matrix(Matrix &entity, int coulmnToDelete){
    int newColumn = 0;
    for(int i = 0; i < entity.size.x; i++){
        if(i != coulmnToDelete){
            this->addColumn();
            for(int j = 0; j < entity.size.y; j++){
                if(this->size.y < entity.size.y) this->addRow();
                // result[newColumn][j] = entity.grid.at(i).at(j);
                this->grid.at(newColumn).at(j) = entity[i][j];
            }
            newColumn++;
        }
    }
}

// specific constructor
// similar to the one deleting a column, but this one swaps column for the values in free vector
// very very useful when you want to calculate determinants
Matrix::Matrix(Matrix &entity, int coulmnToDelete, std::vector<Frac> &freeVector){
    for(int i = 0; i < (entity.size.x - 1); i++){
        this->addColumn();
        for(int j = 0; j < entity.size.y; j++){
            if(this->size.y < entity.size.y) this->addRow();
            if(i != coulmnToDelete)
                this->grid.at(i).at(j) = entity[i][j];
            else
                this->grid.at(i).at(j) = freeVector[j];
        }
    }
}