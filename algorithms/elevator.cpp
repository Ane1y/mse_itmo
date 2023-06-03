#include <bits/stdc++.h>


std::vector<int64_t> people;
std::vector<int64_t> sum;

bool search(int64_t num, size_t n, size_t capacity, size_t p, size_t t) {
   int64_t i = 0;
   int64_t j = 0;
   for (i = 1; i <= n; i++) {
       if (sum[i] >= num) {
           break;
       }
   }
   int64_t level = i;
   int64_t peoples = num - sum[i - 1];
   int64_t all = 0;
   for (i = level; i >= 1;) {
       if (peoples % capacity == 0) {
           all += (peoples / capacity * 2 * p * i);
       } else {
           all += ((peoples / capacity + 1) * 2 * p * i);
       }
       if (all > t) {
           return false;
       }
       int64_t tmp_t = peoples % capacity;
       if (tmp_t == 0) {
           i = i - 1;
           peoples = people[i];
           continue;
       }
       for (j = i - 1; j >= 1; --j) {
           if (people[j] + tmp_t >= capacity) {
               break;
           } else {
               tmp_t += people[j];
           }
       }
       peoples = people[j] - (capacity - tmp_t);
       i = j;
   }
   return all <= t;
}
int64_t bin_search(size_t n, size_t c, size_t p, size_t t) {
   int64_t left = 0;
   int64_t right = sum[n];
   int64_t answer = 0;
   while (left <= right) {
       int64_t middle = (left + right) / 2;
       if (search(middle, n, c, p, t)) {
           answer = middle;
           left = middle + 1;
       } else
           right = middle - 1;
   }
}

int main() {
   size_t n = 0;
   size_t capacity = 0;
   size_t p = 0;
   size_t total_time = 0;

   std::cin >> n >> capacity >> p >> total_time;

   people.resize(n + 1);
   sum.resize(n + 1);

   people[0] = 0;
   sum[0] = 0;
   for (size_t i = 1; i <= n; ++i) {
       std::cin >> people[i];
   }

   return 0;
}
