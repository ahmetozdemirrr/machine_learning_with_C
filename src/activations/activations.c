/* activations.c */

#include <mlc/activations.h>

/*************************************************************
 * The functions receives the address of the directory to be 
 * activated from the user and returns the same address with 
 * the activation operations. 
 * 
 * The function performs the ReLU-sigmoid-tanh activation in-place 
 * on the input array. The array is modified directly, and no 
 * additional memory is allocated.
 * 
 * Note:
 *  exp() here, defined in official math.h library
 *  exp(x) = e^x
 */

/**********************************
 * Mathematical synopsis of ReLU:
 * 
 * 			x if x > 0,
 * relu = {
 * 			0 if x <= 0
 * 
 **********************************/
void relu(double * inputs, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = (inputs[i] > 0) ? inputs[i] : 0;
	}
}

/**********************************
 * Mathematical synopsis of Sigmoid:
 * 
 * sigmoid(x) = 1 / 1 + e^(-x) 
 * 
 **********************************/
void sigmoid(double * inputs, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = 1.0 / (1.0 + exp(-inputs[i]));
	}
}

/**********************************
 * Mathematical synopsis of tanh:
 * 
 * tanh(x) = 1 / 1 + e^(-x) 
 * 
 * name 'tanh' defined in math.f so:
 **********************************/
void tan_h(double * inputs, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = (exp(inputs[i]) - exp(-inputs[i])) / 
					(exp(inputs[i]) + exp(-inputs[i]));

		/*
		or:
		inputs[i] = tanh(inputs[i]);
		*/
	}
}