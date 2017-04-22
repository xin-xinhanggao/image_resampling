resample: main.o psnr.o resample.o
	g++ $$(pkg-config --cflags --libs opencv) main.o resampling.o psnr.o -o resample 

main.o: main.cpp resampling.h
	g++ $$(pkg-config --cflags --libs opencv) -c main.cpp

psnr.o: psnr.h resampling.h psnr.cpp
	g++ $$(pkg-config --cflags --libs opencv) -c psnr.cpp

resample.o: resampling.h resampling.cpp
	g++ $$(pkg-config --cflags --libs opencv) -c resampling.cpp

clean: 
	rm *.o