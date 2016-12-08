#ifndef _LINUX_SNAPPY_H
#define _LINUX_SNAPPY_H 1

#include <stdbool.h>
#include <stddef.h>

/* Only needed for compression. This preallocates the worst case */
struct RDsnappy_env {
	unsigned short *hash_table;
	void *scratch;
	void *scratch_output;
};

struct iovec;
int RDsnappy_init_env(struct RDsnappy_env *env);
int RDsnappy_init_env_sg(struct RDsnappy_env *env, bool sg);
void RDsnappy_free_env(struct RDsnappy_env *env);
int RDsnappy_uncompress_iov(struct iovec *iov_in, int iov_in_len,
			   size_t input_len, char *uncompressed);
int RDsnappy_uncompress(const char *compressed, size_t n, char *uncompressed);
int RDsnappy_compress(struct RDsnappy_env *env,
		    const char *input,
		    size_t input_length,
		    char *compressed,
		    size_t *compressed_length);
int RDsnappy_compress_iov(struct RDsnappy_env *env,
			struct iovec *iov_in,
			int iov_in_len,
			size_t input_length,
			struct iovec *iov_out,
			int *iov_out_len,
			size_t *compressed_length);
bool RDsnappy_uncompressed_length(const char *buf, size_t len, size_t *result);
size_t RDsnappy_max_compressed_length(size_t source_len);




#endif
