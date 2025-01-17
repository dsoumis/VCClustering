#ifndef SECOND_STEP_HASHPAIR_H
#define SECOND_STEP_HASHPAIR_H

#include <unordered_map>

using namespace std;

struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

#endif //SECOND_STEP_HASHPAIR_H
