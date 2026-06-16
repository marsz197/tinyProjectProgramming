#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "project1.cpp"

using namespace std;

struct Instance {
    double features[6];
    double prp;
};

int main() {
    string filePath = "D:\\2nd Year\\2nd Sem\\Programming 2\\machine.data";
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return 1;
    }

    vector<Instance> data;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() >= 9) {
            Instance inst;
            inst.features[0] = stod(tokens[2]); // MYCT
            inst.features[1] = stod(tokens[3]); // MMIN
            inst.features[2] = stod(tokens[4]); // MMAX
            inst.features[3] = stod(tokens[5]); // CACH
            inst.features[4] = stod(tokens[6]); // CHMIN
            inst.features[5] = stod(tokens[7]); // CHMAX
            inst.prp = stod(tokens[8]);         // PRP
            data.push_back(inst);
        }
    }
    file.close();

    int totalSize = data.size();
    if(totalSize == 0){
        cerr << "No data loaded!" << endl;
        return 1;
    }

    int trainSize = totalSize * 0.8;
    int testSize = totalSize - trainSize;

    Matrix A_train(trainSize, 6);
    Vector b_train(trainSize);

    Matrix A_test(testSize, 6);
    Vector b_test(testSize);

    for (int i = 0; i < trainSize; ++i) {
        for (int j = 0; j < 6; ++j) {
            A_train(i + 1, j + 1) = data[i].features[j];
        }
        b_train[i] = data[i].prp;
    }

    for (int i = 0; i < testSize; ++i) {
        int dataIdx = trainSize + i;
        for (int j = 0; j < 6; ++j) {
            A_test(i + 1, j + 1) = data[dataIdx].features[j];
        }
        b_test[i] = data[dataIdx].prp;
    }

    cout << "=== Project 1b Results ===" << endl;
    cout << "Total Instances: " << totalSize << endl;
    cout << "Train Size: " << trainSize << ", Test Size: " << testSize << endl;

    try {
        Matrix pseudoInv = A_train.pseudoInverse();
        Vector x_mp = pseudoInv * b_train;
        Vector predictions_mp = A_test * x_mp;
        double rmse_mp = 0.0;
        for (int i = 0; i < testSize; ++i) {
            double err_mp = predictions_mp[i] - b_test[i];
            rmse_mp += err_mp * err_mp;
        }
        rmse_mp = sqrt(rmse_mp / testSize);
        cout << "RMSE (Moore-Penrose): " << rmse_mp << endl;
    } catch(const exception& e){
        cout << "Moore-Penrose failed: " << e.what() << endl;
    }

    try {
        double lambda = 1.0; // Regularization parameter
        Matrix tikhonovInv = A_train.tikhonovInverse(lambda);
        Vector x_tk = tikhonovInv * b_train;
        Vector predictions_tk = A_test * x_tk;
        double rmse_tk = 0.0;
        for (int i = 0; i < testSize; ++i) {
            double err_tk = predictions_tk[i] - b_test[i];
            rmse_tk += err_tk * err_tk;
        }
        rmse_tk = sqrt(rmse_tk / testSize);
        cout << "RMSE (Tikhonov, lambda=" << lambda << "): " << rmse_tk << endl;
    } catch(const exception& e){
        cout << "Tikhonov failed: " << e.what() << endl;
    }

    return 0;
}
