set(USER_CODE_FILES
  #add file.cpp here if it does not have external dependencies
)


include(GNUInstallDirs)

foreach(USER_CODE_FILE ${USER_CODE_FILES})

  get_filename_component(SOURCE_NAME ${USER_CODE_FILE} NAME_WE)

  if (UNIX OR APPLE)
    set(EXE_NAME "${SOURCE_NAME}.bin")
  elseif (WIN32)
    set(EXE_NAME "UserCustomCode")
  endif ()

  message(STATUS "Adding Example ${EXE_NAME}")
  add_executable(${EXE_NAME} ${USER_CODE_FILE})
  target_link_libraries(${EXE_NAME}
	openpose
	${examples_3rdparty_libraries}
	realsense2)

  target_include_directories(${EXE_NAME} PRIVATE
	${CMAKE_INSTALL_LIBDIR}
	${CMAKE_INSTALL_INCLUDEDIR}
	${CMAKE_CURRENT_SOURCE_DIR})


  if (WIN32)
    set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "User Code")
    configure_file(${CMAKE_SOURCE_DIR}/cmake/OpenPose${VCXPROJ_FILE_GPU_MODE}.vcxproj.user
        ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.vcxproj.user @ONLY)
    # Properties->General->Output Directory
    set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
    set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
  endif (WIN32)

endforeach()


#############################################
#for files with external dependencies
#copy/past a block below and modify as needed
#############################################


#########################
#realsenseIntegration.cpp
#########################
get_filename_component(SOURCE_NAME realsenseIntegration.cpp NAME_WE)

if (UNIX OR APPLE)
  set(EXE_NAME "${SOURCE_NAME}.bin")
elseif (WIN32)
  set(EXE_NAME "UserCustomCode")
endif ()

message(STATUS "Adding Example ${EXE_NAME}")
add_executable(${EXE_NAME} realsenseIntegration.cpp)
target_link_libraries(${EXE_NAME}
  openpose
  ${examples_3rdparty_libraries}
  realsense2)
target_include_directories(${EXE_NAME} PRIVATE
  ${CMAKE_INSTALL_LIBDIR}
  ${CMAKE_INSTALL_INCLUDEDIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
  set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "User Code")
  configure_file(${CMAKE_SOURCE_DIR}/cmake/OpenPose${VCXPROJ_FILE_GPU_MODE}.vcxproj.user
    ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.vcxproj.user @ONLY)
  # Properties->General->Output Directory
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
endif (WIN32)

#############
#end of block
#############

############################
#realsense_synchronous_input
############################
get_filename_component(SOURCE_NAME realsense_synchronous_input.cpp NAME_WE)

if (UNIX OR APPLE)
  set(EXE_NAME "${SOURCE_NAME}.bin")
elseif (WIN32)
  set(EXE_NAME "UserCustomCode")
endif ()

message(STATUS "Adding Example ${EXE_NAME}")
add_executable(${EXE_NAME} realsense_synchronous_input.cpp)
target_link_libraries(${EXE_NAME}
  openpose
  ${examples_3rdparty_libraries}
  realsense2
  sioclient
  pthread)
target_include_directories(${EXE_NAME} PRIVATE
  ${CMAKE_INSTALL_LIBDIR}
  ${CMAKE_INSTALL_INCLUDEDIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXE_NAME} boost_system)

if (WIN32)
  set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "User Code")
  configure_file(${CMAKE_SOURCE_DIR}/cmake/OpenPose${VCXPROJ_FILE_GPU_MODE}.vcxproj.user
    ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.vcxproj.user @ONLY)
  # Properties->General->Output Directory
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
endif (WIN32)

#############
#end of block
#############

############################
#faceFinderStreamer
############################
get_filename_component(SOURCE_NAME faceFinderStreamer.cpp NAME_WE)

if (UNIX OR APPLE)
  set(EXE_NAME "${SOURCE_NAME}.bin")
elseif (WIN32)
  set(EXE_NAME "UserCustomCode")
endif ()

message(STATUS "Adding Example ${EXE_NAME}")
add_executable(${EXE_NAME} faceFinderStreamer.cpp)
target_link_libraries(${EXE_NAME}
  openpose
  ${examples_3rdparty_libraries}
  realsense2
  sioclient
  pthread)
target_include_directories(${EXE_NAME} PRIVATE
  ${CMAKE_INSTALL_LIBDIR}
  ${CMAKE_INSTALL_INCLUDEDIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXE_NAME} boost_system)

if (WIN32)
  set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "User Code")
  configure_file(${CMAKE_SOURCE_DIR}/cmake/OpenPose${VCXPROJ_FILE_GPU_MODE}.vcxproj.user
    ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.vcxproj.user @ONLY)
  # Properties->General->Output Directory
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
endif (WIN32)

#############
#end of block
#############

#########################
#test.cpp (siotest)
#########################
get_filename_component(SOURCE_NAME test.cpp NAME_WE)

if (UNIX OR APPLE)
  set(EXE_NAME "${SOURCE_NAME}.bin")
elseif (WIN32)
  set(EXE_NAME "UserCustomCode")
endif ()

message(STATUS "Adding Example ${EXE_NAME}")
add_executable(${EXE_NAME} test.cpp)
target_link_libraries(${EXE_NAME}
  openpose
  ${examples_3rdparty_libraries}
  sioclient
  pthread)
target_include_directories(${EXE_NAME} PRIVATE
  ${CMAKE_INSTALL_LIBDIR}
  ${CMAKE_INSTALL_INCLUDEDIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXE_NAME} boost_system)

if (WIN32)
  set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "User Code")
  configure_file(${CMAKE_SOURCE_DIR}/cmake/OpenPose${VCXPROJ_FILE_GPU_MODE}.vcxproj.user
    ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.vcxproj.user @ONLY)
  # Properties->General->Output Directory
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
  set_property(TARGET ${EXE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
endif (WIN32)

#############
#end of block
#############
