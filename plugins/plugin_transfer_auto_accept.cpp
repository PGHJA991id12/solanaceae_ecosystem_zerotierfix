#include <solanaceae/plugin/solana_plugin_v1.h>

#include "./transfer_auto_accept.hpp"
#include <solanaceae/util/config_model.hpp>

#include <memory>
#include <limits>
#include <iostream>

#define RESOLVE_INSTANCE(x) static_cast<x*>(solana_api->resolveInstance(#x))
#define PROVIDE_INSTANCE(x, p, v) solana_api->provideInstance(#x, p, static_cast<x*>(v))

static std::unique_ptr<TransferAutoAccept> g_taa = nullptr;

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return "TransferAutoAccept";
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN TAA START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	RegistryMessageModel* rmm = nullptr;
	ConfigModelI* conf = nullptr;

	{ // make sure required types are loaded
		rmm = RESOLVE_INSTANCE(RegistryMessageModel);
		conf = RESOLVE_INSTANCE(ConfigModelI);

		if (rmm == nullptr) {
			std::cerr << "PLUGIN TAA missing RegistryMessageModel\n";
			return 2;
		}

		if (conf == nullptr) {
			std::cerr << "PLUGIN TAA missing ConfigModelI\n";
			return 2;
		}
	}

	// static store, could be anywhere tho
	// construct with fetched dependencies
	g_taa = std::make_unique<TransferAutoAccept>(*rmm, *conf);

	// register types
	PROVIDE_INSTANCE(TransferAutoAccept, "TransferAutoAccept", g_taa.get());

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN TAA STOP()\n";

	g_taa.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float delta) {
	(void)delta;
	//std::cout << "PLUGIN TAA TICK()\n";
	g_taa->iterate();

	return std::numeric_limits<float>::max();
}

} // extern C

