#pragma once

#include <algorithm>
#include <thread>
#include <mutex>
#include <vector>

namespace cls_06 {

    template<class It, class Fun>
    void parallel_for(size_t num_threads, It first, It last, Fun fun) {


        size_t distance = std::distance(first, last);
        num_threads = std::min(distance, num_threads);
        if (num_threads == 0) {
            return;
        }

        std::vector<std::thread> threads;
        threads.reserve(num_threads - 1);

        std::exception_ptr eptr;
        std::atomic_bool ex_thrown = false;
        auto thread_fun = [&](It begin) {
            try {
                if (ex_thrown) {
                    return;
                }
                size_t step = 0;
                for (It it = begin; it != last; ++it) {
                    if (step % num_threads == 0) {
                        fun(*it);
                    }
                    step++;
                }
            } catch (...) {
                eptr = std::current_exception();
                ex_thrown = true;
            }

        };
        auto cur_begin = first;
        for (size_t i = 1; i < num_threads; ++i) {
            threads.emplace_back(thread_fun, cur_begin);
            cur_begin++;
        }

        thread_fun(cur_begin);

        for (auto& thread : threads) {
            thread.join();
        }

        if (eptr) {
            std::rethrow_exception(eptr);
        }
    }
}
