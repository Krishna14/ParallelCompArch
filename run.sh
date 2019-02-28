make clean
for target in singleThreadR singleThreadRW multiThreadR multiThreadRW
do
	make $target
done
	
./singleThreadR.o > output_1T_R.txt
./singleThreadRW.o > output_1T_RW.txt
./multiThreadR.o 2 > output_2T_R.txt
./multiThreadRW.o 2 > output_2T_RW.txt
./multiThread.o 4 > output_4T_R.txt
./multiThreadRW.o 4 > output_4T_RW.txt
./multiThread.o 8 > output_8T_R.txt
./multiThreadRW.o 8 > output_8T_RW.txt
./multiThread.o 16 > output_16T_R.txt
./multiThreadRW.o 16 > output_16T_RW.txt

