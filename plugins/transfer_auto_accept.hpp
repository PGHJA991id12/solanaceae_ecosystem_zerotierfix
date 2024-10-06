#pragma once

#include <solanaceae/object_store/object_store.hpp>
#include <solanaceae/message3/registry_message_model.hpp>

#include <vector>

// fwd
struct ConfigModelI;

class TransferAutoAccept : public RegistryMessageModelEventI, public ObjectStoreEventI {
	ObjectStore2& _os;
	RegistryMessageModelI& _rmm;
	//ContactModelI& _cm;
	ConfigModelI& _conf;

	std::vector<ObjectHandle> _accept_queue;

	public:
		TransferAutoAccept(ObjectStore2& os, RegistryMessageModelI& rmm, ConfigModelI& conf);

		// TODO: iterate
		void iterate(void);

	protected:
		void checkObj(ObjectHandle h);
		void checkMsg(Message3Handle h);

	protected: // mm
		bool onEvent(const Message::Events::MessageConstruct& e) override;
		bool onEvent(const Message::Events::MessageUpdated& e) override;

	protected: // os
		bool onEvent(const ObjectStore::Events::ObjectUpdate& e) override;
};

