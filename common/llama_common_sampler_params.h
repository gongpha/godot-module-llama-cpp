#pragma once

#include "core/io/resource.h"
#include "core/variant/typed_array.h"
#include "core/string/ustring.h"

#include "llama_common.h"
#include "common.h"

class LlamaCommonSamplerParams : public Resource {
	GDCLASS(LlamaCommonSamplerParams, Resource);

	common_params_sampling params;

protected:
	static void _bind_methods();

public:
	void set_params(const common_params_sampling &p);
	common_params_sampling get_params() const;

	void set_seed(uint32_t v);               uint32_t get_seed() const;
	void set_n_prev(int32_t v);              int32_t get_n_prev() const;
	void set_n_probs(int32_t v);             int32_t get_n_probs() const;
	void set_min_keep(int32_t v);            int32_t get_min_keep() const;
	void set_top_k(int32_t v);               int32_t get_top_k() const;
	void set_top_p(float v);                 float get_top_p() const;
	void set_min_p(float v);                 float get_min_p() const;
	void set_xtc_probability(float v);       float get_xtc_probability() const;
	void set_xtc_threshold(float v);         float get_xtc_threshold() const;
	void set_typ_p(float v);                 float get_typ_p() const;
	void set_temp(float v);                  float get_temp() const;
	void set_dynatemp_range(float v);        float get_dynatemp_range() const;
	void set_dynatemp_exponent(float v);     float get_dynatemp_exponent() const;
	void set_penalty_last_n(int32_t v);      int32_t get_penalty_last_n() const;
	void set_penalty_repeat(float v);        float get_penalty_repeat() const;
	void set_penalty_freq(float v);          float get_penalty_freq() const;
	void set_penalty_present(float v);       float get_penalty_present() const;
	void set_dry_multiplier(float v);        float get_dry_multiplier() const;
	void set_dry_base(float v);              float get_dry_base() const;
	void set_dry_allowed_length(int32_t v);  int32_t get_dry_allowed_length() const;
	void set_dry_penalty_last_n(int32_t v);  int32_t get_dry_penalty_last_n() const;
	void set_mirostat(int32_t v);            int32_t get_mirostat() const;
	void set_top_n_sigma(float v);           float get_top_n_sigma() const;
	void set_mirostat_tau(float v);          float get_mirostat_tau() const;
	void set_mirostat_eta(float v);          float get_mirostat_eta() const;
	void set_ignore_eos(bool v);             bool get_ignore_eos() const;
	void set_no_perf(bool v);                bool get_no_perf() const;
	void set_timing_per_token(bool v);       bool get_timing_per_token() const;

	void set_dry_sequence_breakers(const PackedStringArray &arr);
	PackedStringArray get_dry_sequence_breakers() const;

	void set_samplers(const PackedInt32Array &arr);
	PackedInt32Array get_samplers() const;

	void set_grammar(const String &s);
	String get_grammar() const;

	void set_grammar_lazy(bool v);
	bool get_grammar_lazy() const;

	void set_grammar_triggers(const TypedArray<Ref<LlamaCommonGrammarTrigger>> &arr);
	TypedArray<Ref<LlamaCommonGrammarTrigger>> get_grammar_triggers() const;

	void set_preserved_tokens(const PackedInt32Array &arr);
	PackedInt32Array get_preserved_tokens() const;

	void set_logit_bias(const Array &arr);
	Array get_logit_bias() const;

	void set_logit_bias_eog(const Array &arr);
	Array get_logit_bias_eog() const;

	String print_to_string() const;

	LlamaCommonSamplerParams();
	~LlamaCommonSamplerParams();
};
