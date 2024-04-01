#include <fstream>

// define customs officers
struct Reviewer {

};

int main() {
    // open text files
    std::ifstream fin("customs.in");
    std::ofstream fon("customs.out");


    // close files
    fin.close();
    fon.close();
    return 0;
}
