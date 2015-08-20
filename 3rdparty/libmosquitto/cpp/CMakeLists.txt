include_directories(${mqttcpp_SOURCE_DIR}/3rdparty/libmosquitto ${mqttcpp_SOURCE_DIR}/3rdparty/libmosquitto
			${STDBOOL_H_PATH} ${STDINT_H_PATH})
link_directories(${mqttcpp_BINARY_DIR}/3rdparty/libmosquitto)

add_library(libmosquittopp SHARED
	mosquittopp.cpp mosquittopp.h)

target_link_libraries(libmosquittopp libmosquitto)
set_target_properties(libmosquittopp PROPERTIES
	OUTPUT_NAME mosquittopp
	VERSION ${VERSION}
	SOVERSION 1
)
install(TARGETS libmosquittopp RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
install(FILES mosquittopp.h DESTINATION ${INCLUDEDIR})

if (UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif (UNIX)
