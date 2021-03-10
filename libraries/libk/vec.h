#ifndef VEC_H
#define VEC_H

#define create_vec_struct(type) \
	struct {		\
		type *data;	\
		size_t size;	\
		size_t elecnt;	\
		size_t cur;	\
	}



#endif
