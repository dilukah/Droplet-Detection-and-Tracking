#include <string.h>
#include <iostream>
#include <stdio.h>
#include <mat.h>
#include <vector>
#include "MatlabExport.h"

using namespace std;

bool SaveMatlabMat(vector<vector<double> >& src1, vector<vector<double> >& src2, string file_name)
{

    MATFile* pmatFile = NULL;
    pmatFile = matOpen(file_name.c_str(), "w");  // create a mat file and open with write command
    if (pmatFile == NULL)
    {
        cout << "Error creating file " << file_name << endl;
        return(EXIT_FAILURE);
    }
    cout << "Successfully create file " << file_name << "\n\n";


    // get data dimension
    int src1_row = src1.size();
    int src1_col = src1[0].size();
    int src2_row = src2.size();
    int src2_col = src2[0].size();

    //std::cout << "print dimension" << endl;
    //std::cout << src1_row << " " << src1_col << " " << std::endl;
    //std::cout << src2_row << " " << src2_col << " " << std::endl;
    
    if ((src1_row != src2_row) || (src1_col != src2_col))
    {
        cout << "The dimension of two input data should be the same." << endl;
        return(EXIT_FAILURE);
    }
    
    //transpose the data
    //vector<vector<int> > p(m, vector<int>(n));
    double* data1, * data2;

    data1 = new double[src1_col * src1_row];
    for (int i = 0; i < src1_col; i++)
        for (int j = 0; j < src1_row; j++)
            data1[i * src1_row + j] = src1[j][i];

    data2 = new double[src2_col * src2_row];
    for (int i = 0; i < src2_col; i++)
        for (int j = 0; j < src2_row; j++)
            data2[i * src2_row + j] = src2[j][i];

    //create two matlab matrix
    mxArray* x_position = NULL, * y_position = NULL;

    x_position = mxCreateDoubleMatrix(src1_row, src1_col, mxREAL);
    if (x_position == NULL) {
        printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
        printf("Unable to create mxArray.\n");
        return(EXIT_FAILURE);
    }
    y_position = mxCreateDoubleMatrix(src2_row, src2_col, mxREAL);
    if (y_position == NULL) {
        printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
        printf("Unable to create mxArray.\n");
        return(EXIT_FAILURE);
    }

    //assign data
    int status;

    memcpy((void*)(mxGetPr(x_position)), (void*)data1, sizeof(double) * src1_row * src1_col);
    status = matPutVariable(pmatFile, "Xposition", x_position);
    if (status != 0) {
        printf("%s :  Error using matPutVariable on line %d\n", __FILE__, __LINE__);
        return(EXIT_FAILURE);
    }
    
    memcpy((void*)(mxGetPr(y_position)), (void*)data2, sizeof(double) * src2_row * src2_col);
    status = matPutVariable(pmatFile, "Yposition", y_position);
    if (status != 0) {
        printf("%s :  Error using matPutVariable on line %d\n", __FILE__, __LINE__);
        return(EXIT_FAILURE);
    }

    //close file and release source
    mxDestroyArray(x_position);
    mxDestroyArray(y_position);

    //matClose(pmatFile);
    if (matClose(pmatFile) != 0)
    {
        cout << "closing file error.\n";
        return(EXIT_FAILURE);
    }
    

    delete[] data1;
    delete[] data2;

    std::cout << "Data stored successfully." << std::endl;

    return true;
}

bool SaveMatlabMat(vector<vector<double> >& src1, string file_name)
{
    MATFile* pmatFile = NULL;
    pmatFile = matOpen(file_name.c_str(), "w");  // create a mat file and open with write command
    if (pmatFile == NULL)
    {
        cout << "Error creating file " << file_name << endl;
        return(EXIT_FAILURE);
    }
    cout << "Successfully create file " << file_name << "\n\n";


    // get data dimension
    int src1_row = src1.size();
    int src1_col = src1[0].size();


  

    //transpose the data
    //vector<vector<int> > p(m, vector<int>(n));
    double* data1;

    data1 = new double[src1_col * src1_row];
    for (int i = 0; i < src1_col; i++)
        for (int j = 0; j < src1_row; j++)
            data1[i * src1_row + j] = src1[j][i];

    //create two matlab matrix
    mxArray* Ellipse_para = NULL;

    Ellipse_para = mxCreateDoubleMatrix(src1_row, src1_col, mxREAL);
    if (Ellipse_para == NULL) {
        printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
        printf("Unable to create mxArray.\n");
        return(EXIT_FAILURE);
    }
   

    //assign data
    int status;

    memcpy((void*)(mxGetPr(Ellipse_para)), (void*)data1, sizeof(double) * src1_row * src1_col);
    status = matPutVariable(pmatFile, "Ellipse_para", Ellipse_para);
    if (status != 0) {
        printf("%s :  Error using matPutVariable on line %d\n", __FILE__, __LINE__);
        return(EXIT_FAILURE);
    }



    //close file and release source
    mxDestroyArray(Ellipse_para);

    //matClose(pmatFile);
    if (matClose(pmatFile) != 0)
    {
        cout << "closing file error.\n";
        return(EXIT_FAILURE);
    }


    delete[] data1;

    std::cout << "Data stored successfully." << std::endl;

    return true;
}