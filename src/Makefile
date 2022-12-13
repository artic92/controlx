driver: driver.o control.o channel.o control_law.o
	@gcc -o driver driver.o control.o channel.o control_law.o

driver.o: driver.c app.h channel.h control.h
	@gcc -c -g driver.c -o driver.o

control.o: control.c channel.h control_law.h app.h
	@gcc -c -g control.c -o control.o

channel.o: channel.c channel.h
	@gcc -c -g channel.c -o channel.o

control_law.o: control_law.c control_law.h
	@gcc -c -g control_law.c -o control_law.o

clean:
	@rm *.o
	@rm driver
