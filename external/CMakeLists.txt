cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(SOLANACEAE_UTIL_BUILD_TESTING ${SOLANACEAE_ECOSYSTEM_BUILD_TESTING})
add_subdirectory(./solanaceae_util)
add_subdirectory(./solanaceae_plugin)

add_subdirectory(./entt)

set(SOLANACEAE_OBJECT_STORE_BUILD_TESTING ${SOLANACEAE_ECOSYSTEM_BUILD_TESTING} CACHE BOOL "")
add_subdirectory(./solanaceae_object_store)

add_subdirectory(./solanaceae_contact)
add_subdirectory(./solanaceae_message3)

set(SOLANACEAE_BRIDGE_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_bridge)

add_subdirectory(./solanaceae_message_serializer)
set(SOLANACEAE_MESSAGE_FRAGMENT_STORE_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_message_fragment_store)

add_subdirectory(./toxcore)
add_subdirectory(./solanaceae_toxcore)
add_subdirectory(./solanaceae_tox)

set(SOLANACEAE_TOX_UPNP_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_tox_upnp)

set(SOLANACEAE_NGCFT1_SHA1_BUILD_TESTING ${SOLANACEAE_ECOSYSTEM_BUILD_TESTING} CACHE BOOL "")
add_subdirectory(./solanaceae_ngc_ft1)

set(SOLANACEAE_ZOX_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_zox)

set(SOLANACEAE_TOXIC_GAMES_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_toxic_games)

set(SOLANACEAE_IRCCLIENT_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_ircclient)

set(SOLANACEAE_CRDTNOTES_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_crdtnotes)

set(SOLANACEAE_LLAMA-CPP-WEB_BUILD_PLUGINS ON CACHE BOOL "")
add_subdirectory(./solanaceae_llama-cpp-web)

add_subdirectory(./totato)

