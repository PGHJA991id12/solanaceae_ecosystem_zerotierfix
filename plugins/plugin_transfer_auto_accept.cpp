#include <solanaceae/plugin/solana_plugin_v1.h>

#include "./transfer_auto_accept.hpp"
#include <solanaceae/util/config_model.hpp>

#include <entt/entt.hpp>
#include <entt/fwd.hpp>

#include <memory>
#include <limits>
#include <iostream>

static std::unique_ptr<TransferAutoAccept> g_taa = nullptr;

constexpr const char* plugin_name = "TransferAutoAccept";

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return plugin_name;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN " << plugin_name << " START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	try {
		auto* rmm = PLUG_RESOLVE_INSTANCE(RegistryMessageModel);
		auto* conf = PLUG_RESOLVE_INSTANCE(ConfigModelI);

		// static store, could be anywhere tho
		// construct with fetched dependencies
		g_taa = std::make_unique<TransferAutoAccept>(*rmm, *conf);

		// register types
		PLUG_PROVIDE_INSTANCE(TransferAutoAccept, plugin_name, g_taa.get());
	} catch (const ResolveException& e) {
		std::cerr << "PLUGIN " << plugin_name << " " << e.what << "\n";
		return 2;
	}

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN " << plugin_name << " STOP()\n";

	g_taa.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float delta) {
	(void)delta;
	g_taa->iterate();

	return std::numeric_limits<float>::max();
}

} // extern C

