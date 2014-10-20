file (GLOB SOURCE_FILE_LIST src/*.cpp)
file (GLOB HEADER_FILE_LIST include/common/*.*)

add_library (Server_Common STATIC ${SOURCE_FILE_LIST} ${HEADER_FILE_LIST})
target_include_directories(Server_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Server_Common PUBLIC ${BOOST_PATH})
target_include_directories(Server_Common PUBLIC ${POCO_INC_PATH})
target_include_directories(Server_Common PUBLIC ${JSON_INC_PATH})
