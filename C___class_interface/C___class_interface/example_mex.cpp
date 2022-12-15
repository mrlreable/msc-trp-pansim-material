#include "mex.h"
#include "class_handle.hpp"
#include <vector>
#include <cstdlib>
#include <cstring>
#include <random>

// The class that we are interfacing to
class Example
{
private:
    // private: vector like data structure (matlab doesn't know it)
    // train -> args: give array
    // test -> return array
    // communicate through the private vector
    std::vector<double> m_values;
    std::vector<std::string> m_strValues;

public:
    Example()
    {
        mexPrintf("Calling constructor\n");
        m_values = std::vector<double>();
        //m_strValues = std::vector<std::string>();
    }

    ~Example()
    {
        mexPrintf("Calling destructor\n");
    }

    // void train(double *v, size_t n)
    // {
    //     m_values = std::vector<double>{v, v + n};
    // }

    // std::vector<double> test()
    // {
    //     return m_values;
    // }
    void train(std::string *v, size_t n)
    {
        m_strValues = std::vector<std::string>(v, v + n);
    }

    std::vector<double> test()
    {
        for (int i = 0; i < m_strValues.size(); i++){
            m_values.push_back((double)i);
        }
        return m_values;
    }
};

// plhs -> output list as mxArray
// prhs -> input list as mxArray
//      -> when calling mexFunction:
//          - prhs[0]: mode (train, test)
//          - prhs[1]: Example class handler pointer
//          - prhs[2]: The input data that the Example class works on
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Check for valid number of outputs
    if (nlhs > nrhs)
        mexErrMsgIdAndTxt("MATLAB:mexfunction:inputOutputMismatch",
                          "Cannot specify more outputs than inputs.\n");

    // Get the command string
    char cmd[64];
    if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
        mexErrMsgTxt("First input should be a command string less than 64 characters long.");

    // New
    if (!strcmp("new", cmd))
    {
        // Check parameters
        if (nlhs != 1)
            mexErrMsgTxt("New: One output expected.");
        // Return a handle to a new C++ instance
        plhs[0] = convertPtr2Mat<Example>(new Example);
        return;
    }

    // Check there is a second input, which should be the class instance handle
    if (nrhs < 2)
        mexErrMsgTxt("Second input should be a class instance handle.");

    // Delete
    if (!strcmp("delete", cmd))
    {
        // Destroy the C++ object
        destroyObject<Example>(prhs[1]);
        // Warn if other commands were ignored
        if (nlhs != 0 || nrhs != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }

    // Get the class instance pointer from the second input
    Example *example_instance = convertMat2Ptr<Example>(prhs[1]);

    // Call the various class methods
    // Train
    if (!strcmp("train", cmd))
    {
        //Check parameters
        // if (nlhs < 0 || nrhs < 2)
        //     mexErrMsgTxt("Train: Unexpected arguments.");

        // // Get the size of the input array
        // mwSize n = mxGetN(prhs[2]);
        // double *input = mxGetPr(prhs[2]);
        // example_instance->train(input, n);
        // return;
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("Train: Unexpected arguments.");

        for (int i = 0; i < nrhs; i++) {
            mexPrintf("\n\tInput Arg %i is of type:\t%s\n", i, mxGetClassName(prhs[i]));
    }

        mwSize strLen = mxGetElementSize(prhs[2]);
        mexPrintf("input size: %d\n", strLen);
        //void* input = mxGetData(prhs[2]);
        const mxArray *in = prhs[2];
        mxArray *string_class[1], *char_array[1];
        string_class[0] = in;
        mexCallMATLAB(1, char_array, 1, string_class, "char");
        
        //Parse the char array to create an std::string
        int buflen = mxGetN(char_array[0])*sizeof(mxChar)+1;
        char* buf = new char[buflen];
        mxGetString(char_array[0],buf,buflen);
        std::string data = std::string(buf);
        delete buf;
        

        //example_instance->train(input, strLen);
        return;
    }
    // Test
    if (!strcmp("test", cmd))
    {
        // Check parameters
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("Test: Unexpected arguments.");
        // Call the method
        std::vector<double> res = example_instance->test();
        mexPrintf("Size of m_values: %d", res.size());

        // Convert double -> mxArray
        plhs[0] = mxCreateNumericMatrix(1, res.size(), mxDOUBLE_CLASS, mxREAL);
        //plhs[0] = mxCreateCharMatrixFromStrings(res.size(), res[0]);
        memcpy(mxGetData(plhs[0]), &res[0], res.size() * sizeof(char));
        return;
    }

    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
