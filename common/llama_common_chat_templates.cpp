#include "llama_common_chat_templates.h"
#include "llama_common_chat_templates_input.h"
#include "llama_common_chat_parameters.h"
#include "llama_common_utils.h"

#include "../llama_model_instance.h"

#include "llama_common_chat_messages.h"
#include "llama_common_chat_message.h"

#include "core/object/class_db.h"

void LlamaCommonChatTemplates::_bind_methods() {
	ClassDB::bind_static_method("LlamaCommonChatTemplates", D_METHOD("init", "model", "chat_template_override", "bos_token_override", "eos_token_override"), &LlamaCommonChatTemplates::init, DEFVAL(String()), DEFVAL(String()));
	ClassDB::bind_method(D_METHOD("is_valid"), &LlamaCommonChatTemplates::is_valid);
	ClassDB::bind_method(D_METHOD("was_explicit"), &LlamaCommonChatTemplates::was_explicit);
	ClassDB::bind_method(D_METHOD("get_source", "variant"), &LlamaCommonChatTemplates::get_source, DEFVAL(String()));
	ClassDB::bind_method(D_METHOD("apply", "inputs"), &LlamaCommonChatTemplates::apply);
	ClassDB::bind_method(D_METHOD("support_enable_thinking"), &LlamaCommonChatTemplates::support_enable_thinking);
	ClassDB::bind_method(D_METHOD("format_single", "past_msg", "new_msg", "add_assistant", "use_jinja"), &LlamaCommonChatTemplates::format_single);
	ClassDB::bind_method(D_METHOD("format_example", "use_jinja", "chat_template_kwargs"), &LlamaCommonChatTemplates::format_example);
}

Ref<LlamaCommonChatTemplates> LlamaCommonChatTemplates::init(
	const LlamaModelInstance *model,
	const String &chat_template_override,
	const String &bos_token_override,
	const String &eos_token_override
) {
	ERR_FAIL_COND_V(!model, Ref<LlamaCommonChatTemplates>());

	Ref<LlamaCommonChatTemplates> r;
	r.instantiate();
	llama_model *m = model->get_model_ptr();
	r->ptr = common_chat_templates_init(m, chat_template_override.utf8().get_data(), bos_token_override.utf8().get_data(), eos_token_override.utf8().get_data());
	return r;
}

bool LlamaCommonChatTemplates::was_explicit() const { return ptr ? common_chat_templates_was_explicit(ptr.get()) : false; }

String LlamaCommonChatTemplates::get_source(const String &variant) const {
	if (!ptr) {
		return String();
	}
	return String(common_chat_templates_source(ptr.get(), variant.is_empty() ? nullptr : variant.utf8().get_data()).c_str());
}

Ref<LlamaCommonChatParameters> LlamaCommonChatTemplates::apply(const Ref<LlamaCommonChatTemplatesInput> &inputs) const {
	ERR_FAIL_COND_V(!inputs.is_valid(), Ref<LlamaCommonChatParameters>());
	ERR_FAIL_COND_V(!ptr, Ref<LlamaCommonChatParameters>());

	Ref<LlamaCommonChatParameters> r;
	r.instantiate();
	common_chat_params p = common_chat_templates_apply(ptr.get(), inputs->get_inputs());
	r->set_params(p);
	return r;
}

bool LlamaCommonChatTemplates::support_enable_thinking() const {
	ERR_FAIL_COND_V(!ptr, false);
	return common_chat_templates_support_enable_thinking(ptr.get());
}

String LlamaCommonChatTemplates::format_single(
	const Ref<LlamaCommonChatMessages> &past_msg,
	const Ref<LlamaCommonChatMessage> &new_msg,
	bool add_assistant,
	bool use_jinja
) {
	ERR_FAIL_COND_V(!ptr, String());
	std::vector<common_chat_msg> pm = past_msg.is_valid() ? past_msg->get_messages_as_std_vector() : std::vector<common_chat_msg>();
	return String::utf8(common_chat_format_single(ptr.get(), pm, new_msg.is_valid() ? new_msg->get_msg() : common_chat_msg(), add_assistant, use_jinja).c_str());
}

String LlamaCommonChatTemplates::format_example(
	bool use_jinja,
	const Dictionary &chat_template_kwargs
) const {
	ERR_FAIL_COND_V(!ptr, String());
	auto map = godot_typed_dict_to_std_map(TypedDictionary<String, String>(chat_template_kwargs));
	return String::utf8(common_chat_format_example(ptr.get(), use_jinja, map).c_str());
}

LlamaCommonChatTemplates::LlamaCommonChatTemplates() {}
LlamaCommonChatTemplates::~LlamaCommonChatTemplates() {}
