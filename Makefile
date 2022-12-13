driver: ./bin/obj/driver.o ./bin/obj/control.o ./bin/obj/channel.o ./bin/obj/control_law.o
	@gcc -o ./bin/driver ./bin/obj/driver.o ./bin/obj/control.o ./bin/obj/channel.o ./bin/obj/control_law.o

driver.o: ./src/driver.c ./src/app.h ./src/channel.h ./src/control.h
	@gcc -c -g ./src/driver.c -o ./bin/obj/driver.o

control.o: ./src/control.c ./src/channel.h ./src/control_law.h ./src/app.h
	@gcc -c -g ./src/control.c -o ./bin/obj/control.o

channel.o: ./src/channel.c ./src/channel.h
	@gcc -c -g ./src/channel.c -o ./bin/obj/channel.o

control_law.o: ./src/control_law.c ./src/control_law.h
	@gcc -c -g ./src/control_law.c -o ./bin/obj/control_law.o

clean:
	@rm ./bin/driver
	@rm ./bin/obj/*.o
