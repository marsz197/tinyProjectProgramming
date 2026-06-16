#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
//Part A **********************
class Vector{
private:
    int mSize;
    double* mData;
public:
    Vector(int size): mSize(size), mData(new double[size]){
        // Initialize all elements to zero
        for (int i = 0; i < mSize; ++i){
            mData[i] = 0.0;
        }
    }
    ~Vector(){
        delete[] mData;
    }

    Vector operator+(const Vector& other) const{
        assert(mSize == other.mSize);
        Vector result(mSize);
        for (int i = 0; i < mSize; ++i){
            result.mData[i] = mData[i] + other.mData[i];
        }
        return result;
    }
    Vector operator-(const Vector& other) const{
        assert(mSize == other.mSize);
        Vector result(mSize);
        for (int i = 0; i < mSize; ++i){
            result.mData[i] = mData[i] - other.mData[i];
        }
        return result;
    }
    Vector operator-()const{
        Vector result(mSize);
        for (int i =0; i< mSize; ++i){
            result.mData[i] = -mData[i];
        }
        return result;
    }
    Vector operator*(double scalar)const {
        Vector result(mSize);
        for (int i = 0; i < mSize; ++i){
            result.mData[i] = mData[i] * scalar;
        }
        return result;
    }
    Vector operator*(const Vector& other) const{
        assert(mSize == other.mSize);
        Vector result(mSize);
        for (int i = 0; i < mSize; ++i){
            result.mData[i] = mData[i] * other.mData[i];
        }
        return result;
    }
    double& operator[](int index){
        assert(index >= 0 && index < mSize);
        return mData[index];
    }
    const double& operator[](int index) const{
        assert(index >= 0 && index < mSize);
        return mData[index];
    }
    double& operator()(int index){
        assert(index >= 1 && index <= mSize);
        return mData[index - 1]; // Adjust for 1-based indexing
    }
    const double& operator()(int index) const{
        assert(index >= 1 && index <= mSize);
        return mData[index - 1]; // Adjust for 1-based indexing
    }
    Vector& operator=(const Vector& other){
        if (this == &other){
            return *this; // Handle self-assignment
        }
        delete [] mData; // Free existing data
        mSize = other.mSize;
        mData = new double[mSize];
        for (int i = 0; i < mSize; ++i){
            mData[i] = other.mData[i];
        }
        return *this;
    }
    int getSize() const{
        return mSize;
    }
};

class Matrix{
private:
    int mNumRows;
    int mNumCols;
    double** mData;
public:
    Matrix(int numRows, int numCols): mNumRows(numRows), mNumCols(numCols){
        mData = new double*[mNumRows];// Allocate memory for rows
        for (int i=0; i< mNumRows; ++i){
            mData[i] = new double[mNumCols]; // Allocate memory for columns
            for (int j=0; j< mNumCols; ++j){
                mData[i][j] = 0.0; // Initialize all elements to zero
            }
        }
        
    }
    Matrix (const Matrix& other){
        mNumRows = other.mNumRows;
        mNumCols = other.mNumCols;
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i){
            mData[i] = new double[mNumCols]; // Allocate memory for columns
            for (int j = 0; j < mNumCols; ++j){
                mData[i][j] = other.mData[i][j];
            }
        }
    }
    ~Matrix(){
        for (int i = 0; i < mNumRows; ++i){
            delete[] mData[i]; // Free memory for each row
        }
        delete[] mData; // Free memory for row pointers
    }
    int numRows() const{
        return mNumRows;
    }
    int numCols() const{
        return mNumCols;
    }
    double& operator()(int i, int j){
        if (i < 1 || i > mNumRows || j < 1 || j > mNumCols){
            throw out_of_range("Index out of range");
        }
        return mData[i-1][j-1]; // Adjust for 1-based indexing
    }
    const double& operator()(int i, int j) const{
        if (i < 1 || i > mNumRows || j < 1 || j > mNumCols){
            throw out_of_range("Index out of range");
        }
        return mData[i-1][j-1]; // Adjust for 1-based indexing
    }
    Matrix operator+(const Matrix& other) const{
        assert(mNumRows == other.mNumRows && mNumCols == other.mNumCols);
        Matrix result(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; ++i){
            for (int j = 0; j < mNumCols; ++j){
                result.mData[i][j] = mData[i][j] + other.mData[i][j];
            }
        }
        return result;
    }
    Matrix operator-(const Matrix& other) const{
        assert(mNumRows == other.mNumRows && mNumCols == other.mNumCols);
        Matrix result(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; ++i){
            for (int j = 0; j < mNumCols; ++j){
                result.mData[i][j] = mData[i][j] - other.mData[i][j];
            }
        }
        return result;
    }
    Matrix operator-() const{
        Matrix result(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; ++i){
            for (int j = 0; j < mNumCols; ++j){
                result.mData[i][j] = -mData[i][j];
            }
        }
        return result;
    }
    Matrix operator*(double scalar) const{
        Matrix result(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; ++i){
            for (int j = 0; j < mNumCols; ++j){
                result.mData[i][j] = mData[i][j] * scalar;
            }
        }
        return result;
    }
    Matrix operator= (const Matrix& other){
        if (this == &other){
            return *this; // Handle self-assignment
        }
        for (int i = 0; i < mNumRows; ++i){
            delete[] mData[i]; // Free existing data
        }
        delete[] mData; // Free memory for row pointers
        mNumRows = other.mNumRows;
        mNumCols = other.mNumCols;
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i){
            mData[i] = new double[mNumCols]; // Allocate memory for columns
            for (int j = 0; j < mNumCols; ++j){
                mData[i][j] = other.mData[i][j];
            }
        }
        return *this;
    }
    Matrix operator*(const Matrix&other) const{
        assert(mNumCols == other.mNumRows);
        Matrix result(mNumRows, other.mNumCols);
        for (int i = 0; i < mNumRows; ++i){
            for (int j = 0; j < other.mNumCols; ++j){
                result.mData[i][j] = 0.0;
                for (int k = 0; k < mNumCols; ++k){
                    result.mData[i][j] += mData[i][k] * other.mData[k][j];
                }
            }
        }
        return result;
    }
    Vector operator*(const Vector& vec) const{
        assert(mNumCols == vec.getSize());
        Vector result(mNumRows);
        for (int i = 0; i < mNumRows; ++i){
            result[i] = 0.0;
            for (int j = 0; j < mNumCols; ++j){
                result[i] += mData[i][j] * vec[j];
            }
        }
        return result;
    }
    //computes determinant of a square matrix using recursion
    double determinant() const{
        assert(mNumRows == mNumCols); // Ensure it's a square matrix
        if (mNumRows == 1){
            return mData[0][0];
        }
        if (mNumRows == 2){
            return mData[0][0] * mData[1][1] - mData[0][1] * mData[1][0];
        }
        double det = 0.0;
        for (int j = 0; j < mNumCols; ++j){
            Matrix subMatrix(mNumRows - 1, mNumCols - 1);
            for (int i = 1; i < mNumRows; ++i){
                for (int k = 0; k < mNumCols; ++k){
                    if (k < j){
                        subMatrix(i-1, k) = mData[i][k];
                    } else if (k > j){
                        subMatrix(i-1, k-1) = mData[i][k];
                    }
                }
            }
            det += (j % 2 == 0 ? 1 : -1) * mData[0][j] * subMatrix.determinant();
        }
        return det;
    }
    //computes the inverse of a square matrix using Gaussian elimination
    Matrix inverse(int maxIterations = 1000, double tolerance = 1e-10) const{
        assert(mNumRows == mNumCols); // Ensure it's a square matrix
        int n = mNumRows;
        Matrix augmented(n, 2*n);
        // Create the augmented matrix [A | I]
        for (int i = 1; i <= n; ++i){
            for (int j = 1; j <= n; ++j){
                augmented(i, j) = mData[i-1][j-1];
            }
            for (int j = n + 1; j <= 2*n; ++j){
                augmented(i, j) = (i == (j - n)) ? 1.0 : 0.0; // Identity matrix
            }
        }
        // Perform Gaussian elimination
        for (int i = 1; i <= n; ++i){
            int maxRow = i;
            for (int k = i + 1; k <= n; ++k){
                if (abs(augmented(k, i)) > abs(augmented(maxRow, i))){
                    maxRow = k;
                }
            }
            if (maxRow != i){
                for (int j = 1; j <= 2*n; ++j){
                    swap(augmented(i, j), augmented(maxRow, j));
                }
            }
            // Find pivot
            double pivot = augmented(i, i);
            if (abs(pivot) < tolerance){
                throw runtime_error("Matrix is singular and cannot be inverted.");
            }
            // Normalize the pivot row
            for (int j = 1; j <= 2*n; ++j){
                augmented(i, j) /= pivot;
            }
            // Eliminate other rows
            for (int k = 1; k <= n; ++k){
                if (k != i){
                    double factor = augmented(k, i);
                    for (int j = 1; j <= 2*n; ++j){
                        augmented(k, j) -= factor * augmented(i, j);
                    }
                }
            }
        }
        // Extract the inverse from the augmented matrix
        Matrix inverse(n, n);
        for (int i = 1; i <= n; ++i){
            for (int j = 1; j <= n; ++j){
                inverse(i, j) = augmented(i, j + n);
            }
        }
        return inverse;
    }
    //computes the pseudo-inverse of a matrix
    Matrix transpose() const{
        Matrix transposed(mNumCols, mNumRows);
        for (int i = 0; i < mNumRows; ++i){
            for (int j = 0; j < mNumCols; ++j){
                transposed(j+1, i+1) = mData[i][j];
            }
        }
        return transposed;
    }
    Matrix pseudoInverse() const{
        Matrix AT = this->transpose();
        Matrix ATA = AT * (*this);
        Matrix ATA_inv = ATA.inverse();
        Matrix result = ATA_inv * AT;
        return result;
    }
    
    Matrix tikhonovInverse(double lambda) const{
        Matrix AT = this->transpose();
        Matrix ATA = AT * (*this);
        Matrix lambdaI(ATA.numRows(), ATA.numCols());
        for(int i = 1; i <= ATA.numRows(); ++i){
            lambdaI(i, i) = lambda;
        }
        Matrix ATA_plus_lambdaI = ATA + lambdaI;
        Matrix ATA_plus_lambdaI_inv = ATA_plus_lambdaI.inverse();
        Matrix result = ATA_plus_lambdaI_inv * AT;
        return result;
    }   
};
class LinearSystem{
protected:
    int mSize;
    Matrix* mpA;
    Vector* mpB;
private:
    LinearSystem(const LinearSystem& other);
public:
    LinearSystem(Matrix& A, Vector& b){
        assert(A.numRows() == b.getSize());
        mSize = b.getSize();
        mpA = &A;
        mpB = &b;
    };
    virtual Vector Solve(){
        // Implement Gaussian elimination to solve Ax = b
        Matrix augmented(mSize, mSize + 1);
        for (int i = 0; i < mSize; ++i){
            for (int j = 0; j < mSize; ++j){
                augmented(i, j) = (*mpA)(i+1, j+1); // Adjust for 1-based indexing
            }
            augmented(i, mSize) = (*mpB)[i];
        }
        // Forward elimination
        for (int i = 0; i < mSize; ++i){
            // Find pivot
            double pivot = augmented(i, i);
            if (abs(pivot) < 1e-10){
                throw runtime_error("Matrix is singular or nearly singular.");
            }
            // Normalize the pivot row
            for (int j = i; j <= mSize; ++j){
                augmented(i, j) /= pivot;
            }
            // Eliminate below
            for (int k = i + 1; k < mSize; ++k){
                double factor = augmented(k, i);
                for (int j = i; j <= mSize; ++j){
                    augmented(k, j) -= factor * augmented(i, j);
                }
            }
        }
        // Back substitution
        Vector solution(mSize);
        for (int i = mSize - 1; i >= 0; --i){
            solution[i] = augmented(i, mSize);
            for (int j = i + 1; j < mSize; ++j){
                solution[i] -= augmented(i, j) * solution[j];
            }
        }
        return solution;
    }
    virtual ~LinearSystem() {}
};
class PosSymLinSystem : public LinearSystem{
public:
    PosSymLinSystem(Matrix& A, Vector& b): LinearSystem(A, b){
        assert(A.numRows() == A.numCols()); // Ensure A is square
        assert(A.numRows() == b.getSize()); // Ensure dimensions match
        for (int i =1; i <= A.numRows();++i){
            for (int j=i+1; j <= A.numCols();++j){
                assert(abs(A(i,j)-A(j,i)) < 1e-10 && "Matrix is not symmetric."); // Check for symmetry
            }
        }
    }
    //override this method in the class PosSymLinSystem so that it uses the conjugate gradient method for solving linear systems. 
    Vector Solve() override{
        Vector x(mSize); // Initial guess (zero vector)
        Vector r = *mpB; // Initial residual
        Vector p = r; // Initial search direction
        double rsOld = 0.0;
        for (int i = 0; i < mSize; ++i){
            rsOld += r[i] * r[i];
        }
        for (int i = 0; i < mSize; ++i){
            if (sqrt(rsOld) < 1e-10){
                break; // Convergence check
            }
            Vector Ap = (*mpA) * p; // Matrix-vector product
            double alpha = rsOld;
            double ApDotP = 0.0;
            for (int j = 0; j < mSize; ++j){
                ApDotP += Ap[j] * p[j];
            }
            alpha /= ApDotP;
            for (int j = 0; j < mSize; ++j){
                x[j] += alpha * p[j]; // Update solution
                r[j] -= alpha * Ap[j]; // Update residual
            }
            double rsNew = 0.0;
            for (int j = 0; j < mSize; ++j){
                rsNew += r[j] * r[j];
            }
            if (sqrt(rsNew) < 1e-10){
                break; // Convergence check
            }
            for (int j = 0; j < mSize; ++j){
                p[j] = r[j] + (rsNew / rsOld) * p[j]; // Update search direction
            }
            rsOld = rsNew;
        }
        return x;
    }
};