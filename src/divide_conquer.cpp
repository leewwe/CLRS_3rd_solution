#include <vector>
#include <iostream>
#include <limits>
#include <utility>
#include <tuple>

using namespace std;


tuple<int, int, int> find_cross(vector<int>& ivec, int l, int r){
    int mid = (l + r) / 2;
    int left_max_pos = mid;
    int left_max_sum = INT_MIN;
    int sum = 0;
    for(auto i = mid; i >= 0; --i){
        sum += ivec[i];
        if(sum > left_max_sum){
            left_max_sum = sum;
            left_max_pos = i;
        }
    }

    int right_max_sum = INT_MIN;
    int right_max_pos = mid + 1;
    sum = 0;
    for(auto j = mid + 1; j <= r; ++j){
        sum += ivec[j];
        if(sum > right_max_sum){
            right_max_sum = sum;
            right_max_pos = j;
        }
    }
    return tuple<int, int, int>(left_max_pos, right_max_pos, left_max_sum + right_max_sum);
}

tuple<int, int, int> find_max_subarray(vector<int>& ivec, int l, int r) {
    if(l >= r){
        return tuple<int, int, int>(l, r, ivec[l]);
    }

    auto mid = (l + r) / 2;
    auto left = find_max_subarray(ivec, l, mid);
    auto right = find_max_subarray(ivec, mid + 1, r);
    auto cross = find_cross(ivec, l, r);
    auto left_sum = get<2>(left);
    auto right_sum = get<2>(right);
    auto cross_sum = get<2>(cross);
    if(left_sum >= right_sum && left_sum >= cross_sum){
        return left;
    }
    else if(right_sum >= left_sum && right_sum >= cross_sum){
        return right;
    }
    else {
        return cross;
    }
}

// tuple<int, int, int> find_max_subarray_linear(vector<int>& ivec, int l, int r){
//     int left = l;
//     int right = l;
//     int max_sum = ivec[left];
//     for(int i = left; i <= r; ++i){
//         int sum = 0;
//         for(auto j = i + 1; j >= left; --j){
//             sum += ivec[j];
//             if(sum > max_sum){
//                 max_sum = sum;
//                 left = j;
//                 right = i + 1;
//             }
//         }
//     }
//     return tuple<int, int, int>(left, right, max_sum);
// }

tuple<int, int, int> find_max_subarray_linear(vector<int>& ivec, int l, int r){
    int left = l;
    int right = l;
    int max_sum = ivec[0];

    int ending_here_low = 0;
    int ending_here_high = 0;
    int ending_here_sum = ivec[0];
    for(int j = l + 1; j <= r; ++j){
        ending_here_high = j;
        // 如果上次的和是正数，那么下一次的和还有可能是最大子序列最大子序列
        if(ending_here_sum > 0){
            ending_here_sum += ivec[j];
        }
        // 否则下一个数就作为新的最大子序列的开始，并作为新的和
        else{
            ending_here_low = j;
            ending_here_sum = ivec[j];
        }
        if(ending_here_sum > max_sum){
            // 存储最大的子数组的左右index和sum
            max_sum = ending_here_sum;
            left = ending_here_low;
            right = ending_here_high;
        }
    }

    return tuple<int, int, int>(left, right, max_sum);
}


// int main(int argc, char** argv){

//     // vector<int> ivec{13,-3,-25,20,3,-16,-23,18,20,-7,12,-5,-22,15,-4,7};
//     vector<int> ivec{-10,-25,-16,-23,-7,-5,-22,-4};
//     // vector<int> ivec{13,20,3,18,20,12,15,7};
//     // auto res = find_max_subarray(ivec, 0, ivec.size() - 1);
//     auto res = find_max_subarray_linear(ivec, 0, ivec.size() - 1);
//     cout << ivec[std::get<0>(res)] << endl;
//     cout << ivec[std::get<1>(res)] << endl;
//     cout << std::get<2>(res) << endl;
//     return 0;
// }