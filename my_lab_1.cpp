#include <iostream>
#include <set>
#include <random>
#include <fstream>
#include <vector>

class BaseState
{
public:
	BaseState();
	~BaseState();

	virtual bool contains(int s) const = 0;
private:

};
BaseState::BaseState()
{
}
BaseState::~BaseState()
{

}
class DiscreteState : public BaseState {
private:
	int const state;

public:
	DiscreteState(int state) : state(state) { }

	bool contains(int s) const override {
		return s == state;
	}
};

class SegmentState : public BaseState {
private:
	int const beg, end;

public:
	SegmentState() : beg(0), end(-1) { }
	SegmentState(int beg, int end) : beg(beg), end(end) { }

	bool contains(int s) const override {
		return s >= beg && s <= end;
	}
};

class SetState : public BaseState {
private:
	std::set<int> const states;

public:
	SetState() : states() { }
	SetState(std::set<int> const &src) : states(src) { }

	bool contains(int s) const override {
		return states.count(s) > 0;
	}
};

class ProbabilityTest {
private:
	unsigned seed;
	int test_min, test_max;
	unsigned test_count;

public:
	ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

	float operator()(BaseState const &s) const {
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> dstr(test_min, test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;

		return static_cast<float>(good) / static_cast<float>(test_count);
	};

	
};
class Crossing : public BaseState {
public:
	BaseState & State1;
	BaseState & State2;

	bool contains(int s) const override {
		return (State1.contains(s) && State2.contains(s));
	};
	Crossing(BaseState& state1, BaseState& state2) : State1(state1), State2(state2) {}
};

class Union : public BaseState {
	public:
	BaseState & State1;
	BaseState & State2;

	bool contains(int s) const override {
		return (State1.contains(s) or State2.contains(s));
	};
	Union(BaseState& state1, BaseState& state2) : State1(state1), State2(state2) {}
};

class OutAndAdd : public BaseState {
private:
	std::vector<int> const begs;
	std::vector<int> const ends;
	std::set<int> const outs;
	std::set<int> const adds;
public:
	bool contains(int s) const override {
		int s_in_segment = 0;
		for(int i = 0; i<=begs.size(); i++) {
			if (s >= begs[i] && s <= ends[i]) {
				s_in_segment = 1;
				break;
			};
		};
		return (s_in_segment > 0) || (adds.count(s) > 0) && (outs.count(s) == 0);
	};

	OutAndAdd() : begs(0), ends(-1), outs(), adds() { }
	OutAndAdd(std::vector<int> const begs, std::vector<int> const ends, std::set<int> const outs, std::set<int> const adds) : begs(begs), ends(ends), outs(outs), adds(adds) { }
};



int main(int argc, const char * argv[]) {
	DiscreteState d(1);
	SegmentState s1(0, 10);
	SegmentState s2(8, 16);
	Crossing c(s1, s2);
	Union u(s1, s2);

	std::vector <int> b{1, 8};
	std::vector<int> e{3, 17};
	std::set<int> o{2, 10, 13, 15};
	std::set<int> a{5, 7, 18, 20, 22};
	OutAndAdd oua(b, e, o, a);


	ProbabilityTest pt(10, 0, 100, 100000);
	//for (int i = 100;i < 1000000; i += 10000)
	//{
	//	ProbabilityTest pt(10, 0, 100, i);
	//	std::cout << pt(s)<<std::endl;
	//}
	//std::cout << pt(d) << std::endl;
	//std::cout << pt(s) << std::endl;
	//std::cout << pt(ss) << std::endl;
	
	std::ofstream out;
	out.open("data_7");

	for (int i = 1; i < 30000; i++) {
		ProbabilityTest pt(10, 0, 100, i);
		out << i << "	" << pt(oua) << std::endl;
	}
	return 0;
}
