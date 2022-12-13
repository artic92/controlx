driver: app.o driver.o channel.o control_law.o
	@gcc -o driver app.o driver.o channel.o control_law.o

driver.o: driver.c app.h
	@gcc -c -g driver.c

app.o : app.c app.h control_law.h channel.h
	@gcc -c -g app.c

channel.o: channel.c channel.h
	@gcc -c -g channel.c

control_law.o: control_law.c control_law.h
	@gcc -c -g control_law.c

clean:
	@rm driver
	@rm *.o
