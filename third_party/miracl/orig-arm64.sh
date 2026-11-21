clang -c -O2  mrcore.c
clang -c -O2  mrarth0.c
clang -c -O2  mrarth1.c
clang -c -O2  mrarth2.c
clang -c -O2  mralloc.c
clang -c -O2  mrsmall.c
clang -c -O2  mrio1.c
clang -c -O2  mrio2.c
clang -c -O2  mrgcd.c
clang -c -O2  mrjack.c
clang -c -O2  mrxgcd.c
clang -c -O2  mrarth3.c
clang -c -O2  mrbits.c
clang -c -O2  mrrand.c
clang -c -O2  mrprime.c
clang -c -O2  mrcrt.c
clang -c -O2  mrscrt.c
clang -c -O2  mrmonty.c
clang -c -O2  mrpower.c
clang -c -O2  mrsroot.c
clang -c -O2  mrcurve.c
clang -c -O2  mrfast.c
clang -c -O2  mrlucas.c
clang -c -O2  mrzzn2.c
clang -c -O2  mrzzn2b.c
clang -c -O2  mrzzn3.c
clang -c -O2  mrzzn4.c
clang -c -O2  mrshs.c
clang -c -O2  mrshs256.c
clang -c -O2  mrshs512.c
clang -c -O2  mrsha3.c
clang -c -O2  mrfpe.c
clang -c -O2  mraes.c
clang -c -O2  mrgcm.c
clang -c -O2  mrstrong.c
clang -c -O2  mrbrick.c
clang -c -O2  mrebrick.c
clang -c -O2  mrgf2m.c
clang -c -O2  mrec2m.c
clang -c -O2  mrecn2.c
clang -c -O2  mrflash.c
clang -c -O2  mrfrnd.c
clang -c -O2  mrdouble.c
clang -c -O2  mrround.c
clang -c -O2  mrbuild.c
clang -c -O2  mrflsh1.c
clang -c -O2  mrpi.c
clang -c -O2  mrflsh2.c
clang -c -O2  mrflsh3.c
clang -c -O2  mrflsh4.c
clang -c -O2  mrmuldv.c
clang++ -c -O2 big.cpp
clang++ -c -O2 zzn.cpp
clang++ -c -O2 ecn.cpp
clang++ -c -O2 ec2.cpp
clang++ -c -O2 flash.cpp
clang++ -c -O2 gf2m.cpp

# Create library 'miracl.a'
ar rc miracl.a mrcore.o mrarth0.o mrarth1.o mrarth2.o mralloc.o mrsmall.o mrzzn2.o mrzzn3.o
ar r miracl.a mrio1.o mrio2.o mrjack.o mrgcd.o mrxgcd.o mrarth3.o mrbits.o mrecn2.o mrzzn4.o
ar r miracl.a mrrand.o mrprime.o mrcrt.o mrscrt.o mrmonty.o mrcurve.o mrsroot.o mrzzn2b.o
ar r miracl.a mrpower.o mrfast.o mrshs.o mrshs256.o mraes.o mrlucas.o mrstrong.o mrgcm.o    
ar r miracl.a mrflash.o mrfrnd.o mrdouble.o mrround.o mrbuild.o
ar r miracl.a mrflsh1.o mrpi.o mrflsh2.o mrflsh3.o mrflsh4.o 
ar r miracl.a mrbrick.o mrebrick.o mrec2m.o mrgf2m.o mrshs512.o mrsha3.o mrfpe.o
ar r miracl.a big.o flash.o zzn.o gf2m.o ecn.o ec2.o

