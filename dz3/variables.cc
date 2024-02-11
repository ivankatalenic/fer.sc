#include "variables.hh"

#include "function_builder.hh"

IntUnaryFunction const* Var::distance_close	{FunctionBuilder::lFunction(15, 40)};
IntUnaryFunction const* Var::distance_medium{FunctionBuilder::lambdaFunction(30, 60, 90)};
IntUnaryFunction const* Var::distance_far	{FunctionBuilder::gammaFunction(80, 100)};

IntUnaryFunction const* Var::speed_small	{FunctionBuilder::lFunction(25, 45)};
IntUnaryFunction const* Var::speed_medium	{FunctionBuilder::lambdaFunction(40, 60, 80)};
IntUnaryFunction const* Var::speed_big		{FunctionBuilder::gammaFunction(75, 85)};

IntUnaryFunction const* Var::direction_right{FunctionBuilder::lambdaFunction(0, 1, 2)};
IntUnaryFunction const* Var::direction_wrong{FunctionBuilder::lambdaFunction(-1, 0, 1)};

IntUnaryFunction const* Var::accel_nb		{FunctionBuilder::lFunction(-16, -10)};
IntUnaryFunction const* Var::accel_ns		{FunctionBuilder::lambdaFunction(-8, -5, -2)};
IntUnaryFunction const* Var::accel_zo		{FunctionBuilder::lambdaFunction(-5, 0, 5)};
IntUnaryFunction const* Var::accel_ps		{FunctionBuilder::lambdaFunction(2, 10, 18)};
IntUnaryFunction const* Var::accel_pb		{FunctionBuilder::gammaFunction(10, 16)};

IntUnaryFunction const* Var::omega_nb		{FunctionBuilder::lFunction(-280, -240)};
IntUnaryFunction const* Var::omega_ns		{FunctionBuilder::lambdaFunction(-250, -80, -20)};
IntUnaryFunction const* Var::omega_zo		{FunctionBuilder::lambdaFunction(-40, 0, 40)};
IntUnaryFunction const* Var::omega_ps		{FunctionBuilder::lambdaFunction(20, 80, 250)};
IntUnaryFunction const* Var::omega_pb		{FunctionBuilder::gammaFunction(240, 280)};

IntUnaryFunction const* Var::ignore			{FunctionBuilder::constantFunction()};
