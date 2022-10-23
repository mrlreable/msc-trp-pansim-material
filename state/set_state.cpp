#include "mex.hpp"
#include "mexAdapter.hpp"
#include <iostream>
#include <string.h>
#include "state.hpp"

using matlab::mex::ArgumentList;

class MexFunction : public matlab::mex::Function
{
public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
    {
        _state = (int)inputs[0][0];
        std::cout << "State set to " + std::to_string(_state) << std::endl;
    }

    void checkArguments()
    {
        
    }

    // std::string to_string(TypedArray<double>& arr)
    // {
    //     std::string res = "[ ";
    //     for (auto it = arr.begin(); it < arr.end(); ++it)
    //     {
    //         res += std::to_string(*it) + " ";
    //     }
    //     res += "]";
    //     return res;
    // }
};