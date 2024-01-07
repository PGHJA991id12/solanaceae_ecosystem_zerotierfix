#include <solanaceae/plugin/solana_plugin_v1.h>

#include <solanaceae/ngc_ext/ngcext.hpp>
#include <solanaceae/ngc_ft1/ngcft1.hpp>
#include <solanaceae/ngc_ft1_sha1/sha1_ngcft1.hpp>

#include <memory>
#include <iostream>

#define RESOLVE_INSTANCE(x) static_cast<x*>(solana_api->resolveInstance(#x))
#define PROVIDE_INSTANCE(x, p, v) solana_api->provideInstance(#x, p, static_cast<x*>(v))

static std::unique_ptr<NGCEXTEventProvider> g_ngcextep = nullptr;
// TODO: make sep plug
static std::unique_ptr<NGCFT1> g_ngcft1 = nullptr;
static std::unique_ptr<SHA1_NGCFT1> g_sha1_ngcft1 = nullptr;

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return "NGCEXT";
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN NGCEXT START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	ToxI* tox_i = nullptr;
	ToxEventProviderI* tox_event_provider_i = nullptr;
	Contact3Registry* cr = nullptr;
	RegistryMessageModel* rmm = nullptr;
	ToxContactModel2* tcm = nullptr;

	{ // make sure required types are loaded
		tox_i = RESOLVE_INSTANCE(ToxI);
		tox_event_provider_i = RESOLVE_INSTANCE(ToxEventProviderI);
		cr = RESOLVE_INSTANCE(Contact3Registry);
		rmm = RESOLVE_INSTANCE(RegistryMessageModel);
		tcm = RESOLVE_INSTANCE(ToxContactModel2);

		if (tox_i == nullptr) {
			std::cerr << "PLUGIN NGCEXT missing ToxI\n";
			return 2;
		}

		if (tox_event_provider_i == nullptr) {
			std::cerr << "PLUGIN NGCEXT missing ToxEventProviderI\n";
			return 2;
		}

		if (cr == nullptr) {
			std::cerr << "PLUGIN NGCEXT missing Contact3Registry\n";
			return 2;
		}

		if (rmm == nullptr) {
			std::cerr << "PLUGIN NGCEXT missing RegistryMessageModel\n";
			return 2;
		}

		if (tcm == nullptr) {
			std::cerr << "PLUGIN NGCEXT missing ToxContactModel2\n";
			return 2;
		}
	}

	// static store, could be anywhere tho
	// construct with fetched dependencies
	g_ngcextep = std::make_unique<NGCEXTEventProvider>(*tox_event_provider_i);
	g_ngcft1 = std::make_unique<NGCFT1>(*tox_i, *tox_event_provider_i, *g_ngcextep.get());
	g_sha1_ngcft1 = std::make_unique<SHA1_NGCFT1>(*cr, *rmm, *g_ngcft1.get(), *tcm);

	// register types
	PROVIDE_INSTANCE(NGCEXTEventProviderI, "NGCEXT", g_ngcextep.get());

	PROVIDE_INSTANCE(NGCFT1EventProviderI, "NGCEXT", g_ngcft1.get());
	PROVIDE_INSTANCE(NGCFT1, "NGCEXT", g_ngcft1.get());

	PROVIDE_INSTANCE(SHA1_NGCFT1, "NGCEXT", g_sha1_ngcft1.get());

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN NGCEXT STOP()\n";

	g_sha1_ngcft1.reset();
	g_ngcft1.reset();
	g_ngcextep.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float delta) {
	const float ft_interval = g_ngcft1->iterate(delta);
	g_sha1_ngcft1->iterate(delta);
	return ft_interval;
}

} // extern C

