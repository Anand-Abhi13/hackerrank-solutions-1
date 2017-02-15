// Not yet working.
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

struct action {
    bool is_r;
    size_t start, end;
};

static bool swappable(const std::vector<size_t>& elements, size_t i, size_t j);
static void do_swap_check(const std::vector<size_t>& elements, std::vector<action>& actions, size_t n, const std::vector<size_t>& sorted_elements);

static void do_reverse_check(const std::vector<size_t>& elements, std::vector<action>& actions, size_t n, const std::vector<size_t>& sorted_elements);

int main(void) {

    size_t n;
    std::cin >> n;

    std::vector<size_t> elements(n);

	bool sorted = true; size_t last = 0;
    for (auto& _e : elements) {
        std::cin >> _e;
		sorted = sorted && _e >= last;
		last = _e;
	}

	if (sorted) {
		std::cout << "yes\n";
		return 0;
	}

    std::vector<action> actions;

	std::vector<size_t> sorted_elements(elements);
	std::sort(sorted_elements.begin(),sorted_elements.end());

	do_reverse_check(elements, actions, n, sorted_elements);
	if (actions.empty())
		do_swap_check(elements, actions, n, sorted_elements);

    if (actions.size() == 1) {

		std::cout << "yes\n";

        if (actions.size() == 1) {
            action a = actions.front();
            std::cout << (a.is_r ? "reverse" : "swap") << " " << a.start+1 << " " << a.end+1 << '\n';
        }
    } else {
        std::cout << "no\n";
    }

    return 0;
}

static bool swappable(const std::vector<size_t>& elements, size_t i, size_t j) {
	if (i > j)
		std::swap(i,j);
	if (i >= elements.size() || j >= elements.size())
		return false;

	bool res = i < elements.size()-1 && elements[i] > elements[i+1]
	           && (j > 0 && elements[j] < elements[j-1]);

	if (!res) return res;

	res = res && elements[i] > elements[j]
	          && (j == elements.size()-1 || elements[i] < elements[j+1])
			  && elements[i] >= elements[j-1]
	          && (i == 0 || elements[j] > elements[i-1])
			  && elements[j] <= elements[i+1];

	return res;
}

static void do_swap_check(const std::vector<size_t>& elements, std::vector<action>& actions, size_t n, const std::vector<size_t>& sorted_elements) {

	for ( size_t i = 0,j, m = n-1 ; i < m ; ++i ) {
		if (elements[i] != sorted_elements[i]) {
			for ( j = i+1; j < n ; ++j)
				if (swappable(elements,i,j))
					actions.push_back({false,i,j});
			// for (j = i+1; j < n && actions.size() <= 2; ++j)
			// 	if (elements[i] == sorted_elements[j])
			// 		actions.push_back({false,i,j});
		}
	}
}

static bool find_in_bet(const std::vector<size_t>& sorted_elements, size_t min, size_t max);

static void do_reverse_check(const std::vector<size_t>& elements, std::vector<action>& actions, size_t n, const std::vector<size_t>& sorted_elements) {
	static auto get_end_of_decline = [&elements, &n](size_t i){
		for (; i < n-1 && elements[i] > elements[i+1]; ++i);
		return i;
	};

	for (size_t i = 0, j; i < n && actions.empty(); ++i) {
		j = get_end_of_decline(i);
		if (j > i+1) {
			actions.push_back({true, i, j});
			// std::cout << "Can reverse from " << i+1 << " to " << j+1 << "\n\t";
			// while (i <= j) {
			// 	std::cout << elements[i++] << ' ';
			// 	if ((j-i) % 9 == 0)
			// 		std::cout << "\n\t";
			// }
			// std::cout << '\n';

			if (i > 0 && find_in_bet(sorted_elements, elements[j], elements[i-1]))
				actions.push_back({false, j, i-1});

			if (j < elements.size()-1 && find_in_bet(sorted_elements, elements[i], elements[j+1]))
				actions.push_back({false, i, j+1});

			i = j;
		}
	}
}

static bool find_in_bet(const std::vector<size_t>& sorted_elements, size_t min, size_t max) {
	size_t start = 0, end = sorted_elements.size(), mid;
	while (start < end) {
		mid = (end+start)/2;
		if (sorted_elements[mid] <= min) {
			start = mid + 1;
		} else if (sorted_elements[mid] < max) {
			return true;
		} else {
			end = mid;
		}
	}
	return false;
}
