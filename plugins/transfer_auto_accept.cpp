#include "./transfer_auto_accept.hpp"

#include <solanaceae/message3/components.hpp>

#include <solanaceae/util/config_model.hpp>

#include <iostream>

TransferAutoAccept::TransferAutoAccept(RegistryMessageModel& rmm, ConfigModelI& conf) : _rmm(rmm), _conf(conf) {
	_rmm.subscribe(this, RegistryMessageModel_Event::message_construct);
	_rmm.subscribe(this, RegistryMessageModel_Event::message_updated);

	if (!_conf.has_string("TransferAutoAccept", "save_path")) {
		_conf.set("TransferAutoAccept", "save_path", std::string_view{"tmp_save_dir"});
	}

	// dont load module instead?
	//if (!_conf.has_bool("TransferAutoAccept", "autoaccept")) {
		//_conf.set("TransferAutoAccept", "autoaccept", false); // safe default
	//}

	if (!_conf.has_int("TransferAutoAccept", "autoaccept_limit")) {
		_conf.set("TransferAutoAccept", "autoaccept_limit", 50l*1024l*1024l); // sane default
	}
}

void TransferAutoAccept::iterate(void) {
	for (auto& it : _accept_queue) {
		if (it.all_of<Message::Components::Transfer::ActionAccept>()) {
			continue; // already accepted
		}

		it.emplace<Message::Components::Transfer::ActionAccept>(
			// TODO: contact to entry
			_conf.get_string("TransferAutoAccept", "save_path").value_or("tmp_save_dir")
		);
		std::cout << "TAA: auto accpeted transfer\n";
		_rmm.throwEventUpdate(it);
	}
	_accept_queue.clear();
}

void TransferAutoAccept::checkMsg(Message3Handle h) {
	if (h.all_of<Message::Components::Transfer::ActionAccept>()) {
		return; // already accepted
	}

	if (!h.all_of<Message::Components::Transfer::TagReceiving, Message::Components::Transfer::TagPaused, Message::Components::Transfer::FileInfo>()) {
		return;
	}

	const auto& file_info = h.get<Message::Components::Transfer::FileInfo>();
	// TODO: contact to entry
	if (file_info.total_size > uint64_t(_conf.get_int("TransferAutoAccept", "autoaccept_limit").value_or(1024*1024))) {
		return; // too large
	}

	if (file_info.file_list.empty() || file_info.file_list.front().file_name.empty()) {
		return; // bad file
	}

	_accept_queue.push_back(h);
}

bool TransferAutoAccept::onEvent(const Message::Events::MessageConstruct& e) {
	std::cout << "TAA: msg c\n";
	checkMsg(e.e);

	return false;
}

bool TransferAutoAccept::onEvent(const Message::Events::MessageUpdated& e) {
	std::cout << "TAA: msg u\n";
	checkMsg(e.e);

	return false;
}

