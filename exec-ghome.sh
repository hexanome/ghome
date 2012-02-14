# The purpose of this script is to run all elements of the project.


redis-server > redis.log & REDIS_ID=$!

# Spawner (spawns all sensor/actuator threads in gthread).
if [ ! -e ./modulesTraduction/spawner ]; then
  make
fi
./modulesTraduction/spawner > spawner.log & SPAWNER_ID=$!


# Web server.
node serv/app.js > web.log & WEB_ID=$!

# Rules server.
node rules/mum.js > rules.log

kill $REDIS_ID
kill $SPAWNER_ID
kill $WEB_ID

node serv/app.js

