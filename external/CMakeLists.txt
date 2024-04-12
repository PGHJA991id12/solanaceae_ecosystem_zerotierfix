cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

add_subdirectory(./solanaceae_util)
add_subdirectory(./solanaceae_plugin)

add_subdirectory(./entt)

add_subdirectory(./solanaceae_contact)
add_subdirectory(./solanaceae_message3)

set(SOLANACEAE_BRIDGE_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_bridge)

set(SOLANACEAE_OBJECT_STORE_BUILD_TESTING ${SOLANACEAE_ECOSYSTEM_BUILD_TESTING})
add_subdirectory(./solanaceae_object_store)

add_subdirectory(./toxcore)
add_subdirectory(./solanaceae_toxcore)
add_subdirectory(./solanaceae_tox)

set(SOLANACEAE_TOX_UPNP_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_tox_upnp)

add_subdirectory(./solanaceae_ngc_ft1)

set(SOLANACEAE_ZOX_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_zox)

set(SOLANACEAE_TOXIC_GAMES_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_toxic_games)

set(SOLANACEAE_IRCCLIENT_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_ircclient)

set(SOLANACEAE_CRDTNOTES_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_crdtnotes)

set(SOLANACEAE_LLAMA-CPP-WEB_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_llama-cpp-web)

add_subdirectory(./totato)

