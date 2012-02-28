# The purpose of this script is to run all elements of the project.


redis-server > redis.log & REDIS_ID=$!

# Spawner (spawns all sensor/actuator threads in gthread).
if [ ! -e ./modulesTraduction/spawner ]; then
  make
fi
./modulesTraduction/spawner > spawner.log & SPAWNER_ID=$!

# Rules server.
node rules/mum.js > rules.log & RULES_ID=$!

cd server && make debug

kill $REDIS_ID
kill $SPAWNER_ID
kill $RULES_ID


