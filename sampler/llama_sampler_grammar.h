#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerGrammar : public LlamaSampler {
	GDCLASS(LlamaSamplerGrammar, LlamaSampler);

	String grammar_str;
	String grammar_root;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_grammar_str", "str"), &LlamaSamplerGrammar::set_grammar_str);
		ClassDB::bind_method(D_METHOD("get_grammar_str"), &LlamaSamplerGrammar::get_grammar_str);
		ClassDB::bind_method(D_METHOD("set_grammar_root", "root"), &LlamaSamplerGrammar::set_grammar_root);
		ClassDB::bind_method(D_METHOD("get_grammar_root"), &LlamaSamplerGrammar::get_grammar_root);
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar_str", PROPERTY_HINT_MULTILINE_TEXT), "set_grammar_str", "get_grammar_str");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar_root"), "set_grammar_root", "get_grammar_root");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override {
		ERR_FAIL_COND_V_MSG(vocab == nullptr, nullptr, "requries a valid LlamaVocab to create the sampler");
		CharString gs = grammar_str.utf8();
		CharString gr = grammar_root.utf8();
		const char *gs_c = gs.get_data();
		const char *gr_c = gr.get_data();
		return llama_sampler_init_grammar(vocab, gs_c, gr_c);
	}

public:
	void set_grammar_str(const String &s) { grammar_str = s; }
	String get_grammar_str() const { return grammar_str; }
	void set_grammar_root(const String &s) { grammar_root = s; }
	String get_grammar_root() const { return grammar_root; }
};
