#pragma once

#include "llama.h"
#include "../llama_sampler.h"

struct llama_sampler * llama_sampler_init_greedy();
struct llama_sampler * llama_sampler_init_dist(uint32_t seed);
struct llama_sampler * llama_sampler_init_top_k(int32_t k);
struct llama_sampler * llama_sampler_init_top_p(float p, size_t min_keep);
struct llama_sampler * llama_sampler_init_min_p(float p, size_t min_keep);
struct llama_sampler * llama_sampler_init_typical(float p, size_t min_keep);
struct llama_sampler * llama_sampler_init_temp(float temp);
struct llama_sampler * llama_sampler_init_temp_ext(float temp, float delta, float exponent);
struct llama_sampler * llama_sampler_init_xtc(float p, float t, size_t min_keep, uint32_t seed);
struct llama_sampler * llama_sampler_init_mirostat(int32_t n_vocab, uint32_t seed, float tau, float eta, int32_t m);
struct llama_sampler * llama_sampler_init_mirostat_v2(uint32_t seed, float tau, float eta);
struct llama_sampler * llama_sampler_init_grammar(const struct llama_vocab * vocab, const char * grammar_str, const char * grammar_root);
struct llama_sampler * llama_sampler_init_penalties(int32_t penalty_last_n, float penalty_repeat, float penalty_freq, float penalty_present);
struct llama_sampler * llama_sampler_init_top_n_sigma(float n);
struct llama_sampler * llama_sampler_init_dry(const struct llama_vocab * vocab, int32_t n_ctx_train, float dry_multiplier, float dry_base, int32_t dry_allowed_length, int32_t dry_penalty_last_n, const char ** seq_breakers, size_t num_breakers);
struct llama_sampler * llama_sampler_init_logit_bias(int32_t n_vocab, int32_t n_logit_bias, const llama_logit_bias * logit_bias);
struct llama_sampler * llama_sampler_init_infill(const struct llama_vocab * vocab);
