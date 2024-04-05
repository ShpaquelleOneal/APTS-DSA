#include <fstream>

// MAIN PROGRAM
int main() {

    // open text files
    std::ifstream fin("server.in.txt");
    std::ofstream fon("server.out.txt");


    // close files
    fin.close();
    fon.close();
    return 0;

}
