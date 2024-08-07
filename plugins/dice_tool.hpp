#pragma once

#include <solanaceae/tox_p2prng/p2prng.hpp>
#include <solanaceae/contact/contact_model3.hpp>

#include <vector>

class DiceTool : public P2PRNGEventI {
	P2PRNGI& _p2prng;
	Contact3Registry& _cr;

	struct Rolls {
		std::vector<uint8_t> id;

		P2PRNG::State state {P2PRNG::State::UNKNOWN};
		uint16_t state_number_1{};
		uint16_t state_number_2{};

		uint16_t final_result{};
	};
	std::vector<Rolls> _rolls;

	public:
		DiceTool(P2PRNGI& p2prng, Contact3Registry& cr);
		~DiceTool(void);

		float render(float time_delta);

	protected: // p2prng
		bool onEvent(const P2PRNG::Events::Init&) override;
		bool onEvent(const P2PRNG::Events::HMAC&) override;
		bool onEvent(const P2PRNG::Events::Secret&) override;
		bool onEvent(const P2PRNG::Events::Done&) override;
		bool onEvent(const P2PRNG::Events::ValError&) override;
};

