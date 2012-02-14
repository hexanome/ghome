Broad Design
============

                                     +----------+
                                     | Actuator |
     +--------+    +--------+       /|  Module  |\
     |   Web  |    |  Rules |------' +----------+ \
     | Server |    | Server |------.               `> TCP to
     +--------+    +--------+       \ +--------+  .>  device
           \          /              \| Sensor | /
            \        /                | Module |/
             ________                 +--------+
            (        )
            (  Redis )
            (   DB   )
            (        )
            `--------'

