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

public:
    Example()
    {
        mexPrintf("Calling constructor\n");
        m_values = std::vector<double>();
    }

    ~Example()
    {
        mexPrintf("Calling destructor\n");
    }

    void train(double *v, size_t n)
    {
        m_values = std::vector<double>{v, v + n};
    }

    std::vector<double> test()
    {
        return m_values;
    }
};

// plhs -> output list as mxArray
// prhs -> input list as mxArray
//      -> when calling mexFunction:
//          - prhs[0]: mode (train, test)
//          - prhs[1]: Example class handler pointer
//          - prhs[1]: The input data that the Example class works on
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
        // Check parameters
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("Train: Unexpected arguments.");

        // Get the size of the input array
        mwSize n = mxGetN(prhs[2]);
        double *input = mxGetPr(prhs[2]);
        example_instance->train(input, n);
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

        // Convert double -> mxArray
        plhs[0] = mxCreateNumericMatrix(1, res.size(), mxDOUBLE_CLASS, mxREAL);
        memcpy(mxGetData(plhs[0]), &res[0], res.size() * sizeof(double));
        return;
    }

    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
