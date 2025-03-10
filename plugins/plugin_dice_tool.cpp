#include <solanaceae/plugin/solana_plugin_v1.h>

#include <solanaceae/contact/contact_store_i.hpp>

#include "./dice_tool.hpp"

#include <imgui.h>

#include <entt/entt.hpp>
#include <entt/fwd.hpp>

#include <memory>
#include <limits>
#include <iostream>

static std::unique_ptr<DiceTool> g_dt = nullptr;

constexpr const char* plugin_name = "DiceTool";

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
		auto* p2prng_i = PLUG_RESOLVE_INSTANCE(P2PRNGI);
		auto* cs = PLUG_RESOLVE_INSTANCE(ContactStore4I);

		// static store, could be anywhere tho
		// construct with fetched dependencies
		g_dt = std::make_unique<DiceTool>(*p2prng_i, *cs);

		auto* imguic = PLUG_RESOLVE_INSTANCE_VERSIONED(ImGuiContext, ImGui::GetVersion());
		auto* imguimemaf = PLUG_RESOLVE_INSTANCE_VERSIONED(ImGuiMemAllocFunc, ImGui::GetVersion());
		auto* imguimemff = PLUG_RESOLVE_INSTANCE_VERSIONED(ImGuiMemFreeFunc, ImGui::GetVersion());
		// meh
		auto* imguimemud = plug_resolveInstanceOptional<void*>(solana_api, "ImGuiMemUserData", ImGui::GetVersion());

		ImGui::SetCurrentContext(imguic);
		ImGui::SetAllocatorFunctions(imguimemaf, imguimemff, imguimemud);

		// register types
		PLUG_PROVIDE_INSTANCE(DiceTool, plugin_name, g_dt.get());
	} catch (const ResolveException& e) {
		std::cerr << "PLUGIN " << plugin_name << " " << e.what << "\n";
		return 2;
	}

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN " << plugin_name << " STOP()\n";

	g_dt.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float delta) {
	(void)delta;
	//g_dt->iterate();

	return std::numeric_limits<float>::max();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_render(float delta) {
	return g_dt->render(delta);
}

} // extern C

