#include <iostream>
#include <unordered_set>

#define EXTRAYEARS 5

struct Triplet {
	unsigned int a, b, c;
};

struct SumProduct {
	unsigned int sum, product;

	// Equality function for storage in unordered_set
	bool operator==(const SumProduct &other) const {
		return (other.sum == sum && other.product == product);
	}
};

// Hash function for storage in unordered_set
namespace std {
	template <>
	struct hash<SumProduct> {
		size_t operator()(const SumProduct& s) const {
			return ((hash<int>()(s.sum) ^ (hash<int>()(s.product) << 1)) >> 1);
		}
	};
}

Triplet solve() {
	std::unordered_set<SumProduct> visited; // SumProduct's we've visited once
	std::unordered_set<SumProduct> obscure; // SumProduct's we've visited twice, so an obscure SumProduct

	//TODO: prune items in these unordered sets we are sure we won't need anymore

	SumProduct s;
	unsigned int na, nb, nc;
	for (unsigned int a = 0; ; a++) {
		for (unsigned int b = 0; b <= a; b++) {
			for (unsigned int c = 0; c <= b; c++) {
				//Iterate over every combination of a, b and c
				s = SumProduct{a+b+c, a*b*c};	
				auto found = visited.find(s);
				if (found != visited.end()) {
					// The SumProduct was already visited and now we've found another
					// triplet that leads to this SumProduct, so it's an obscure number
					obscure.insert(s);

					// Time to find obscures above and below
					na = a;
					nb = b;
					nc = c;
					int acc = 0; // amount of obscure numbers above and below
					// first we'll try above
					while (true) {
						na++;
						nb++;
						nc++;
						s = SumProduct{na+nb+nc, na*nb*nc};
						found = obscure.find(s);
						if (found == obscure.end()) {
							// Not an obscure number anymore
							break;
						}
						acc++;
					}
					na = a;
					nb = b;
					nc = c;
					// Now we'll try below
					while (true) {
						if (na == 0 || nb == 0 || nc == 0) break;
						na--;
						nb--;
						nc--;
						s = SumProduct{na+nb+nc, na*nb*nc};
						found = obscure.find(s);
						if (found == obscure.end()) {
							// Not an obscure number anymore
							break;
						}
						acc++;
					}
					// If there are enough obscure numbers in sequence, we return the lowest one
					// the triplet (na, nb, nc) failed at the search for obscure numbers we did
					// before, so that means that (na+1, nb+1, nc+1) is the first valid
					// obscure number
					if (acc >= EXTRAYEARS) return Triplet{na+1, nb+1, nc+1};
				} else {
					// not in visited, so the first time we find this obscure number
					visited.insert(s);
				}
			}
		}
	}
}

// The function solve only stores the SumProduct in unordered maps, 
// not the triplets that lead to this SumProduct. To find two Triplets
// that lead to a specific SumProduct, you can use this function.
std::pair<Triplet, Triplet> getObscurePair(SumProduct s) {
	int i = 0;
	std::pair<Triplet, Triplet> ans;
	unsigned int c;
	for (unsigned int a = 0; ; a++) {
		for (unsigned int b = 0; b <= a; b++) {
			if (a + b > s.sum) continue;
			c = s.sum - a - b;
			if (s.product == a*b*c) {
				if (i == 0) {
					ans.first = Triplet{a, b, c};
					i++;
				} else {
					ans.second = Triplet{a, b, c};
					return ans;
				}
			}
		}
	}
	return ans;
}

int main() {
	Triplet sol = solve();
	for (int i = 0; i < EXTRAYEARS + 1; i++) {
		std::cout << "Triplet{" << sol.a << ", " << sol.b << ", " << sol.c <<"}" <<  std::endl;
		std::cout << "The sum and the product can be made with:" << std::endl;
		auto ans = getObscurePair(SumProduct{sol.a+sol.b+sol.c, sol.a*sol.b*sol.c});
		std::cout << ans.first.a << " " << ans.first.b << " " << ans.first.c << std::endl;
		std::cout << ans.second.a << " " << ans.second.b << " " << ans.second.c << std::endl;
		sol.a++;
		sol.b++;
		sol.c++;
	}
	
	return 0;
}




