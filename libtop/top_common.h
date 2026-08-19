/******************************************************************************

			   Copyright (c) 2013 - 2015
			Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#ifndef __top_common_h
#define __top_common_h

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0]))
#endif

#ifndef offsetof
#define offsetof(STRUCT, MEMBER) \
   /*lint -save -e(413) -e(507) -e(831) */ \
   ((size_t) &((STRUCT *) 0)->MEMBER ) \
				/*lint -restore */
#endif

/** Proc counters */
#if defined(LINUX) || defined(ECOS)
#define ONU_CNT_PROC(key1, key2, name, proc_entry) \
{ key1, key2, name, top_proc_line_get, onu_top_proc_get, NULL, NULL, proc_entry },
#define OPTIC_CNT_PROC(key1, key2, name, proc_entry) \
{ key1, key2, name, top_proc_line_get, optic_top_proc_get, NULL, NULL, proc_entry },
#else
#define CNT_PROC(key1, key2, name, proc_entry)
#endif

/** Regular counters */
#define CNT(key1, key2, name, line_get, page_get, on_enter, on_leave) \
{ key1, key2, name, line_get, page_get, on_enter, on_leave, NULL },

struct top_context;

/** Read file contents into shared buffer from procfs.

   \param[in] ctx   context
   \param[in] name  File name

   \return Number of lines in file
*/
int linux_file_read(struct top_context *ctx, const char *name);

/** Read file contents into shared buffer from emulated procfs.

   \param[in] ctx   context
   \param[in] name  File name

   \return Number of lines in file
*/
int ecos_file_read(struct top_context *ctx, const char *name, const bool onu);

/** read file from procfs

   \return Number of lines in the file
*/
int onu_top_proc_get(struct top_context *ctx, const char *file_name);

/** read file from procfs

   \return Number of lines in the file
*/
int optic_top_proc_get(struct top_context *ctx, const char *file_name);

/** Retrieve pointer to specified line

   \param[in]  line  Table entry number; -1 for header
   \param[out] text  Table entry text
*/
char *top_proc_line_get(struct top_context *ctx, const int line, char *text);

int help_get(struct top_context *ctx, const char *dummy);

void help_entry_render(struct top_context *ctx,
		       char *buff, int group1, int group2);

#endif
