typedef unsigned int size_t;

static inline void* memset(void *str, int c, size_t n) {
	uint8_t* data = (uint8_t*) str;
	while (n-- > 0) *(data) = (uint8_t) c;
	return str;
}
