cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

add_library(plugin_zox_ngc SHARED
	./plugin_zox_ngc.cpp
)

target_link_libraries(plugin_zox_ngc PUBLIC
	solanaceae_plugin
	solanaceae_zox
)

