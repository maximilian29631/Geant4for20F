#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TestEm5)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILER mpicxx)

SET(Geant4_LIBRARIES ${Geant4_LIBRARIES} G4mpi)
ADD_LIBRARY(G4mpi SHARED IMPORTED)
SET_PROPERTY(TARGET G4mpi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(G4mpi PROPERTIES
  IMPORTED_LOCATION_RELEASE "${Geant4_DIR}/../g4mpi/lib/libG4mpi.so"
  IMPORTED_SONAME_RELEASE "libG4mpi.so"
)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
		    /mnt/misc/sw/x86_64/Debian/8/geant4/gnu/10.04.02-mpi/lib/g4mpi/include
		   )
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

add_executable(TestEm5 TestEm5.cc ${sources} ${headers} ${TOOLS_FORTRAN_OBJECTS})
target_link_libraries(TestEm5 ${Geant4_LIBRARIES} ${HBOOK_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build TestEm5. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(TestEm5_SCRIPTS
    acosta.mac anthony.mac berger.mac bichsel.mac dedx1.mac dedx2.mac fluo.mac gammaSpectrum.mac gottsch.mac hanson.mac hunger.mac ion.mac kulchi.mac mumsc.mac mutev.mac pixe.mac shen1.mac shen2.mac tavora.mac TestEm5.in TestEm5.out tramu.mac vincour.mac vis.mac
  )

foreach(_script ${TestEm5_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS TestEm5 DESTINATION bin)

