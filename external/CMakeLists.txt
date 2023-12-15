cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

add_subdirectory(./solanaceae_util)
add_subdirectory(./solanaceae_plugin)

add_subdirectory(./entt)

add_subdirectory(./solanaceae_contact)
add_subdirectory(./solanaceae_message3)

set(SOLANACEAE_BRIDGE_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_bridge)

add_subdirectory(./toxcore)
add_subdirectory(./solanaceae_toxcore)
add_subdirectory(./solanaceae_tox)

add_subdirectory(./solanaceae_ngc_ft1)

add_subdirectory(./solanaceae_zox)

set(SOLANACEAE_TOXIC_GAMES_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_toxic_games)

set(SOLANACEAE_IRCCLIENT_BUILD_PLUGINS ON)
add_subdirectory(./solanaceae_ircclient)

add_subdirectory(./totato)

