#pragma once

#include <solanaceae/message3/registry_message_model.hpp>

#include <vector>

// fwd
struct ConfigModelI;

class TransferAutoAccept : public RegistryMessageModelEventI {
	RegistryMessageModel& _rmm;
	//ContactModelI& _cm;
	ConfigModelI& _conf;

	std::vector<Message3Handle> _accept_queue;

	public:
		TransferAutoAccept(RegistryMessageModel& rmm, ConfigModelI& conf);

		// TODO: iterate
		void iterate(void);

	protected:
		void checkMsg(Message3Handle h);

	protected: // mm
		bool onEvent(const Message::Events::MessageConstruct& e) override;
		bool onEvent(const Message::Events::MessageUpdated& e) override;

};

