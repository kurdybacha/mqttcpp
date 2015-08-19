include_directories(${dl_SOURCE_DIR}/base)
link_directories(${dl_SOURCE_DIR}/base)

add_library(libbase SHARED
	global.h)

target_link_libraries(libbase ${LIBRARIES})

set_target_properties(libbase PROPERTIES
	OUTPUT_NAME base
	VERSION ${VERSION}
	SOVERSION 1
)

install(TARGETS libbase RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
install(FILES global.h DESTINATION ${INCLUDEDIR})
