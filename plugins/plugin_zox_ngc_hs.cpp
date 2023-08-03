#include <solanaceae/plugin/solana_plugin_v1.h>

#include <solanaceae/toxcore/tox_interface.hpp>
#include <solanaceae/zox/ngc_hs.hpp>

#include <memory>
#include <iostream>

// fwd
struct ContactModelI;
class RegistryMessageModel;

#define RESOLVE_INSTANCE(x) static_cast<x*>(solana_api->resolveInstance(#x))
#define PROVIDE_INSTANCE(x, p, v) solana_api->provideInstance(#x, p, static_cast<x*>(v))

static std::unique_ptr<ZoxNGCHistorySync> g_zngchs = nullptr;

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return "ZoxNGCHistorySync";
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN ZNGCHS START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	ToxI* tox_i = nullptr;
	ToxEventProviderI* tox_event_provider_i = nullptr;
	ZoxNGCEventProviderI* zox_ngc_event_provider_i = nullptr;
	Contact3Registry* cr = nullptr;
	ToxContactModel2* tcm = nullptr;
	RegistryMessageModel* rmm = nullptr;

	{ // make sure required types are loaded
		tox_i = RESOLVE_INSTANCE(ToxI);
		tox_event_provider_i = RESOLVE_INSTANCE(ToxEventProviderI);
		zox_ngc_event_provider_i = RESOLVE_INSTANCE(ZoxNGCEventProviderI);
		cr = RESOLVE_INSTANCE(Contact3Registry);
		tcm = RESOLVE_INSTANCE(ToxContactModel2);
		rmm = RESOLVE_INSTANCE(RegistryMessageModel);

		if (tox_i == nullptr) {
			std::cerr << "PLUGIN ZNGCHS missing ToxI\n";
			return 2;
		}

		if (tox_event_provider_i == nullptr) {
			std::cerr << "PLUGIN ZNGCHS missing ToxEventProviderI\n";
			return 2;
		}

		if (zox_ngc_event_provider_i == nullptr) {
			std::cerr << "PLUGIN ZNGCHS missing ZoxNGCEventProviderI\n";
			return 2;
		}

		if (cr == nullptr) {
			std::cerr << "PLUGIN ZNGCHS missing ContactModelI\n";
			return 2;
		}

		if (tcm == nullptr) {
			std::cerr << "PLUGIN ZNGCHS missing ToxContactModel2\n";
			return 2;
		}

		if (rmm == nullptr) {
			std::cerr << "PLUGIN ZNGCHS missing RegistryMessageModel\n";
			return 2;
		}
	}

	// static store, could be anywhere tho
	// construct with fetched dependencies
	g_zngchs = std::make_unique<ZoxNGCHistorySync>(*tox_event_provider_i, *zox_ngc_event_provider_i, *tox_i, *cr, *tcm, *rmm);

	// register types
	PROVIDE_INSTANCE(ZoxNGCHistorySync, "ZoxNGCHistorySync", g_zngchs.get());

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN ZNGCHS STOP()\n";

	g_zngchs.reset();
}

SOLANA_PLUGIN_EXPORT void solana_plugin_tick(float delta) {
	//std::cout << "PLUGIN ZNGCHS TICK()\n";
	g_zngchs->tick(delta);
}

} // extern C

