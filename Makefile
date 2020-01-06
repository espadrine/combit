SHELL = bash
CCFLAGS =
# We support multiple flags, such as changing the gear size:
#CCFLAGS = -DCOMBIT_REGSIZE=8
# …or enabling debugging mode:
#CCFLAGS = -DCOMBIT_DEBUG=2

combit: combit.*
	gcc -O9 -mavx2 $(CCFLAGS) -o combit combit.c

/usr/local/bin/RNG_test:
	mkdir PractRand
	curl -Ls 'https://downloads.sourceforge.net/project/pracrand/PractRand-pre0.95.zip' >PractRand/PractRand.zip
	cd PractRand; \
	  unzip PractRand.zip; \
	  g++ -c src/*.cpp src/RNGs/*.cpp src/RNGs/other/*.cpp -O3 -Iinclude -pthread; \
	  ar rcs libPractRand.a *.o; \
	  g++ -o RNG_test tools/RNG_test.cpp libPractRand.a -O3 -Iinclude -pthread; \
	  g++ -o RNG_benchmark tools/RNG_benchmark.cpp libPractRand.a -O3 -Iinclude -pthread; \
	  g++ -o RNG_output tools/RNG_output.cpp libPractRand.a -O3 -Iinclude -pthread
	sudo mv PractRand/RNG_{test,benchmark,output} /usr/local/bin
	rm -rf PractRand

/usr/local/bin/testu01: testu01.c
	curl -sO 'http://simul.iro.umontreal.ca/testu01/TestU01.zip'
	unzip TestU01.zip
	mv TestU01-*/ TestU01
	cd TestU01; \
	  ./configure --prefix="$$(dirname $$(pwd))"; \
	  make; make install
	gcc -std=c99 -Wall -O3 -o testu01 testu01.c -Iinclude -Llib -ltestu01 -lprobdist -lmylib -lm
	sudo mv testu01 /usr/local/bin
	rm -rf TestU01*

test: test/PractRand test/BigCrush test/perf

test/PractRand: /usr/local/bin/RNG_test combit
	./combit | RNG_test stdin64 | tee test/PractRand

test/BigCrush: /usr/local/bin/testu01 combit
	./combit | testu01 --big | tee test/BigCrush

# This must be performed with no other processes running.
test/perf: combit
	echo "PRNG fingerprint: $$(./combit | tr -dc a-z | head -c 10; echo)" >test/perf
	./combit --bytes 4294967296 2>&1 >/dev/null | tee test/perf

# TODO: seed diffusion analysis: avalanche effect
# (1-bit difference effect on output probability.)

# TODO: correlated seed analysis (with PractRand).
