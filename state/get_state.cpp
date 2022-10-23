#include "mex.hpp"
#include "mexAdapter.hpp"
#include <iostream>
#include "state.hpp"

using matlab::mex::ArgumentList;

class MexFunction : public matlab::mex::Function
{
public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
    {
        // Get extern variable from set_state.cpp
        matlab::data::ArrayFactory factory;

        outputs[0] = factory.createScalar<int16_t>(_state);
        outputs[0][0] = _state;

        std::cout << "state retrieved is " + std::to_string(_state) << std::endl;
    }

    void checkArguments()
    {
        
    }
};