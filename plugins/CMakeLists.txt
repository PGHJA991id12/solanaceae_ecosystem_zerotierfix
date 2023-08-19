cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

add_library(plugin_ngcft1 SHARED
	./plugin_ngcft1.cpp
)

target_link_libraries(plugin_ngcft1 PUBLIC
	solanaceae_plugin
	solanaceae_ngcext
	solanaceae_ngcft1
	solanaceae_sha1_ngcft1
)

########################################

add_library(plugin_zox_ngc SHARED
	./plugin_zox_ngc.cpp
)

target_link_libraries(plugin_zox_ngc PUBLIC
	solanaceae_plugin
	solanaceae_zox
)

########################################

add_library(plugin_zox_ngc_hs SHARED
	./plugin_zox_ngc_hs.cpp
)

target_link_libraries(plugin_zox_ngc_hs PUBLIC
	solanaceae_plugin
	solanaceae_zox
)

########################################

add_library(plugin_transfer_auto_accept SHARED
	./plugin_transfer_auto_accept.cpp
	./transfer_auto_accept.hpp
	./transfer_auto_accept.cpp
)

target_link_libraries(plugin_transfer_auto_accept PUBLIC
	solanaceae_plugin
	solanaceae_util
	solanaceae_message3
)
