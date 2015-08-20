include_directories(${mqttcpp_SOURCE_DIR}/network
                    ${mqttcpp_SOURCE_DIR}/base
			${STDBOOL_H_PATH} ${STDINT_H_PATH})
link_directories(${mosquitto_BINARY_DIR}/network)

add_definitions(-std=c++1y)

add_executable(mqttcpp_sub mqttcpp_sub.cpp)

target_link_libraries(mqttcpp_sub libnetwork)

install(TARGETS mqttcpp_sub RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
