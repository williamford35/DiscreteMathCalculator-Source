#include <iostream>
#include <strstream>
using namespace std;

#include "miracl/big.h"
#include "miracl/flash.h"
#include "miracl/ecn.h"

void ECDH (const ECn& G);
pair<Big, Big> ECDSA_sign (const Big& n, const ECn& G, char *file, const Big& private_key, Big& z );
void ECDSA_verify(const ECn& G, const ECn& HA, const Big& z, const Big& n, const pair<Big,Big>& signature, char *file);
Big Hash (FILE *fp);
