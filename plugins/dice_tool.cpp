#include "./dice_tool.hpp"

#include <solanaceae/contact/components.hpp>

#include <imgui.h>

#include <entt/container/dense_set.hpp>

#include <cstdint>
#include <iostream>

DiceTool::DiceTool(P2PRNGI& p2prng, Contact3Registry& cr) : _p2prng(p2prng), _cr(cr) {
	p2prng.subscribe(this, P2PRNG_Event::init);
	p2prng.subscribe(this, P2PRNG_Event::hmac);
	p2prng.subscribe(this, P2PRNG_Event::secret);
	p2prng.subscribe(this, P2PRNG_Event::done);
	p2prng.subscribe(this, P2PRNG_Event::val_error);
}

DiceTool::~DiceTool(void) {
}

float DiceTool::render(float) {
	if (ImGui::Begin("DiceTool")) {
		// header with values for new roll

		ImGui::TextUnformatted("sides:");
		ImGui::SameLine();
		static uint16_t g_sides {6};
		ImGui::InputScalar("##sides", ImGuiDataType_U16, &g_sides);

		static entt::dense_set<Contact3> peers;

		if (ImGui::CollapsingHeader("peers")) {
			ImGui::Indent();
			// list with peers participating

			for (auto it = peers.begin(); it != peers.end();) {
				ImGui::PushID(entt::to_integral(*it));
				if (ImGui::SmallButton("-")) {
					it = peers.erase(it);
					ImGui::PopID();
					continue;
				}

				Contact3Handle c {_cr, *it};

				const char* str_ptr = "<unk>";
				if (const auto* name_ptr = c.try_get<Contact::Components::Name>(); name_ptr != nullptr && !name_ptr->name.empty()) {
					str_ptr = name_ptr->name.c_str();
				}

				ImGui::SameLine();
				ImGui::TextUnformatted(str_ptr);

				ImGui::PopID();
				it++;
			}

			if (ImGui::Button("add")) {
				ImGui::OpenPopup("peer selector");
			}
			if (ImGui::BeginPopup("peer selector")) {
				for (const auto& [cv] : _cr.view<Contact::Components::TagBig>().each()) {
					Contact3Handle c {_cr, cv};

					if (peers.contains(c)) {
						continue;
					}

					const char* str_ptr = "<unk>";
					if (const auto* name_ptr = c.try_get<Contact::Components::Name>(); name_ptr != nullptr && !name_ptr->name.empty()) {
						str_ptr = name_ptr->name.c_str();
					}

					if (c.all_of<Contact::Components::TagGroup, Contact::Components::ParentOf>()) {
						if (ImGui::BeginMenu(str_ptr)) {
							for (const Contact3 child_cv : c.get<Contact::Components::ParentOf>().subs) {
								Contact3Handle child_c {_cr, child_cv};

								if (peers.contains(child_c)) {
									continue;
								}

								const char* child_str_ptr = "<unk>";
								if (const auto* name_ptr = child_c.try_get<Contact::Components::Name>(); name_ptr != nullptr && !name_ptr->name.empty()) {
									child_str_ptr = name_ptr->name.c_str();
								}

								if (ImGui::MenuItem(child_str_ptr)) {
									peers.emplace(child_c);
								}
							}
							ImGui::EndMenu();
						}
					} else {
						if (ImGui::MenuItem(str_ptr)) {
							peers.emplace(c);
						}
					}
				}


				ImGui::EndPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("clear")) {
				peers.clear();
			}

			ImGui::Unindent();
		}

		if (ImGui::Button("roll")) {
			//std::vector<Contact3Handle> c_vec{peers.cbegin(), peers.cend()};
			std::vector<Contact3Handle> c_vec;
			for (const auto cv : peers) {
				c_vec.emplace_back(_cr, cv);
			}

			auto new_id = _p2prng.newGernationPeers(c_vec, ByteSpan{reinterpret_cast<uint8_t*>(&g_sides), sizeof(g_sides)});
			if (!new_id.empty()) {
				auto& new_roll = _rolls.emplace_back();
				new_roll.id = new_id;
			}
		}

		ImGui::SeparatorText("Rolls");

		// list of past rolls and their state
		//ImGui::CollapsingHeader("d");
		ImGui::Text("d6 [?] hmac 4/6");
		ImGui::Text("d6 [?] secret 1/3");
		ImGui::Text("d6 [1]");
	}
	ImGui::End();

	return 10.f;
}

bool DiceTool::onEvent(const P2PRNG::Events::Init&) {
	return false;
}

bool DiceTool::onEvent(const P2PRNG::Events::HMAC&) {
	return false;
}

bool DiceTool::onEvent(const P2PRNG::Events::Secret&) {
	return false;
}

bool DiceTool::onEvent(const P2PRNG::Events::Done&) {
	std::cout << "got a done!!!!!!!!!!!!!!!!!!\n";
	return false;
}

bool DiceTool::onEvent(const P2PRNG::Events::ValError&) {
	return false;
}

