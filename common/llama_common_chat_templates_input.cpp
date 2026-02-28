#include "llama_common_chat_templates_input.h"
#include "llama_common_chat_messages.h"
#include "llama_common_chat_tool.h"
#include "llama_common_utils.h"
#include "llama_common_defined.h"

#include "common/chat.h"

#include "core/object/class_db.h"

void LlamaCommonChatTemplatesInput::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_messages", "messages"), &LlamaCommonChatTemplatesInput::set_messages);
	ClassDB::bind_method(D_METHOD("get_messages"), &LlamaCommonChatTemplatesInput::get_messages);
	ClassDB::bind_method(D_METHOD("set_grammar", "grammar"), &LlamaCommonChatTemplatesInput::set_grammar);
	ClassDB::bind_method(D_METHOD("get_grammar"), &LlamaCommonChatTemplatesInput::get_grammar);
	ClassDB::bind_method(D_METHOD("set_json_schema", "schema"), &LlamaCommonChatTemplatesInput::set_json_schema);
	ClassDB::bind_method(D_METHOD("get_json_schema"), &LlamaCommonChatTemplatesInput::get_json_schema);
	ClassDB::bind_method(D_METHOD("set_add_generation_prompt", "add"), &LlamaCommonChatTemplatesInput::set_add_generation_prompt);
	ClassDB::bind_method(D_METHOD("get_add_generation_prompt"), &LlamaCommonChatTemplatesInput::get_add_generation_prompt);
	ClassDB::bind_method(D_METHOD("set_use_jinja", "use"), &LlamaCommonChatTemplatesInput::set_use_jinja);
	ClassDB::bind_method(D_METHOD("get_use_jinja"), &LlamaCommonChatTemplatesInput::get_use_jinja);
	ClassDB::bind_method(D_METHOD("set_tools", "tools"), &LlamaCommonChatTemplatesInput::set_tools);
	ClassDB::bind_method(D_METHOD("get_tools"), &LlamaCommonChatTemplatesInput::get_tools);
	ClassDB::bind_method(D_METHOD("set_tool_choice", "choice"), &LlamaCommonChatTemplatesInput::set_tool_choice);
	ClassDB::bind_method(D_METHOD("get_tool_choice"), &LlamaCommonChatTemplatesInput::get_tool_choice);
	ClassDB::bind_method(D_METHOD("set_parallel_tool_calls", "parallel"), &LlamaCommonChatTemplatesInput::set_parallel_tool_calls);
	ClassDB::bind_method(D_METHOD("get_parallel_tool_calls"), &LlamaCommonChatTemplatesInput::get_parallel_tool_calls);
	ClassDB::bind_method(D_METHOD("set_reasoning_format", "format"), &LlamaCommonChatTemplatesInput::set_reasoning_format);
	ClassDB::bind_method(D_METHOD("get_reasoning_format"), &LlamaCommonChatTemplatesInput::get_reasoning_format);
	ClassDB::bind_method(D_METHOD("set_enable_thinking", "enable"), &LlamaCommonChatTemplatesInput::set_enable_thinking);
	ClassDB::bind_method(D_METHOD("get_enable_thinking"), &LlamaCommonChatTemplatesInput::get_enable_thinking);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "messages", PROPERTY_HINT_RESOURCE_TYPE, "LlamaCommonChatMessages"), "set_messages", "get_messages");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar"), "set_grammar", "get_grammar");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "json_schema"), "set_json_schema", "get_json_schema");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "add_generation_prompt"), "set_add_generation_prompt", "get_add_generation_prompt");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_jinja"), "set_use_jinja", "get_use_jinja");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "tools"), "set_tools", "get_tools");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tool_choice"), "set_tool_choice", "get_tool_choice");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "parallel_tool_calls"), "set_parallel_tool_calls", "get_parallel_tool_calls");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "reasoning_format"), "set_reasoning_format", "get_reasoning_format");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_thinking"), "set_enable_thinking", "get_enable_thinking");
	ClassDB::bind_method(D_METHOD("set_chat_template_kwargs_dictionary", "kwargs"), &LlamaCommonChatTemplatesInput::set_chat_template_kwargs_dictionary);
	ClassDB::bind_method(D_METHOD("set_chat_template_kwargs", "kwargs"), &LlamaCommonChatTemplatesInput::set_chat_template_kwargs);
	ClassDB::bind_method(D_METHOD("get_chat_template_kwargs"), &LlamaCommonChatTemplatesInput::get_chat_template_kwargs);
	ClassDB::bind_method(D_METHOD("set_enable_bos", "enable"), &LlamaCommonChatTemplatesInput::set_enable_bos);
	ClassDB::bind_method(D_METHOD("get_enable_bos"), &LlamaCommonChatTemplatesInput::get_enable_bos);
	ClassDB::bind_method(D_METHOD("set_enable_eos", "enable"), &LlamaCommonChatTemplatesInput::set_enable_eos);
	ClassDB::bind_method(D_METHOD("get_enable_eos"), &LlamaCommonChatTemplatesInput::get_enable_eos);
}

void LlamaCommonChatTemplatesInput::set_inputs(const common_chat_templates_inputs &p_inputs) {
	inputs = p_inputs;
}

common_chat_templates_inputs LlamaCommonChatTemplatesInput::get_inputs() const {
	return inputs;
}

void LlamaCommonChatTemplatesInput::set_messages(const Ref<LlamaCommonChatMessages> &p_messages) {
	ERR_FAIL_COND(!p_messages.is_valid());
	inputs.messages = p_messages->get_messages_as_std_vector();
}
Ref<LlamaCommonChatMessages> LlamaCommonChatTemplatesInput::get_messages() const {
	if (inputs.messages.empty())
		return Ref<LlamaCommonChatMessages>();
	return LlamaCommonChatMessages::from_std_vector(inputs.messages);
}

void LlamaCommonChatTemplatesInput::set_grammar(const String &p_grammar) { inputs.grammar = p_grammar.utf8().get_data(); }
String LlamaCommonChatTemplatesInput::get_grammar() const { return String::utf8(inputs.grammar.c_str()); }

void LlamaCommonChatTemplatesInput::set_json_schema(const String &p_json_schema) { inputs.json_schema = p_json_schema.utf8().get_data(); }
String LlamaCommonChatTemplatesInput::get_json_schema() const { return String::utf8(inputs.json_schema.c_str()); }

void LlamaCommonChatTemplatesInput::set_add_generation_prompt(bool p_add) { inputs.add_generation_prompt = p_add; }
bool LlamaCommonChatTemplatesInput::get_add_generation_prompt() const { return inputs.add_generation_prompt; }

void LlamaCommonChatTemplatesInput::set_use_jinja(bool p_use) { inputs.use_jinja = p_use; }
bool LlamaCommonChatTemplatesInput::get_use_jinja() const { return inputs.use_jinja; }

void LlamaCommonChatTemplatesInput::set_tools(const TypedArray<LlamaCommonChatTool> &p_tools) {
	inputs.tools.clear();
	for (int i = 0; i < p_tools.size(); ++i) {
		Ref<LlamaCommonChatTool> t = p_tools[i];
		common_chat_tool ct = t->get_tool();
		if (t.is_valid()) inputs.tools.push_back(ct);
	}
}

TypedArray<LlamaCommonChatTool> LlamaCommonChatTemplatesInput::get_tools() const {
	TypedArray<LlamaCommonChatTool> arr;
	for (const auto &t : inputs.tools) {
		Ref<LlamaCommonChatTool> rt;
		rt.instantiate();
		
		rt->set_tool(t);
		arr.push_back(rt);
	}
	return arr;
}

void LlamaCommonChatTemplatesInput::set_tool_choice(LlamaCommonChat::ChatToolChoice p_tool_choice) { inputs.tool_choice = (common_chat_tool_choice)p_tool_choice; }
LlamaCommonChat::ChatToolChoice LlamaCommonChatTemplatesInput::get_tool_choice() const { return (LlamaCommonChat::ChatToolChoice)inputs.tool_choice; }

void LlamaCommonChatTemplatesInput::set_parallel_tool_calls(bool p_parallel) { inputs.parallel_tool_calls = p_parallel; }
bool LlamaCommonChatTemplatesInput::get_parallel_tool_calls() const { return inputs.parallel_tool_calls; }

void LlamaCommonChatTemplatesInput::set_reasoning_format(LlamaCommon::ReasoningFormat p_reasoning_format) { inputs.reasoning_format = (common_reasoning_format)p_reasoning_format; }
LlamaCommon::ReasoningFormat LlamaCommonChatTemplatesInput::get_reasoning_format() const { return (LlamaCommon::ReasoningFormat)inputs.reasoning_format; }

void LlamaCommonChatTemplatesInput::set_enable_thinking(bool p_enable) { inputs.enable_thinking = p_enable; }
bool LlamaCommonChatTemplatesInput::get_enable_thinking() const { return inputs.enable_thinking; }

void LlamaCommonChatTemplatesInput::set_chat_template_kwargs_dictionary(const Dictionary &p_kwargs) { inputs.chat_template_kwargs = godot_typed_dict_to_std_map(TypedDictionary<String, String>(p_kwargs)); }
void LlamaCommonChatTemplatesInput::set_chat_template_kwargs(const TypedDictionary<String, String> &p_kwargs) { inputs.chat_template_kwargs = godot_typed_dict_to_std_map(p_kwargs); }
TypedDictionary<String, String> LlamaCommonChatTemplatesInput::get_chat_template_kwargs() const { return std_map_to_godot_typed_dict(inputs.chat_template_kwargs); }

void LlamaCommonChatTemplatesInput::set_enable_bos(bool p_enable) { inputs.add_bos = p_enable; }
bool LlamaCommonChatTemplatesInput::get_enable_bos() const { return inputs.add_bos; }

void LlamaCommonChatTemplatesInput::set_enable_eos(bool p_enable) { inputs.add_eos = p_enable; }
bool LlamaCommonChatTemplatesInput::get_enable_eos() const { return inputs.add_eos; }

LlamaCommonChatTemplatesInput::LlamaCommonChatTemplatesInput() {
}
LlamaCommonChatTemplatesInput::~LlamaCommonChatTemplatesInput() {
}
