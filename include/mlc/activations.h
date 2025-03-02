/* activations.h */

#ifndef MLC_ACTIVATIONS_H
#define MLC_ACTIVATIONS_H

#include <stdio.h>
#include <stddef.h> /* for size_t */
#include <math.h>

/* Activation functions: */

void relu(double * inputs, size_t size);
void sigmoid(double * inputs, size_t size);
void tan_h(double * inputs, size_t size);

#endif /* MLC_ACTIVATIONS_H */