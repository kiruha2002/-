#include <iostream>
#include <set>
#include <random>
#include <fstream>
#include <algorithm>

class State {
public:
    virtual bool contains(int s) const = 0;
};

class DiscreteState : public State {
private:
    int const state;
public:
    explicit DiscreteState(int state) : state(state) { }

    bool contains(int s) const override {
        return s == state;
    }
};

class SegmentState : virtual public State {
private:
    int const beg, end;
public:
    SegmentState() : beg(0), end(-1) { }

    SegmentState(int beg, int end) : beg(beg), end(end) { }

    bool contains(int s) const override {
        return s >= beg && s <= end;
    }
};

class SetState : virtual public State {
friend SetState operator,(const SetState& a, const SetState& b) {
std::set<int> ab = a.states;
ab.insert(b.states.begin(), b.states.end());
return SetState(ab);
}
private:
    std::set<int> const states;
public:
    SetState() : states() { }

    explicit SetState(std::set<int> const &src) : states(src) { }

    virtual bool contains(int s) const override {
        return states.count(s) > 0;
    }
};
class SetAndSegmentState : public SetState, SegmentState {
public:
    SetAndSegmentState(int beg, int end) : SetState(), SegmentState(beg, end) { }
    bool contains(int s) const override {
        return SetState::contains(s) || SegmentState::contains(s);
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;
public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min),
                                                                                      test_max(test_max),
                                                                                      test_count(test_count) { }

    float operator()(State const &s, const std::string& logPath) const {
        std::ofstream fout(logPath + ".csv");

        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;

        for (unsigned cnt = 0; cnt != test_count; ++cnt) {
            if (s.contains(dstr(rng))) { ++good; }
            if (!(cnt % 10 + 1)) {
                std::string r = std::to_string((static_cast<double>(good) / static_cast<double>(cnt)));
                std::replace(r.begin(), r.end(), '.', ',');
                fout << r << '\n';
            }
        }
        fout.close();
        return static_cast<float>(good) / static_cast<float>(test_count);
    }
};

int main(int argc, const char *argv[]) {
    DiscreteState d(1);
    SegmentState s(0, 10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    ProbabilityTest pt(10, 0, 100, 100000);
    std::cout << pt(d, "L0") << std::endl;
    std::cout << pt(s, "L1") << std::endl;
    std::cout << pt(ss, "L2") << std::endl;
    return 0;
}
