cmake_minimum_required(VERSION 2.8)

# Add Python
# The following two lines are required on my machine (loong story) adjust them to your taste
set(PYTHON_LIBRARY "/opt/local/Library/Frameworks/Python.framework/Versions/3.3/lib/libpython3.3.dylib")
set(PYTHON_INCLUDE_DIR "/opt/local/Library/Frameworks/Python.framework/Versions/3.3/include/python3.3m/")
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Add Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package(Boost COMPONENTS python system REQUIRED)

# Build library
add_library(unionfind MODULE union_find_py.cc)
set_target_properties(unionfind PROPERTIES PREFIX "")
target_link_libraries(unionfind ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
