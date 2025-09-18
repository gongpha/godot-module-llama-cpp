#include "llama_common_sampler_params.h"

#include "core/object/class_db.h"

static Dictionary _bias_to_dict(const llama_logit_bias &b) {
	Dictionary d; d["token"] = (int32_t)b.token; d["bias"] = b.bias; return d;
}

static llama_logit_bias _dict_to_bias(const Dictionary &d) {
	llama_logit_bias b{}; if (d.has("token")) b.token = (llama_token)(int32_t)d["token"]; if (d.has("bias")) b.bias = (float)(double)d["bias"]; return b;
}

void LlamaCommonSamplerParams::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_seed", "v"), &LlamaCommonSamplerParams::set_seed);
	ClassDB::bind_method(D_METHOD("get_seed"), &LlamaCommonSamplerParams::get_seed);
	ClassDB::bind_method(D_METHOD("set_n_prev", "v"), &LlamaCommonSamplerParams::set_n_prev);
	ClassDB::bind_method(D_METHOD("get_n_prev"), &LlamaCommonSamplerParams::get_n_prev);
	ClassDB::bind_method(D_METHOD("set_n_probs", "v"), &LlamaCommonSamplerParams::set_n_probs);
	ClassDB::bind_method(D_METHOD("get_n_probs"), &LlamaCommonSamplerParams::get_n_probs);
	ClassDB::bind_method(D_METHOD("set_min_keep", "v"), &LlamaCommonSamplerParams::set_min_keep);
	ClassDB::bind_method(D_METHOD("get_min_keep"), &LlamaCommonSamplerParams::get_min_keep);
	ClassDB::bind_method(D_METHOD("set_top_k", "v"), &LlamaCommonSamplerParams::set_top_k);
	ClassDB::bind_method(D_METHOD("get_top_k"), &LlamaCommonSamplerParams::get_top_k);
	ClassDB::bind_method(D_METHOD("set_top_p", "v"), &LlamaCommonSamplerParams::set_top_p);
	ClassDB::bind_method(D_METHOD("get_top_p"), &LlamaCommonSamplerParams::get_top_p);
	ClassDB::bind_method(D_METHOD("set_min_p", "v"), &LlamaCommonSamplerParams::set_min_p);
	ClassDB::bind_method(D_METHOD("get_min_p"), &LlamaCommonSamplerParams::get_min_p);
	ClassDB::bind_method(D_METHOD("set_xtc_probability", "v"), &LlamaCommonSamplerParams::set_xtc_probability);
	ClassDB::bind_method(D_METHOD("get_xtc_probability"), &LlamaCommonSamplerParams::get_xtc_probability);
	ClassDB::bind_method(D_METHOD("set_xtc_threshold", "v"), &LlamaCommonSamplerParams::set_xtc_threshold);
	ClassDB::bind_method(D_METHOD("get_xtc_threshold"), &LlamaCommonSamplerParams::get_xtc_threshold);
	ClassDB::bind_method(D_METHOD("set_typ_p", "v"), &LlamaCommonSamplerParams::set_typ_p);
	ClassDB::bind_method(D_METHOD("get_typ_p"), &LlamaCommonSamplerParams::get_typ_p);
	ClassDB::bind_method(D_METHOD("set_temp", "v"), &LlamaCommonSamplerParams::set_temp);
	ClassDB::bind_method(D_METHOD("get_temp"), &LlamaCommonSamplerParams::get_temp);
	ClassDB::bind_method(D_METHOD("set_dynatemp_range", "v"), &LlamaCommonSamplerParams::set_dynatemp_range);
	ClassDB::bind_method(D_METHOD("get_dynatemp_range"), &LlamaCommonSamplerParams::get_dynatemp_range);
	ClassDB::bind_method(D_METHOD("set_dynatemp_exponent", "v"), &LlamaCommonSamplerParams::set_dynatemp_exponent);
	ClassDB::bind_method(D_METHOD("get_dynatemp_exponent"), &LlamaCommonSamplerParams::get_dynatemp_exponent);
	ClassDB::bind_method(D_METHOD("set_penalty_last_n", "v"), &LlamaCommonSamplerParams::set_penalty_last_n);
	ClassDB::bind_method(D_METHOD("get_penalty_last_n"), &LlamaCommonSamplerParams::get_penalty_last_n);
	ClassDB::bind_method(D_METHOD("set_penalty_repeat", "v"), &LlamaCommonSamplerParams::set_penalty_repeat);
	ClassDB::bind_method(D_METHOD("get_penalty_repeat"), &LlamaCommonSamplerParams::get_penalty_repeat);
	ClassDB::bind_method(D_METHOD("set_penalty_freq", "v"), &LlamaCommonSamplerParams::set_penalty_freq);
	ClassDB::bind_method(D_METHOD("get_penalty_freq"), &LlamaCommonSamplerParams::get_penalty_freq);
	ClassDB::bind_method(D_METHOD("set_penalty_present", "v"), &LlamaCommonSamplerParams::set_penalty_present);
	ClassDB::bind_method(D_METHOD("get_penalty_present"), &LlamaCommonSamplerParams::get_penalty_present);
	ClassDB::bind_method(D_METHOD("set_dry_multiplier", "v"), &LlamaCommonSamplerParams::set_dry_multiplier);
	ClassDB::bind_method(D_METHOD("get_dry_multiplier"), &LlamaCommonSamplerParams::get_dry_multiplier);
	ClassDB::bind_method(D_METHOD("set_dry_base", "v"), &LlamaCommonSamplerParams::set_dry_base);
	ClassDB::bind_method(D_METHOD("get_dry_base"), &LlamaCommonSamplerParams::get_dry_base);
	ClassDB::bind_method(D_METHOD("set_dry_allowed_length", "v"), &LlamaCommonSamplerParams::set_dry_allowed_length);
	ClassDB::bind_method(D_METHOD("get_dry_allowed_length"), &LlamaCommonSamplerParams::get_dry_allowed_length);
	ClassDB::bind_method(D_METHOD("set_dry_penalty_last_n", "v"), &LlamaCommonSamplerParams::set_dry_penalty_last_n);
	ClassDB::bind_method(D_METHOD("get_dry_penalty_last_n"), &LlamaCommonSamplerParams::get_dry_penalty_last_n);
	ClassDB::bind_method(D_METHOD("set_mirostat", "v"), &LlamaCommonSamplerParams::set_mirostat);
	ClassDB::bind_method(D_METHOD("get_mirostat"), &LlamaCommonSamplerParams::get_mirostat);
	ClassDB::bind_method(D_METHOD("set_top_n_sigma", "v"), &LlamaCommonSamplerParams::set_top_n_sigma);
	ClassDB::bind_method(D_METHOD("get_top_n_sigma"), &LlamaCommonSamplerParams::get_top_n_sigma);
	ClassDB::bind_method(D_METHOD("set_mirostat_tau", "v"), &LlamaCommonSamplerParams::set_mirostat_tau);
	ClassDB::bind_method(D_METHOD("get_mirostat_tau"), &LlamaCommonSamplerParams::get_mirostat_tau);
	ClassDB::bind_method(D_METHOD("set_mirostat_eta", "v"), &LlamaCommonSamplerParams::set_mirostat_eta);
	ClassDB::bind_method(D_METHOD("get_mirostat_eta"), &LlamaCommonSamplerParams::get_mirostat_eta);
	ClassDB::bind_method(D_METHOD("set_ignore_eos", "v"), &LlamaCommonSamplerParams::set_ignore_eos);
	ClassDB::bind_method(D_METHOD("get_ignore_eos"), &LlamaCommonSamplerParams::get_ignore_eos);
	ClassDB::bind_method(D_METHOD("set_no_perf", "v"), &LlamaCommonSamplerParams::set_no_perf);
	ClassDB::bind_method(D_METHOD("get_no_perf"), &LlamaCommonSamplerParams::get_no_perf);
	ClassDB::bind_method(D_METHOD("set_timing_per_token", "v"), &LlamaCommonSamplerParams::set_timing_per_token);
	ClassDB::bind_method(D_METHOD("get_timing_per_token"), &LlamaCommonSamplerParams::get_timing_per_token);

	ClassDB::bind_method(D_METHOD("set_dry_sequence_breakers", "arr"), &LlamaCommonSamplerParams::set_dry_sequence_breakers);
	ClassDB::bind_method(D_METHOD("get_dry_sequence_breakers"), &LlamaCommonSamplerParams::get_dry_sequence_breakers);
	ClassDB::bind_method(D_METHOD("set_samplers", "arr"), &LlamaCommonSamplerParams::set_samplers);
	ClassDB::bind_method(D_METHOD("get_samplers"), &LlamaCommonSamplerParams::get_samplers);
	ClassDB::bind_method(D_METHOD("set_grammar", "s"), &LlamaCommonSamplerParams::set_grammar);
	ClassDB::bind_method(D_METHOD("get_grammar"), &LlamaCommonSamplerParams::get_grammar);
	ClassDB::bind_method(D_METHOD("set_grammar_lazy", "v"), &LlamaCommonSamplerParams::set_grammar_lazy);
	ClassDB::bind_method(D_METHOD("get_grammar_lazy"), &LlamaCommonSamplerParams::get_grammar_lazy);
	ClassDB::bind_method(D_METHOD("set_grammar_triggers", "arr"), &LlamaCommonSamplerParams::set_grammar_triggers);
	ClassDB::bind_method(D_METHOD("get_grammar_triggers"), &LlamaCommonSamplerParams::get_grammar_triggers);
	ClassDB::bind_method(D_METHOD("set_preserved_tokens", "arr"), &LlamaCommonSamplerParams::set_preserved_tokens);
	ClassDB::bind_method(D_METHOD("get_preserved_tokens"), &LlamaCommonSamplerParams::get_preserved_tokens);
	ClassDB::bind_method(D_METHOD("set_logit_bias", "arr"), &LlamaCommonSamplerParams::set_logit_bias);
	ClassDB::bind_method(D_METHOD("get_logit_bias"), &LlamaCommonSamplerParams::get_logit_bias);
	ClassDB::bind_method(D_METHOD("set_logit_bias_eog", "arr"), &LlamaCommonSamplerParams::set_logit_bias_eog);
	ClassDB::bind_method(D_METHOD("get_logit_bias_eog"), &LlamaCommonSamplerParams::get_logit_bias_eog);
	ClassDB::bind_method(D_METHOD("print_to_string"), &LlamaCommonSamplerParams::print_to_string);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "n_prev"), "set_n_prev", "get_n_prev");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "n_probs"), "set_n_probs", "get_n_probs");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "min_keep"), "set_min_keep", "get_min_keep");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "top_k"), "set_top_k", "get_top_k");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "top_p"), "set_top_p", "get_top_p");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_p"), "set_min_p", "get_min_p");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "xtc_probability"), "set_xtc_probability", "get_xtc_probability");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "xtc_threshold"), "set_xtc_threshold", "get_xtc_threshold");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "typ_p"), "set_typ_p", "get_typ_p");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temp"), "set_temp", "get_temp");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dynatemp_range"), "set_dynatemp_range", "get_dynatemp_range");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dynatemp_exponent"), "set_dynatemp_exponent", "get_dynatemp_exponent");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "penalty_last_n"), "set_penalty_last_n", "get_penalty_last_n");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "penalty_repeat"), "set_penalty_repeat", "get_penalty_repeat");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "penalty_freq"), "set_penalty_freq", "get_penalty_freq");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "penalty_present"), "set_penalty_present", "get_penalty_present");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dry_multiplier"), "set_dry_multiplier", "get_dry_multiplier");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dry_base"), "set_dry_base", "get_dry_base");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "dry_allowed_length"), "set_dry_allowed_length", "get_dry_allowed_length");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "dry_penalty_last_n"), "set_dry_penalty_last_n", "get_dry_penalty_last_n");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mirostat"), "set_mirostat", "get_mirostat");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "top_n_sigma"), "set_top_n_sigma", "get_top_n_sigma");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mirostat_tau"), "set_mirostat_tau", "get_mirostat_tau");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mirostat_eta"), "set_mirostat_eta", "get_mirostat_eta");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ignore_eos"), "set_ignore_eos", "get_ignore_eos");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_perf"), "set_no_perf", "get_no_perf");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "timing_per_token"), "set_timing_per_token", "get_timing_per_token");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "dry_sequence_breakers"), "set_dry_sequence_breakers", "get_dry_sequence_breakers");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "samplers"), "set_samplers", "get_samplers");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar", PROPERTY_HINT_MULTILINE_TEXT), "set_grammar", "get_grammar");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "grammar_lazy"), "set_grammar_lazy", "get_grammar_lazy");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "grammar_triggers", PROPERTY_HINT_RESOURCE_TYPE, "LlamaCommonGrammarTrigger"), "set_grammar_triggers", "get_grammar_triggers");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "preserved_tokens"), "set_preserved_tokens", "get_preserved_tokens");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "logit_bias"), "set_logit_bias", "get_logit_bias");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "logit_bias_eog"), "set_logit_bias_eog", "get_logit_bias_eog");
}

void LlamaCommonSamplerParams::set_params(const common_params_sampling &p) { params = p; }
common_params_sampling LlamaCommonSamplerParams::get_params() const { return params; }

void LlamaCommonSamplerParams::set_seed(uint32_t v){ params.seed = v; }
uint32_t LlamaCommonSamplerParams::get_seed() const { return params.seed; }
void LlamaCommonSamplerParams::set_n_prev(int32_t v){ params.n_prev = v; }
int32_t LlamaCommonSamplerParams::get_n_prev() const { return params.n_prev; }
void LlamaCommonSamplerParams::set_n_probs(int32_t v){ params.n_probs = v; }
int32_t LlamaCommonSamplerParams::get_n_probs() const { return params.n_probs; }
void LlamaCommonSamplerParams::set_min_keep(int32_t v){ params.min_keep = v; }
int32_t LlamaCommonSamplerParams::get_min_keep() const { return params.min_keep; }
void LlamaCommonSamplerParams::set_top_k(int32_t v){ params.top_k = v; }
int32_t LlamaCommonSamplerParams::get_top_k() const { return params.top_k; }
void LlamaCommonSamplerParams::set_top_p(float v){ params.top_p = v; }
float LlamaCommonSamplerParams::get_top_p() const { return params.top_p; }
void LlamaCommonSamplerParams::set_min_p(float v){ params.min_p = v; }
float LlamaCommonSamplerParams::get_min_p() const { return params.min_p; }
void LlamaCommonSamplerParams::set_xtc_probability(float v){ params.xtc_probability = v; }
float LlamaCommonSamplerParams::get_xtc_probability() const { return params.xtc_probability; }
void LlamaCommonSamplerParams::set_xtc_threshold(float v){ params.xtc_threshold = v; }
float LlamaCommonSamplerParams::get_xtc_threshold() const { return params.xtc_threshold; }
void LlamaCommonSamplerParams::set_typ_p(float v){ params.typ_p = v; }
float LlamaCommonSamplerParams::get_typ_p() const { return params.typ_p; }
void LlamaCommonSamplerParams::set_temp(float v){ params.temp = v; }
float LlamaCommonSamplerParams::get_temp() const { return params.temp; }
void LlamaCommonSamplerParams::set_dynatemp_range(float v){ params.dynatemp_range = v; }
float LlamaCommonSamplerParams::get_dynatemp_range() const { return params.dynatemp_range; }
void LlamaCommonSamplerParams::set_dynatemp_exponent(float v){ params.dynatemp_exponent = v; }
float LlamaCommonSamplerParams::get_dynatemp_exponent() const { return params.dynatemp_exponent; }
void LlamaCommonSamplerParams::set_penalty_last_n(int32_t v){ params.penalty_last_n = v; }
int32_t LlamaCommonSamplerParams::get_penalty_last_n() const { return params.penalty_last_n; }
void LlamaCommonSamplerParams::set_penalty_repeat(float v){ params.penalty_repeat = v; }
float LlamaCommonSamplerParams::get_penalty_repeat() const { return params.penalty_repeat; }
void LlamaCommonSamplerParams::set_penalty_freq(float v){ params.penalty_freq = v; }
float LlamaCommonSamplerParams::get_penalty_freq() const { return params.penalty_freq; }
void LlamaCommonSamplerParams::set_penalty_present(float v){ params.penalty_present = v; }
float LlamaCommonSamplerParams::get_penalty_present() const { return params.penalty_present; }
void LlamaCommonSamplerParams::set_dry_multiplier(float v){ params.dry_multiplier = v; }
float LlamaCommonSamplerParams::get_dry_multiplier() const { return params.dry_multiplier; }
void LlamaCommonSamplerParams::set_dry_base(float v){ params.dry_base = v; }
float LlamaCommonSamplerParams::get_dry_base() const { return params.dry_base; }
void LlamaCommonSamplerParams::set_dry_allowed_length(int32_t v){ params.dry_allowed_length = v; }
int32_t LlamaCommonSamplerParams::get_dry_allowed_length() const { return params.dry_allowed_length; }
void LlamaCommonSamplerParams::set_dry_penalty_last_n(int32_t v){ params.dry_penalty_last_n = v; }
int32_t LlamaCommonSamplerParams::get_dry_penalty_last_n() const { return params.dry_penalty_last_n; }
void LlamaCommonSamplerParams::set_mirostat(int32_t v){ params.mirostat = v; }
int32_t LlamaCommonSamplerParams::get_mirostat() const { return params.mirostat; }
void LlamaCommonSamplerParams::set_top_n_sigma(float v){ params.top_n_sigma = v; }
float LlamaCommonSamplerParams::get_top_n_sigma() const { return params.top_n_sigma; }
void LlamaCommonSamplerParams::set_mirostat_tau(float v){ params.mirostat_tau = v; }
float LlamaCommonSamplerParams::get_mirostat_tau() const { return params.mirostat_tau; }
void LlamaCommonSamplerParams::set_mirostat_eta(float v){ params.mirostat_eta = v; }
float LlamaCommonSamplerParams::get_mirostat_eta() const { return params.mirostat_eta; }
void LlamaCommonSamplerParams::set_ignore_eos(bool v){ params.ignore_eos = v; }
bool LlamaCommonSamplerParams::get_ignore_eos() const { return params.ignore_eos; }
void LlamaCommonSamplerParams::set_no_perf(bool v){ params.no_perf = v; }
bool LlamaCommonSamplerParams::get_no_perf() const { return params.no_perf; }
void LlamaCommonSamplerParams::set_timing_per_token(bool v){ params.timing_per_token = v; }
bool LlamaCommonSamplerParams::get_timing_per_token() const { return params.timing_per_token; }

void LlamaCommonSamplerParams::set_dry_sequence_breakers(const PackedStringArray &arr){
	params.dry_sequence_breakers.clear();
	params.dry_sequence_breakers.reserve(arr.size());
	for (int i = 0; i < arr.size(); ++i) params.dry_sequence_breakers.push_back(String(arr[i]).utf8().get_data());
}
PackedStringArray LlamaCommonSamplerParams::get_dry_sequence_breakers() const{
	PackedStringArray out; for (auto &s : params.dry_sequence_breakers) out.push_back(String::utf8(s.c_str())); return out;
}

void LlamaCommonSamplerParams::set_samplers(const PackedInt32Array &arr){
	params.samplers.clear(); params.samplers.reserve(arr.size());
	for (int i = 0; i < arr.size(); ++i) params.samplers.push_back((common_sampler_type)arr[i]);
}
PackedInt32Array LlamaCommonSamplerParams::get_samplers() const{
	PackedInt32Array out; for (auto &t : params.samplers) out.push_back((int32_t)t); return out;
}

void LlamaCommonSamplerParams::set_grammar(const String &s){ params.grammar = s.utf8().get_data(); }
String LlamaCommonSamplerParams::get_grammar() const{ return String::utf8(params.grammar.c_str()); }

void LlamaCommonSamplerParams::set_grammar_lazy(bool v){ params.grammar_lazy = v; }
bool LlamaCommonSamplerParams::get_grammar_lazy() const{ return params.grammar_lazy; }

void LlamaCommonSamplerParams::set_grammar_triggers(const TypedArray<Ref<LlamaCommonGrammarTrigger>> &arr){
	params.grammar_triggers.clear(); params.grammar_triggers.reserve(arr.size());
	for (int i = 0; i < arr.size(); ++i) {
		Ref<LlamaCommonGrammarTrigger> gt = arr[i];
		if (gt.is_valid()) params.grammar_triggers.push_back(gt->get_grammar_trigger());
	}
}
TypedArray<Ref<LlamaCommonGrammarTrigger>> LlamaCommonSamplerParams::get_grammar_triggers() const{
	TypedArray<Ref<LlamaCommonGrammarTrigger>> out; for (auto &g : params.grammar_triggers) { Ref<LlamaCommonGrammarTrigger> r; r.instantiate(); r->set_grammar_trigger(g); out.push_back(r); } return out;
}

void LlamaCommonSamplerParams::set_preserved_tokens(const PackedInt32Array &arr){
	params.preserved_tokens.clear();
	for (int i = 0; i < arr.size(); ++i) params.preserved_tokens.insert((llama_token)arr[i]);
}
PackedInt32Array LlamaCommonSamplerParams::get_preserved_tokens() const{
	PackedInt32Array out; for (auto &t : params.preserved_tokens) out.push_back((int32_t)t); return out;
}

void LlamaCommonSamplerParams::set_logit_bias(const Array &arr){
	params.logit_bias.clear(); params.logit_bias.reserve(arr.size());
	for (int i = 0; i < arr.size(); ++i) params.logit_bias.push_back(_dict_to_bias(arr[i]));
}
Array LlamaCommonSamplerParams::get_logit_bias() const{
	Array out; for (auto &b : params.logit_bias) out.push_back(_bias_to_dict(b)); return out;
}

void LlamaCommonSamplerParams::set_logit_bias_eog(const Array &arr){
	params.logit_bias_eog.clear(); params.logit_bias_eog.reserve(arr.size());
	for (int i = 0; i < arr.size(); ++i) params.logit_bias_eog.push_back(_dict_to_bias(arr[i]));
}
Array LlamaCommonSamplerParams::get_logit_bias_eog() const{
	Array out; for (auto &b : params.logit_bias_eog) out.push_back(_bias_to_dict(b)); return out;
}

String LlamaCommonSamplerParams::print_to_string() const { return String::utf8(params.print().c_str()); }

LlamaCommonSamplerParams::LlamaCommonSamplerParams() {}
LlamaCommonSamplerParams::~LlamaCommonSamplerParams() {}