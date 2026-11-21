#ifndef BOB_H
#define BOB_H
#include "miracl/big.h"
#include "miracl/flash.h"
#include "miracl/ecn.h"

// #include "elliptic-curves/buildEllipticCurveParameters.h"

#include <unordered_map>

// Number of sets used in Pollards rho-method.
#define POLLARD_SET_COUNT    16
// This argument is passed to epoint objects' f method (which is a bitwise and).
// The result of f fuction must be no more than POLLARD_SET_COUNT constant.
#define POLLARD_SET_ARG      0xF

bool discreteLogBruteForce (const ECn& P, const ECn& Q, const Big& n, Big& disclog, double& work_time, Big& iterations);
bool discreteLogBabyStepGiantStep (const ECn& P, const ECn& Q, const Big& n, Big& disclog, Big& iterations, double& work_time);
int f (const ECn &p, int arg);
bool pollard (ECn& P, ECn& Q, Big& n, Big& disclog, Big& iterations, double& work_time);

#endif // BOB_H