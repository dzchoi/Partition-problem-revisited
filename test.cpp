#include <vector>

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



#include <iostream>

template <class T>
static void show_T(const T& ts, const char open ='[', const char close=']')
{
    typename T::const_iterator it = ts.begin();
    std::cout << open;
    if ( it != ts.end() )
	std::cout << *it++;

    while ( it != ts.end() )
	std::cout << ' ' << *it++;
    std::cout << close;
}



#include <numeric>  // std::accumulate()

int main(int argc, char* argv[])
{
    std::vector<unsigned> S;
    S.reserve(argc-1);

    for ( int i = 1 ; i < argc ; ++i )
	S.push_back(std::atoi(argv[i]));

    const unsigned K = std::accumulate(S.begin(), S.end(), 0);
    for ( unsigned sum = 0 ; sum <= K ; ++sum ) {
	std::cout << sum << ": ";
	show_T(subset(S, sum));
	std::cout << '\n';
    }
}
