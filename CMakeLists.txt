# Works with 3.10.2
cmake_minimum_required(VERSION 3.10.2)

# Project name and a few useful settings.
# Other commands can pick up the results
project (boid
	VERSION 0.1
	DESCRIPTION "bird-oid object project"
	LANGUAGES CXX)

# The compiled library code is here
add_subdirectory(libs)

# The executable code is here
add_subdirectory(src)