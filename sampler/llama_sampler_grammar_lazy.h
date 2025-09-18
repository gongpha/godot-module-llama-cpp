#pragma once

#include "llama_sampler_common.h"
#include "core/variant/typed_array.h"

class LlamaSamplerGrammarLazyPattern : public LlamaSampler {
	GDCLASS(LlamaSamplerGrammarLazyPattern, LlamaSampler);

	String grammar_str;
	String grammar_root;
	PackedStringArray trigger_patterns;
	PackedInt32Array trigger_tokens;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_grammar_str", "str"), &LlamaSamplerGrammarLazyPattern::set_grammar_str);
		ClassDB::bind_method(D_METHOD("get_grammar_str"), &LlamaSamplerGrammarLazyPattern::get_grammar_str);
		ClassDB::bind_method(D_METHOD("set_grammar_root", "root"), &LlamaSamplerGrammarLazyPattern::set_grammar_root);
		ClassDB::bind_method(D_METHOD("get_grammar_root"), &LlamaSamplerGrammarLazyPattern::get_grammar_root);
		ClassDB::bind_method(D_METHOD("set_trigger_patterns", "patterns"), &LlamaSamplerGrammarLazyPattern::set_trigger_patterns);
		ClassDB::bind_method(D_METHOD("get_trigger_patterns"), &LlamaSamplerGrammarLazyPattern::get_trigger_patterns);
		ClassDB::bind_method(D_METHOD("set_trigger_tokens", "tokens"), &LlamaSamplerGrammarLazyPattern::set_trigger_tokens);
		ClassDB::bind_method(D_METHOD("get_trigger_tokens"), &LlamaSamplerGrammarLazyPattern::get_trigger_tokens);
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar_str", PROPERTY_HINT_MULTILINE_TEXT), "set_grammar_str", "get_grammar_str");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar_root"), "set_grammar_root", "get_grammar_root");
		ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "trigger_patterns"), "set_trigger_patterns", "get_trigger_patterns");
		ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "trigger_tokens"), "set_trigger_tokens", "get_trigger_tokens");
	}

	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override {
		ERR_FAIL_COND_V_MSG(vocab == nullptr, nullptr, "requires a valid LlamaVocab to create the sampler");
		CharString gs = grammar_str.utf8();
		CharString gr = grammar_root.utf8();
		const char *gs_c = gs.get_data();
		const char *gr_c = gr.get_data();

		Vector<CharString> tmp;
		tmp.resize(trigger_patterns.size());
		for (int i = 0; i < trigger_patterns.size(); ++i) tmp.write[i] = trigger_patterns[i].utf8();
		Vector<const char*> c_patterns;
		c_patterns.resize(tmp.size());
		for (int i = 0; i < tmp.size(); ++i) c_patterns.write[i] = tmp[i].get_data();
		const char **pat_ptr = c_patterns.size() ? (const char**)c_patterns.ptr() : nullptr;
		size_t pat_count = (size_t)c_patterns.size();

		const llama_token *tok_ptr = (const llama_token*)(trigger_tokens.size() ? trigger_tokens.ptr() : nullptr);
		size_t tok_count = (size_t)trigger_tokens.size();

		return llama_sampler_init_grammar_lazy_patterns(vocab, gs_c, gr_c, pat_ptr, pat_count, tok_ptr, tok_count);
	}

public:
	void set_grammar_str(const String &s) { grammar_str = s; }
	String get_grammar_str() const { return grammar_str; }
	void set_grammar_root(const String &s) { grammar_root = s; }
	String get_grammar_root() const { return grammar_root; }
	void set_trigger_patterns(const PackedStringArray &arr) { trigger_patterns = arr; }
	PackedStringArray get_trigger_patterns() const { return trigger_patterns; }
	void set_trigger_tokens(const PackedInt32Array &arr) { trigger_tokens = arr; }
	PackedInt32Array get_trigger_tokens() const { return trigger_tokens; }
};
