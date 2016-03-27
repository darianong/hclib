#ifndef HCLIB_CPP_H_
#define HCLIB_CPP_H_

#include "hclib_common.h"
#include "hclib-rt.h"
#include "hclib-async.h"
#include "hclib-forasync.h"
#include "hclib_promise.h"
#include "hclib.h"
#include "hclib-locality-graph.h"

namespace hclib {

typedef hclib_triggered_task_t triggered_task_t;
typedef hclib_locale_t locale_t;

template <typename T>
void launch(T lambda) {
    hclib_task_t *user_task = _allocate_async(lambda, false);
    hclib_launch((generic_frame_ptr)spawn, user_task);
}

promise_t **promise_create_n(size_t nb_promises, int null_terminated);

extern hclib_worker_state *current_ws();
int get_current_worker();
int num_workers();

int get_num_locales();
hclib_locale_t *get_closest_locale();
hclib_locale_t *get_all_locales();
hclib_locale_t **get_all_locales_of_type(int type, int *out_count);

hclib::future_t *allocate_at(size_t nbytes, hclib::locale_t *locale);
hclib::future_t *reallocate_at(void *ptr, size_t nbytes,
        hclib::locale_t *locale);
void free_at(void *ptr, hclib::locale_t *locale);
hclib::future_t *memset_at(void *ptr, int pattern, size_t nbytes,
        hclib::locale_t *locale);

template<typename... future_list_t>
inline hclib::future_t *async_copy_await(hclib::locale_t *dst_locale, void *dst,
        hclib::locale_t *src_locale, void *src, size_t nbytes,
        future_list_t... futures) {
    hclib_future_t **future_list = construct_future_list(futures...);
    return new hclib::future_t(hclib_async_copy(dst_locale, dst, src_locale,
                src, nbytes, future_list));
}

inline hclib::future_t *async_copy(hclib::locale_t *dst_locale, void *dst,
        hclib::locale_t *src_locale, void *src, size_t nbytes) {
    return new hclib::future_t(hclib_async_copy(dst_locale, dst, src_locale,
                src, nbytes, NULL));
}


}

#endif
