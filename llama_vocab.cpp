#include "llama_vocab.h"
#include "llama_model_instance.h"

#include "core/error/error_macros.h"

#include "thirdparty/llama/include/llama.h"

#include <vector>
#include <string>

LlamaVocab::LlamaVocab() : vocab(nullptr) {
}

LlamaVocab::~LlamaVocab() {
	destroy();
}

void LlamaVocab::_bind_methods() {
	ClassDB::bind_method(D_METHOD("init_from_model", "model"), &LlamaVocab::init_from_model);
	ClassDB::bind_method(D_METHOD("destroy"), &LlamaVocab::destroy);
	ClassDB::bind_method(D_METHOD("is_valid"), &LlamaVocab::is_valid);

	ClassDB::bind_method(D_METHOD("get_token_count"), &LlamaVocab::get_token_count);
	ClassDB::bind_method(D_METHOD("get_vocab_type"), &LlamaVocab::get_vocab_type);

	ClassDB::bind_method(D_METHOD("bos"), &LlamaVocab::bos);
	ClassDB::bind_method(D_METHOD("eos"), &LlamaVocab::eos);
	ClassDB::bind_method(D_METHOD("eot"), &LlamaVocab::eot);
	ClassDB::bind_method(D_METHOD("sep"), &LlamaVocab::sep);
	ClassDB::bind_method(D_METHOD("nl"), &LlamaVocab::nl);
	ClassDB::bind_method(D_METHOD("pad"), &LlamaVocab::pad);
	ClassDB::bind_method(D_METHOD("mask"), &LlamaVocab::mask);
	ClassDB::bind_method(D_METHOD("add_bos"), &LlamaVocab::add_bos);
	ClassDB::bind_method(D_METHOD("add_eos"), &LlamaVocab::add_eos);
	ClassDB::bind_method(D_METHOD("add_sep"), &LlamaVocab::add_sep);

	ClassDB::bind_method(D_METHOD("is_eog", "token"), &LlamaVocab::is_eog);
	ClassDB::bind_method(D_METHOD("is_control", "token"), &LlamaVocab::is_control);
	ClassDB::bind_method(D_METHOD("get_text", "token"), &LlamaVocab::get_text);
	ClassDB::bind_method(D_METHOD("get_score", "token"), &LlamaVocab::get_score);
	ClassDB::bind_method(D_METHOD("token_to_piece", "token", "lstrip", "special"), &LlamaVocab::token_to_piece, DEFVAL(0), DEFVAL(true));

	ClassDB::bind_method(D_METHOD("tokenize", "text", "add_special", "parse_special"), &LlamaVocab::tokenize, DEFVAL(true), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("detokenize", "tokens", "remove_special", "unparse_special"), &LlamaVocab::detokenize, DEFVAL(false), DEFVAL(true));

	BIND_ENUM_CONSTANT(VOCAB_TYPE_NONE);
	BIND_ENUM_CONSTANT(VOCAB_TYPE_SPM);
	BIND_ENUM_CONSTANT(VOCAB_TYPE_BPE);
	BIND_ENUM_CONSTANT(VOCAB_TYPE_WPM);
	BIND_ENUM_CONSTANT(VOCAB_TYPE_UGM);
	BIND_ENUM_CONSTANT(VOCAB_TYPE_RWKV);
	BIND_ENUM_CONSTANT(VOCAB_TYPE_PLAMO2);
}

Error LlamaVocab::init_from_model(LlamaModelInstance *p_model) {
	destroy();
	ERR_FAIL_COND_V(p_model == nullptr, ERR_INVALID_PARAMETER);
	vocab = llama_model_get_vocab(p_model->get_model_ptr());
	ERR_FAIL_COND_V(vocab == nullptr, ERR_CANT_CREATE);
	return OK;
}

void LlamaVocab::destroy() {
	vocab = nullptr;
}

int32_t LlamaVocab::get_token_count() const {
	ERR_FAIL_COND_V(!vocab, 0);
	return llama_vocab_n_tokens(vocab);
}

LlamaVocab::VocabType LlamaVocab::get_vocab_type() const {
	ERR_FAIL_COND_V(!vocab, VOCAB_TYPE_NONE);
	return (VocabType)llama_vocab_type(vocab);
}

int32_t LlamaVocab::bos() const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_bos(vocab); }
int32_t LlamaVocab::eos() const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_eos(vocab); }
int32_t LlamaVocab::eot() const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_eot(vocab); }
int32_t LlamaVocab::sep() const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_sep(vocab); }
int32_t LlamaVocab::nl()  const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_nl (vocab); }
int32_t LlamaVocab::pad() const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_pad(vocab); }
int32_t LlamaVocab::mask()const { ERR_FAIL_COND_V(!vocab, -1); return (int32_t)llama_vocab_mask(vocab); }
bool LlamaVocab::add_bos() const { ERR_FAIL_COND_V(!vocab, false); return llama_vocab_get_add_bos(vocab); }
bool LlamaVocab::add_eos() const { ERR_FAIL_COND_V(!vocab, false); return llama_vocab_get_add_eos(vocab); }
bool LlamaVocab::add_sep() const { ERR_FAIL_COND_V(!vocab, false); return llama_vocab_get_add_sep(vocab); }

bool LlamaVocab::is_eog(int32_t p_token) const { ERR_FAIL_COND_V(!vocab, false); return llama_vocab_is_eog(vocab, (llama_token)p_token); }
bool LlamaVocab::is_control(int32_t p_token) const { ERR_FAIL_COND_V(!vocab, false); return llama_vocab_is_control(vocab, (llama_token)p_token); }
String LlamaVocab::get_text(int32_t p_token) const { ERR_FAIL_COND_V(!vocab, String()); return String::utf8(llama_vocab_get_text(vocab, (llama_token)p_token)); }
double LlamaVocab::get_score(int32_t p_token) const { ERR_FAIL_COND_V(!vocab, 0.0); return (double)llama_vocab_get_score(vocab, (llama_token)p_token); }

String LlamaVocab::token_to_piece(int32_t p_token, int32_t p_lstrip, bool p_special) const {
	ERR_FAIL_COND_V(!vocab, String());
	ERR_FAIL_COND_V(p_token < 0 || p_token >= get_token_count(), String());
	
	char buf[512];
	int32_t n = llama_token_to_piece(vocab, (llama_token)p_token, buf, (int32_t)sizeof(buf), p_lstrip, p_special);
	if (n <= 0) return String();
	return String::utf8(buf, MIN(n, (int32_t)sizeof(buf)));
}

PackedInt32Array LlamaVocab::tokenize(const String &p_text, bool p_add_special, bool p_parse_special) const {
	PackedInt32Array out;
	ERR_FAIL_COND_V(!vocab, out);
	CharString cs = p_text.utf8();
	const char *txt = cs.get_data();
	int32_t len = (int32_t)cs.length();
	// First pass to get size
	int32_t need = llama_tokenize(vocab, txt, len, nullptr, 0, p_add_special, p_parse_special);
	if (need < 0) {
		// negative means how many would have been produced
		need = -need;
	}
	std::vector<llama_token> tmp((size_t)need);
	int32_t n = llama_tokenize(vocab, txt, len, tmp.data(), need, p_add_special, p_parse_special);
	if (n < 0) n = -n;
	out.resize(n);
	for (int i = 0; i < n; ++i) out.set(i, (int32_t)tmp[(size_t)i]);
	return out;
}

String LlamaVocab::detokenize(const PackedInt32Array &p_tokens, bool p_remove_special, bool p_unparse_special) const {
	ERR_FAIL_COND_V(!vocab, String());
	std::vector<llama_token> toks;
	toks.reserve((size_t)p_tokens.size());
	for (int i = 0; i < p_tokens.size(); ++i) toks.push_back((llama_token)p_tokens[i]);
	// first pass for size
	int32_t need = llama_detokenize(vocab, toks.data(), (int32_t)toks.size(), nullptr, 0, p_remove_special, p_unparse_special);
	if (need < 0) need = -need;
	std::string buf((size_t)need, '\0');
	int32_t got = llama_detokenize(vocab, toks.data(), (int32_t)toks.size(), buf.data(), need, p_remove_special, p_unparse_special);
	if (got < 0) got = -got;
	return String::utf8(buf.c_str(), got);
}
