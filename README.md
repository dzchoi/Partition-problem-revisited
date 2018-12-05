# Partition problem revisited

**Partition problem**, as the [wiki page](https://en.wikipedia.org/wiki/Partition_problem) says, is the task of deciding whether a given multiset S of positive integers can be partitioned into two subsets S1 and S2 such that the sum of the numbers in S1 equals the sum of the numbers in S2. As the wiki page says, there is a [dynamic programming](https://en.wikipedia.org/wiki/Dynamic_programming) solution that involves creating a two-dimensional boolean table *P*.

Instead of *P*, however, we can define **one-dimensional** index table *I* by:

> When *S* = { *x<sub>0</sub>*, *x<sub>1</sub>*, ..., *x<sub>N-1</sub>* },  
> *I[s]* returns the first index *i* such that the sum of the numbers in a subset of { *x<sub>0</sub>*, *x<sub>1</sub>*, ..., *x<sub>i</sub>* } equals *s*, or returns *N* if no such *i* exists.

For example, if *S* = [ 1, 3, 1, 1, 2, 2 ], we will have *I* = [ 6, 0, 2, 1, 1, 2, 3, 4, 4, 5, 5 ].  
Because:  
- *I[1]* = 0  *(subset { 1 } of { x<sub>0</sub>=1 } can sum to 1.)*,
- *I[2]* = 2  *(subset { 1, 1 } of { x<sub>0</sub>=1, x<sub>1</sub>=3, x<sub>2</sub>=1 } can sum to 2.)*,  
- *I[3]* = 1  *(subset { 3 } of { x<sub>0</sub>=1, x<sub>1</sub>=3 } can sum to 3.)*,
- *I[4]* = 1  *(subset { 1, 3 } of { x<sub>0</sub>=1, x<sub>1</sub>=3 } can sum to 3.)*,
- ...

### Implementation in C++

The table *I* can be created as simply as *P* can be. It will take less time mostly, although the worst case is still the same, *O(N\*sum)*, where *N* is the cardinality(size) of *S* and *sum* is the last index of the table *I*. We can generate *I* as much as specified by *sum*.

```C++
typedef int index_t;

std::vector<index_t> make_I(const std::vector<unsigned>& S, const unsigned sum)
{
    const index_t n = S.size();

    std::vector<index_t> I;
    I.reserve(sum+1);
    I.push_back(n);  // I[0] = n as a space filler

    for ( index_t i = 1 ; i <= sum ; ++i ) {
        I.push_back(n);
        for ( index_t j = 0 ; j < n ; ++j )
            if ( i == S[j] || (i > S[j] && I[i-S[j]] < j) ) {
                I.back() = j;
                break;
            }
    }

    return I;
}
```

### Applications

The same function *find_partition()* in the wiki page can be defined.

```C++
bool find_partition(const std::vector<unsigned>& S)
{
    const unsigned K = std::accumulate(S.begin(), S.end(), 0);
    return make_I(S, K/2)[K/2] < S.size();
}
```

We can find the actual subset *S1* that can make such a sum. (There may be more than one possible subset. In that case, this function finds one that consists of as earlier elements of *S* as possible.)

``` C++
std::vector<unsigned> subset(const std::vector<unsigned>& S, unsigned sum)
{
    // If S is supposed the same over calls, we can build I once for the biggest sum and 
    // use it again.
    std::vector<index_t> I = make_I(S, sum);
    std::vector<unsigned> S1;

    for ( index_t i ; (i = I[sum]) < S.size() ; sum -= S[i] )
        S1.push_back(S[i]);

    return S1;
}
```

So,

```C++
int main()
{
    for ( auto each: subset({ 1, 3, 1, 1, 2, 2}, 7) )
        std::cout << each << ' ';
    // will produce: 2 1 3 1
}
```
