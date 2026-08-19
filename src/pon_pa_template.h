#ifndef pon_pa_template_h
#define pon_pa_template_h

/* Structure to hold the PON PA template context */
struct pon_pa_template_context {
	/* Handle to the high-level interface */
	void *hl_handle;

	/* Pointer to the configuration API */
	const struct pa_config *pa_config;
};

#endif /* pon_pa_template_h */
