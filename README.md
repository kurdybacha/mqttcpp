How to test (Mac OS X only for now)
====

Clone and build project:
----
* `$  git clone https://github.com/kladku/mqttcpp`
* `$  cd mqttcpp`
* `$  mkdir build`
* `$  cd build`
* `$  cmake ..`

Install and run mosquitto server:
----
* `$  brew install mosquitto`
* `$  /usr/local/sbin/mosquitto`


Run mqttcpp_client on two different terminals, so you can send messages between them:
----
0. `$  examples/mqttcpp_client user1 user2`
0. `$  examples/mqttcpp_client user2 user1`
