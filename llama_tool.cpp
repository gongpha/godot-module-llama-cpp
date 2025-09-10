#include "llama_tool.h"

#include "core/object/class_db.h"

void LLamaTool::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &LLamaTool::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &LLamaTool::get_name);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &LLamaTool::set_description);
	ClassDB::bind_method(D_METHOD("get_description"), &LLamaTool::get_description);
	ClassDB::bind_method(D_METHOD("set_json_schema", "json_schema"), &LLamaTool::set_json_schema);
	ClassDB::bind_method(D_METHOD("get_json_schema"), &LLamaTool::get_json_schema);
	ClassDB::bind_method(D_METHOD("set_handler", "callable"), &LLamaTool::set_handler);
	ClassDB::bind_method(D_METHOD("get_handler"), &LLamaTool::get_handler);
	ClassDB::bind_method(D_METHOD("invoke", "args"), &LLamaTool::invoke);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description"), "set_description", "get_description");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "json_schema"), "set_json_schema", "get_json_schema");
	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "handler"), "set_handler", "get_handler");
}

Variant LLamaTool::invoke(const Dictionary &p_args) const {
	if (!handler.is_valid()) {
		return Variant();
	}
	Array args;
	args.push_back(p_args);
	const Variant ret = handler.callv(args);
	return ret;
}

void LlamaToolLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_tool", "tool"), &LlamaToolLibrary::add_tool);
	ClassDB::bind_method(D_METHOD("clear"), &LlamaToolLibrary::clear);
	ClassDB::bind_method(D_METHOD("get_count"), &LlamaToolLibrary::get_count);
	ClassDB::bind_method(D_METHOD("get_tool", "index"), &LlamaToolLibrary::get_tool);
	ClassDB::bind_method(D_METHOD("find_tool", "name"), &LlamaToolLibrary::find_tool);
	ClassDB::bind_method(D_METHOD("get_tools"), &LlamaToolLibrary::get_tools);
}

void LlamaToolLibrary::add_tool(const Ref<LLamaTool> &p_tool) {
	if (p_tool.is_valid()) {
		tools.push_back(p_tool);
	}
}

void LlamaToolLibrary::clear() {
	tools.clear();
}

Ref<LLamaTool> LlamaToolLibrary::get_tool(int p_index) const {
	if (p_index < 0 || p_index >= tools.size()) return Ref<LLamaTool>();
	return tools[p_index];
}

Ref<LLamaTool> LlamaToolLibrary::find_tool(const String &p_name) const {
	for (int i = 0; i < tools.size(); ++i) {
		Ref<LLamaTool> t = tools[i];
		if (t.is_valid() && t->get_name() == p_name) return t;
	}
	return Ref<LLamaTool>();
}
