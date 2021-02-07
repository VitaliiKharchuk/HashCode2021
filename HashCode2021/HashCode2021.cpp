#include <cstdlib>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>

#include <windows.h>
#include <string>
#include <iostream>

typedef uint32_t Ingredient;
typedef uint64_t Score;

using namespace std;

class IngredientsMapper {
    std::unordered_map<std::string, Ingredient> table;

    IngredientsMapper() {}

public:
    static IngredientsMapper& get() {
        static IngredientsMapper singleton;
        return singleton;
    }

    Ingredient operator[](const std::string& name) {
        return table.count(name) ? table[name] : (table[name] = table.size());
    }

    void clear() { table.clear(); }

    size_t count() { return table.size(); }
};


class Pizza {
    std::unordered_set<Ingredient> ingredients;
    size_t id;
public:
    Pizza() { }
    Pizza(size_t id) : id(id) { }
    Pizza(const Pizza& cp) : ingredients(cp.ingredients), id(cp.id) { }
    Pizza(Pizza&& mv) : ingredients(std::move(mv.ingredients)), id(mv.id) { }
    Pizza& operator=(const Pizza& cp) = default;
    Pizza& operator=(Pizza&& mv) {
        ingredients = std::move(mv.ingredients);
        return *this;
    }
    ~Pizza() = default;
    size_t getId() const { return id; }
    void add(Ingredient i) { ingredients.insert(i); }
    void add(const Pizza& cp) {
        // TODO!!! Need optimization
        for (Ingredient i : cp.ingredients)
            ingredients.insert(i);
    }
    size_t size() const { return ingredients.size(); }
};


int errexit(const char* msg, int errcode = EXIT_FAILURE) {
    std::cout << msg << std::endl;
    return errcode;
}

class Team {
    std::vector<Pizza> pizzas;
public:
    Team() {}
    void add(Pizza&& pizza) {
        pizzas.push_back(pizza);
    }
    size_t size() const { return pizzas.size(); }
    Score getScore() const {
        Pizza total;
        for (auto& pizza : pizzas)
            total.add(pizza);
        return total.size() * total.size();
    }
};

std::vector<Pizza> pizzas;
std::unordered_set<size_t> available;

std::vector<Team> bestSolution;
Score bestScore;
std::vector<Team> currentSolution;
Score currentScore;

/*
// TODO!!! Add backtracking
void brutal(unsigned T2, unsigned T3, unsigned T4) {
    if (T4 > 0 && available.size() >= 4) {

        brutal(T2, T3, T4 - 1);
    }
    if (T3 > 0 && available.size() >= 3) {
        ...
            brutal(T2, T3 - 1, 0);
    }
    if (T2 > 0 && available.size() >= 2) {



        brutal(T2 - 1, 0, 0);
    }
    // Optimize
    if (currentScore > bestScore) {
        bestScore = currentScore;
        bestSolution = currentSolution;
    }
}
*/

int main(int argc, char* argv[]) {
    IngredientsMapper& mapper = IngredientsMapper::get();

    std::ifstream infile("../Input.txt");
    if (!infile.is_open())
        return errexit("Cannot open file");

    unsigned M, T2, T3, T4;
    if (!(infile >> M >> T2 >> T3 >> T4))
        return errexit("No filename given");
    if (M == 0 || T2 + T3 + T4 == 0)
        return errexit("Bad initial params");

    pizzas.reserve(M);
    for (unsigned p = 0; p < M; ++p) {
        unsigned I;
        if (!(infile >> I))
            return errexit("Bad format");
        pizzas.emplace_back(pizzas.size());
        Pizza& pizza = pizzas[p];
        for (unsigned i = 0; i < I; ++i) {
            std::string name;
            if (!(infile >> name))
                return errexit("Bad format (2)");
            pizza.add(mapper[name]);
        }
    }

    std::cout << "Read " << M << " pizzas, totally " << mapper.count() << " ingredients.\n";

    return EXIT_SUCCESS;
}
