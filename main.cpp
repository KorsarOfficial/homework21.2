#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <optional>
#include <ranges>

// merge K sorted arrays: O(N log K), N = total elements
// min-heap entry: (value, array_index, element_index)
[[nodiscard]] auto merge_k_sorted(std::span<const std::vector<int>> arrays)
    -> std::vector<int> {
    struct Entry {
        int val, arr, idx;
        [[nodiscard]] constexpr bool operator>(const Entry& o) const { return val > o.val; }
    };
    // T_push = O(log K), T_pop = O(log K)
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    int total = 0;
    for (int i = 0; i < static_cast<int>(arrays.size()); ++i) {
        if (!arrays[i].empty()) {
            pq.push({arrays[i][0], i, 0});
            total += static_cast<int>(arrays[i].size());
        }
    }
    std::vector<int> result;
    result.reserve(total);
    while (!pq.empty()) {
        const auto [val, arr, idx] = pq.top();
        pq.pop();
        result.push_back(val);
        if (idx + 1 < static_cast<int>(arrays[arr].size()))
            pq.push({arrays[arr][idx + 1], arr, idx + 1});
    }
    return result;
}

// inv(A) = |{(i,j) : i<j ^ A[i]>A[j]}|, O(n log n)
// inv_cross = Sigma_{j=mid+1}^{hi} |{i in [lo,mid] : a[i] > a[j]}|
auto merge_count(std::span<int> a, std::span<int> tmp, int lo, int hi) -> long long {
    if (lo >= hi) return 0;
    const int mid = lo + (hi - lo) / 2;
    long long cnt = 0;
    cnt += merge_count(a, tmp, lo, mid);
    cnt += merge_count(a, tmp, mid + 1, hi);
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            // a[i..mid] > a[j]
            cnt += (mid - i + 1);
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi) tmp[k++] = a[j++];
    for (int x = lo; x <= hi; ++x) a[x] = tmp[x];
    return cnt;
}

[[nodiscard]] auto inversion_count(std::vector<int> a) -> long long {
    const int n = static_cast<int>(a.size());
    std::vector<int> tmp(n);
    return merge_count(a, tmp, 0, n - 1);
}

// dp[i] = min tail of increasing subsequence of length i+1, O(n log n)
[[nodiscard]] auto lis_length(std::span<const int> a) -> int {
    std::vector<int> tails;
    for (const int x : a) {
        const auto it = std::ranges::lower_bound(tails, x);
        if (it == tails.end())
            tails.push_back(x);
        else
            *it = x;
    }
    return static_cast<int>(tails.size());
}

// LIS with reconstruction: O(n log n)
[[nodiscard]] auto lis_sequence(std::span<const int> a) -> std::vector<int> {
    const int n = static_cast<int>(a.size());
    if (n == 0) return {};
    std::vector<int> tails, tails_idx, prev_idx(n, -1), pos(n);
    for (int i = 0; i < n; ++i) {
        const auto it = std::ranges::lower_bound(tails, a[i]);
        const int p = static_cast<int>(it - tails.begin());
        if (it == tails.end()) {
            tails.push_back(a[i]);
            tails_idx.push_back(i);
        } else {
            *it = a[i];
            tails_idx[p] = i;
        }
        pos[i] = p;
        if (p > 0) prev_idx[i] = tails_idx[p - 1];
    }
    const int len = static_cast<int>(tails.size());
    std::vector<int> result(len);
    int k = tails_idx[len - 1];
    for (int i = len - 1; i >= 0; --i) {
        result[i] = a[k];
        k = prev_idx[k];
    }
    return result;
}

// find largest i : a[i] < a[i+1], then largest j : a[j] > a[i], swap, reverse [i+1..]
// O(n)
auto next_perm(std::span<int> a) -> bool {
    const int n = static_cast<int>(a.size());
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    std::swap(a[i], a[j]);
    std::ranges::reverse(a.subspan(i + 1));
    return true;
}

void print_vec(std::string_view label, std::span<const int> v) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]\n";
}

int main() {
    // K=4, O(N log K)
    std::cout << "=== Merge K Sorted Arrays ===\n";
    const std::vector<std::vector<int>> sorted_arrays = {
        {1, 5, 9, 21},
        {2, 3, 7, 12, 18},
        {4, 8, 11, 15},
        {6, 10, 13, 14, 16, 17, 19, 20}
    };
    for (int i = 0; i < static_cast<int>(sorted_arrays.size()); ++i)
        print_vec("  arr[" + std::to_string(i) + "]", sorted_arrays[i]);
    auto merged = merge_k_sorted(sorted_arrays);
    print_vec("  merged", merged);
    std::cout << "\n";

    // inv(A) = |{(i,j) : i<j ^ A[i]>A[j]}|
    std::cout << "=== Inversion Count ===\n";
    const std::vector<int> arr1 = {8, 4, 2, 1};
    // inv = C(4,2) = 6
    const long long inv1 = inversion_count(arr1);
    print_vec("  A", arr1);
    std::cout << "  inv(A) = " << inv1 << "\n";

    const std::vector<int> arr2 = {3, 1, 2, 5, 4, 7, 6};
    const long long inv2 = inversion_count(arr2);
    print_vec("  B", arr2);
    std::cout << "  inv(B) = " << inv2 << "\n\n";

    // dp[i] = min tail of IS of length i+1, O(n log n)
    std::cout << "=== Longest Increasing Subsequence ===\n";
    const std::vector<int> lis_arr = {10, 9, 2, 5, 3, 7, 101, 18, 4, 6, 8, 12};
    print_vec("  A", lis_arr);
    std::cout << "  LIS length = " << lis_length(lis_arr) << "\n";
    auto lis_seq = lis_sequence(lis_arr);
    print_vec("  LIS", lis_seq);
    std::cout << "\n";

    // O(n)
    std::cout << "=== Next Permutation ===\n";
    std::vector<int> perm = {1, 2, 3, 4};
    std::cout << "  First 10 permutations of {1,2,3,4}:\n";
    for (int i = 0; i < 10; ++i) {
        print_vec("    " + std::to_string(i + 1), perm);
        next_perm(perm);
    }
    std::cout << "\n";

    std::vector<int> last_perm = {4, 3, 2, 1};
    print_vec("  last_perm", last_perm);
    const bool has_next = next_perm(last_perm);
    std::cout << "  has_next = " << (has_next ? "true" : "false") << "\n";

    return 0;
}
