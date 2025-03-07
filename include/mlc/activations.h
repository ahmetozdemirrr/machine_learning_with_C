/* include/mlc/activations.h */

#ifndef MLC_ACTIVATIONS_H
#define MLC_ACTIVATIONS_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <mlc/config.h>


/* Activation functions: */

/*************************************************************
 * The functions receive the address of the input vector to be 
 * activated from the user and modify it with the activation 
 * operations in-place.
 * 
 * The functions perform ReLU, sigmoid, tanh, leaky ReLU, softmax, 
 * or swish activation on the input array. The array is modified 
 * directly, and no additional memory is allocated.
 * 
 * Note:
 *  exp() is defined in the official math.h library
 *  exp(x) = e^x
 * 
 * Activation functions return -1 if the input vector is NULL or 
 * the size parameter is 0. Otherwise, if the process is successful, 
 * they return 0.
 *************************************************************/

static inline int 
check_inputs(double * inputs, size_t size) 
{
    if (inputs == NULL || size == 0)
    {
        LOG_ERROR("`inputs` NULL or size of `inputs` is 0");
        return -1;
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of ReLU:
 * 
 * 			x if x > 0,
 * relu = {
 * 			0 if x <= 0
 * 
 **********************************/
static inline int 
relu(double * inputs, size_t size)
{
	if (check_inputs(inputs, size) != 0) return -1;

	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = (inputs[i] > 0) ? inputs[i] : 0;
	}
	return 0;
}

/**********************************
 * Mathematical synopsis of Sigmoid:
 * 
 * sigmoid(x) = 1 / 1 + e^(-x) 
 * 
 **********************************/
static inline int 
sigmoid(double * inputs, size_t size)
{
	if (check_inputs(inputs, size) != 0) return -1;

	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = 1.0 / (1.0 + exp(-inputs[i]));
	}
	return 0;
}

/**********************************
 * Mathematical synopsis of tanh:
 * 
 * tanh(x) = (e^(x) - e^(-x)) / (e^(x) + e^(-x))
 * 
 * name 'tanh' defined in math.h so,
 * we will tanh_ name...
 **********************************/
static inline int 
tanh_(double * inputs, size_t size)
{
	if (check_inputs(inputs, size) != 0) return -1;

	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = (exp(inputs[i]) - exp(-inputs[i])) / 
					(exp(inputs[i]) + exp(-inputs[i]));
	}
	return 0;
}

/**********************************
 * Mathematical synopsis of Leaky ReLU:
 * 
 * 			       x if x > 0,
 * leaky relu = {
 * 			       alpha * x if x <= 0
 * 
 **********************************/
static inline int 
leaky_relu(double * inputs, size_t size, double alpha)
{
	if (check_inputs(inputs, size) != 0) return -1;

	for (size_t i = 0; i < size; ++i)
	{
		inputs[i] = (inputs[i] > 0) ? inputs[i] : alpha * inputs[i];
	}
	return 0;
}

/**********************************
 * Mathematical synopsis of Softmax:
 * 
 * softmax(x_i) = e^(x_i) / Σ e^(x_j)
 * 
 * Note: Computes probabilities across 
 * the vector, using max subtraction 
 * for numerical stability.
 **********************************/
static inline int 
softmax(double * inputs, size_t size)
{
	if (check_inputs(inputs, size) != 0) return -1;

	/* Step 1: Find the maximum element for numerical stability */
	double max = inputs[0];

	for (size_t i = 1; i < size; ++i)
	{
		max = (inputs[i] > max) ? inputs[i] : max;
	}

	/* Step 2: Compute exponentials in-place and their sum */
    double sum = 0.0;

    for (size_t i = 0; i < size; ++i) 
    {
        inputs[i] = exp(inputs[i] - max);
        sum += inputs[i];
    }

	/* Step 3: Normalize to get probabilities */
    for (size_t i = 0; i < size; ++i) 
    {
        inputs[i] /= sum;
    }
	return 0;
}

/**********************************
 * Mathematical synopsis of Swish:
 * 
 * swish(x) = x * sigmoid(x) = x / (1 + e^(-x))
 * 
 **********************************/
static inline int 
swish(double * inputs, size_t size)
{
	if (check_inputs(inputs, size) != 0) return -1;

	for (size_t i = 0; i < size; ++i)
	{
		double val_optm = inputs[i];
		inputs[i] = val_optm * (1.0 / (1.0 + exp(-val_optm)));
	}
	return 0;
}

#endif /* MLC_ACTIVATIONS_H */