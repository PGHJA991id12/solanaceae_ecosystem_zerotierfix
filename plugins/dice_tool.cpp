#include "./dice_tool.hpp"

#include <solanaceae/contact/contact_store_i.hpp>
#include <solanaceae/contact/components.hpp>

#include <imgui.h>

#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/handle.hpp>

#include <cstdint>
#include <iostream>

DiceTool::DiceTool(P2PRNGI& p2prng, ContactStore4I& cs) : _p2prng(p2prng), _cs(cs) {
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

		static entt::dense_set<Contact4> peers;

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

				ContactHandle4 c = _cs.contactHandle(*it);

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
				for (const auto& [cv] : _cs.registry().view<Contact::Components::TagBig>().each()) {
					ContactHandle4 c = _cs.contactHandle(cv);

					if (peers.contains(c)) {
						continue;
					}

					const char* str_ptr = "<unk>";
					if (const auto* name_ptr = c.try_get<Contact::Components::Name>(); name_ptr != nullptr && !name_ptr->name.empty()) {
						str_ptr = name_ptr->name.c_str();
					}

					if (c.all_of<Contact::Components::TagGroup, Contact::Components::ParentOf>()) {
						if (ImGui::BeginMenu(str_ptr)) {
							for (const Contact4 child_cv : c.get<Contact::Components::ParentOf>().subs) {
								ContactHandle4 child_c = _cs.contactHandle(child_cv);

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
			//std::vector<ContactHandle4> c_vec{peers.cbegin(), peers.cend()};
			std::vector<ContactHandle4> c_vec;
			for (const auto cv : peers) {
				c_vec.emplace_back(_cs.contactHandle(cv));
			}

			std::vector<uint8_t> is {'D', 'I', 'C', 'E'};
			is.push_back(reinterpret_cast<uint8_t*>(&g_sides)[0]);
			is.push_back(reinterpret_cast<uint8_t*>(&g_sides)[1]);
			static_assert(sizeof(g_sides) == 2);

			auto new_id = _p2prng.newGernationPeers(c_vec, ByteSpan{is});
			//if (!new_id.empty()) {
			//}
		}

		ImGui::SeparatorText("Rolls");

		// list of past rolls and their state
		for (auto it = _rolls.crbegin(); it != _rolls.crend(); it++) {
			const auto& roll = *it;

			std::string text{"d"};
			text += std::to_string(roll.sides);
			text += " [";
			if (roll.state == P2PRNG::DONE) {
				// dice start at 1
				text += std::to_string(int(roll.final_result)+1);
			} else {
				text += "?";
			}
			text += "]";

			if (roll.state == P2PRNG::INIT) {
				text += " INIT";
			} else if (roll.state == P2PRNG::HMAC) {
				text += " HMAC ";
				text += std::to_string(roll.state_number_1);
				text += "/";
				text += std::to_string(roll.state_number_2);
			} else if (roll.state == P2PRNG::SECRET) {
				text += " SECRET ";
				text += std::to_string(roll.state_number_1);
				text += "/";
				text += std::to_string(roll.state_number_2);
			}

			ImGui::TextUnformatted(text.c_str());
		}
	}
	ImGui::End();

	return 10.f;
}

bool DiceTool::onEvent(const P2PRNG::Events::Init& e) {
	if (e.initial_state.size != 4+2) {
		return false;
	}

	if (e.initial_state[0] != 'D' || e.initial_state[1] != 'I' || e.initial_state[2] != 'C' || e.initial_state[3] != 'E') {
		return false;
	}

	auto& new_roll = _rolls.emplace_back();
	new_roll.id = {e.id.cbegin(), e.id.cend()};
	new_roll.state = P2PRNG::State::INIT;
	reinterpret_cast<uint8_t*>(&new_roll.sides)[0] = e.initial_state[4];
	reinterpret_cast<uint8_t*>(&new_roll.sides)[1] = e.initial_state[5];

	return true;
}

bool DiceTool::onEvent(const P2PRNG::Events::HMAC& e) {
	auto roll_it = std::find_if(_rolls.begin(), _rolls.end(), [&e](const auto& a) -> bool { return ByteSpan{a.id} == e.id; });
	if (roll_it == _rolls.cend()) {
		return false;
	}

	roll_it->state = P2PRNG::State::HMAC;
	roll_it->state_number_1 = e.have;
	roll_it->state_number_2 = e.out_of;

	return true;
}

bool DiceTool::onEvent(const P2PRNG::Events::Secret& e) {
	auto roll_it = std::find_if(_rolls.begin(), _rolls.end(), [&e](const auto& a) -> bool { return ByteSpan{a.id} == e.id; });
	if (roll_it == _rolls.cend()) {
		return false;
	}

	roll_it->state = P2PRNG::State::SECRET;
	roll_it->state_number_1 = e.have;
	roll_it->state_number_2 = e.out_of;

	return true;
}

bool DiceTool::onEvent(const P2PRNG::Events::Done& e) {
	auto roll_it = std::find_if(_rolls.begin(), _rolls.end(), [&e](const auto& a) -> bool { return ByteSpan{a.id} == e.id; });
	if (roll_it == _rolls.cend()) {
		return false;
	}

	roll_it->state = P2PRNG::State::DONE;
	roll_it->state_number_1 = 0;
	roll_it->state_number_2 = 0;
	roll_it->final_result = (e.result[0] | (e.result[1] << 8)) % roll_it->sides;

	std::cout << "done die roll " << roll_it->final_result << "\n";
	return true;
}

bool DiceTool::onEvent(const P2PRNG::Events::ValError& e) {
	auto roll_it = std::find_if(_rolls.cbegin(), _rolls.cend(), [&e](const auto& a) -> bool { return ByteSpan{a.id} == e.id; });
	if (roll_it == _rolls.cend()) {
		return false;
	}

	return false;
}

