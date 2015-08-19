include_directories(${dl_SOURCE_DIR}/base
         ${dl_SOURCE_DIR}/3rdparty/libmosquitto
         ${dl_SOURCE_DIR}/network)

link_directories(${dl_SOURCE_DIR}/network ${dl_SOURCE_DIR}/base ${dl_SOURCE_DIR}/3rdparty/libmosquitto)

add_library(libnetwork SHARED
	session.h session.cpp)

target_link_libraries(libnetwork ${LIBRARIES})

set_target_properties(libnetwork PROPERTIES
	OUTPUT_NAME network
	VERSION ${VERSION}
	SOVERSION 1
)

install(TARGETS libnetwork RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
install(FILES session.h DESTINATION ${INCLUDEDIR})
