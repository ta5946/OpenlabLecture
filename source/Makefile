all: helloWorld vectorAdd julia

%: %.cu
	nvcc -arch sm_80 -std=c++17 -Xcompiler -Wall -g -O2 $< -o $@

julia: julia.cu *.h
	nvcc -arch sm_80 -std=c++17 -Xcompiler -Wall -g -O2 $< -o $@
%/julia: %/julia.cu *.h
	nvcc -arch sm_80 -std=c++17 -Xcompiler -Wall -g -O2 $< -o $@

clean:
	rm -f helloWorld julia.png vectorAdd solution/julia.png