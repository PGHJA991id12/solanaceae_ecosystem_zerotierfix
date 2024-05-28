cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

add_library(plugin_ngcft1 MODULE
	./plugin_ngcft1.cpp
)
set_target_properties(plugin_ngcft1 PROPERTIES
	C_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON # do we need this?
	POSITION_INDEPENDENT_CODE ON
)
target_compile_definitions(plugin_ngcft1 PUBLIC ENTT_API_IMPORT)

target_link_libraries(plugin_ngcft1 PUBLIC
	solanaceae_plugin
	solanaceae_ngcext
	solanaceae_ngcft1
	solanaceae_sha1_ngcft1
)

########################################

add_library(plugin_transfer_auto_accept MODULE
	./plugin_transfer_auto_accept.cpp
	./transfer_auto_accept.hpp
	./transfer_auto_accept.cpp
)
set_target_properties(plugin_transfer_auto_accept PROPERTIES
	C_VISIBILITY_PRESET hidden
)
target_compile_definitions(plugin_transfer_auto_accept PUBLIC ENTT_API_IMPORT)

target_link_libraries(plugin_transfer_auto_accept PUBLIC
	solanaceae_plugin
	solanaceae_util
	solanaceae_message3
	solanaceae_tox_messages # sad, for filekind
)
