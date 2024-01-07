#include <solanaceae/plugin/solana_plugin_v1.h>

#include <solanaceae/zox/ngc.hpp>

#include <memory>
#include <limits>
#include <iostream>

// fwd
class RegMessageModel;

#define RESOLVE_INSTANCE(x) static_cast<x*>(solana_api->resolveInstance(#x))
#define PROVIDE_INSTANCE(x, p, v) solana_api->provideInstance(#x, p, static_cast<x*>(v))

static std::unique_ptr<ZoxNGCEventProvider> g_zngc = nullptr;

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return "ZoxNGC";
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN ZNGC START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	ToxEventProviderI* tox_event_provider_i = nullptr;

	{ // make sure required types are loaded
		tox_event_provider_i = RESOLVE_INSTANCE(ToxEventProviderI);

		if (tox_event_provider_i == nullptr) {
			std::cerr << "PLUGIN ZNGC missing ToxEventProviderI\n";
			return 2;
		}
	}

	// static store, could be anywhere tho
	// construct with fetched dependencies
	g_zngc = std::make_unique<ZoxNGCEventProvider>(*tox_event_provider_i);

	// register types
	PROVIDE_INSTANCE(ZoxNGCEventProviderI, "ZoxNGC", g_zngc.get());

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN ZNGC STOP()\n";

	g_zngc.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float delta) {
	(void)delta;
	//std::cout << "PLUGIN ZNGC TICK()\n";
	return std::numeric_limits<float>::max();
}

} // extern C

