# The purpose of this makefile is to setup all elements of the project.

all: db spawner rules web


spawner: modulesTraduction/EnOceanModuleActuator/EnOceanModuleActuator/actuator modulesTraduction/EnOceanModuleTraduction/EnOceanModuleTraduction/sensor
	if [ ! -p from-sensor ]; then mkfifo from-sensor; fi && \
	if [ ! -p to-actuator ]; then mkfifo to-actuator; fi && \
	cd modulesTraduction/ && \
	make && \
	./spawner

db:
	redis-server

web: serv/app.js
	node serv/app.js

rules: rules/mum.js rules/comp/redisbase.js
	node rules/mum.js


.PHONY: all rules db web spawner
