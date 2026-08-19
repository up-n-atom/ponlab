/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef IFXMIPS_VECTORING_STUB_H
#define IFXMIPS_VECTORING_STUB_H

extern int32_t (*mei_dsm_cb_func_hook)(uint32_t *p_error_vector);

extern void (*ltq_vectoring_priority_hook)(uint32_t priority);

#endif /* IFXMIPS_VECTORING_STUB_H */

