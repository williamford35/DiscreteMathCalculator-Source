#include <miracl/miracl.h>
#include <miracl/big.h>

static void shs_transform(sha *sh);
void shs_init(sha *sh);
void shs_process(sha *sh,int byte);
void shs_hash(sha *sh,char hash[20]);
Big bihash (ifstream &fp);
