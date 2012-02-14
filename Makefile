# The purpose of this makefile is to setup all elements of the project.

spawner:
	if [ ! -p from-sensor ]; then mkfifo from-sensor; fi && \
	if [ ! -p to-actuator ]; then mkfifo to-actuator; fi && \
	cd modulesTraduction/ && \
	make

clean:
	rm *.log

.PHONY: spawner clean
