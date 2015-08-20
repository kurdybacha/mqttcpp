include_directories(${mqttcpp_SOURCE_DIR}/base
         ${mqttcpp_SOURCE_DIR}/3rdparty/libmosquitto
         ${mqttcpp_SOURCE_DIR}/3rdparty/libmosquitto/cpp
         ${mqttcpp_SOURCE_DIR}/network)

link_directories(${mqttcpp_SOURCE_DIR}/base
         ${mqttcpp_SOURCE_DIR}/3rdparty/libmosquitto
         ${mqttcpp_SOURCE_DIR}/3rdparty/libmosquitto/cpp
         ${mqttcpp_SOURCE_DIR}/network)

add_definitions(-std=c++1y)

add_library(libnetwork SHARED
	session.h session.cpp)

target_link_libraries(libnetwork libmosquittopp ${LIBRARIES})

set_target_properties(libnetwork PROPERTIES
	OUTPUT_NAME network
	VERSION ${VERSION}
	SOVERSION 1
)

install(TARGETS libnetwork RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
install(FILES session.h DESTINATION ${INCLUDEDIR})
