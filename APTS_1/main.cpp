#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// define customs officers
struct Officer {
    unsigned int officerId;
    char officerType;
    unsigned int avgTime;
    unsigned int nextAvailableTime = 0;

    // function that adds appointments to the officer by adding time
    void addArrival () {
        nextAvailableTime += avgTime;
    }
};

// define arrivals
struct Arrival {
    char arrivalType;
    unsigned int arrivalTime;
    unsigned int exitTime;
    unsigned int officerID;
};

// define a custom comparison function for sorting
bool compareArrivals(const Arrival& a, const Arrival& b) {
    // compare exit times first
    if (a.exitTime != b.exitTime) {
        return a.exitTime < b.exitTime; // sort by exit time in ascending order
    }
    // if exit times are equal, prioritize priority arrivals
    if (a.arrivalType != b.arrivalType) {
        return a.arrivalType == 'P'; // priority arrivals first
    }
    // if types are equal, sort by officerID
    return a.officerID < b.officerID; // lower officerID first
}

int main() {
    // open text files
    ifstream fin("customs.in");
    ofstream fon("customs.out");

    // flag to check if anything has been read
    bool isEmpty = true;

    // read first line and store needed preconditions
    unsigned int p, n, pTime, nTime;
    fin >> p >> n >> pTime >> nTime;

    // initialize officers arrays
    Officer pOfficers[p];
    Officer nOfficers[n];

    // initialize arrivals array
    vector<Arrival> allArrivals;

    // create officers
    for (int i = 0; i < p; i++) {
        pOfficers[i].officerType = 'P';
        pOfficers[i].officerId = i+1;
        pOfficers[i].avgTime = pTime;
    }
    for (int i = 0; i < n; i++) {
        nOfficers[i].officerType = 'N';
        nOfficers[i].officerId = i+1;
        nOfficers[i].avgTime = nTime;
    }

    // proceed to descriptions of specific officers or arrivals
    char firstSymbol;
    while (fin >> firstSymbol){

        if (firstSymbol == 'T') { // if description of particular officer
            char t;
            unsigned int officerId, newTime;

            fin >> t >> officerId >> newTime;

            // set new processing times for particular officers
            if (t == 'P') { // if priority officer

                pOfficers[officerId - 1].avgTime = newTime;


            } else if (t == 'N') { // if non-priority officer
                nOfficers[officerId - 1].avgTime = newTime;
            }


        } else if (firstSymbol == 'P') { // processing of a priority arrival

            unsigned int arrivalTime;
            fin >> arrivalTime;


            // declare variables to store officer with the lowest next available time
            unsigned int nextAvailableOfficer = 4000000001;
            unsigned int nextAvailableOfficerIndex;

            bool officerAlreadyFound = false;

            // check all officers for the lowest next available time
            for (int i = 0; i < p; i ++) {
                // if the officer is already free, assign new arrival straight to him
                if (arrivalTime >= pOfficers[i].nextAvailableTime) {
                    pOfficers[i].nextAvailableTime = arrivalTime;
                    pOfficers[i].addArrival();

                    // add a new arrival to the vector with correct parameters
                    allArrivals.push_back({'P', arrivalTime, pOfficers[i].nextAvailableTime, i});

                    officerAlreadyFound = true;
                    break;
                } else if (pOfficers[i].nextAvailableTime < nextAvailableOfficer) { // else check all officers for their time and choose the next available
                    nextAvailableOfficer = pOfficers[i].nextAvailableTime;
                    nextAvailableOfficerIndex = i;
                }
            }

            // if arrival not assigned in the loop, then assign it to the next available officer and calculate correct time
            if (!officerAlreadyFound) {
                pOfficers[nextAvailableOfficerIndex].addArrival();
                unsigned int arrivalExitTime = pOfficers[nextAvailableOfficerIndex].nextAvailableTime;

                allArrivals.push_back({'P', arrivalTime, arrivalExitTime, nextAvailableOfficerIndex});
            }

            isEmpty = false;


        } else if (firstSymbol == 'N') { // processing of a non-priority arrival
            unsigned int arrivalTime;
            fin >> arrivalTime;

            // declare variables to store officer with the lowest next available time
            unsigned int nextAvailableOfficer = 4000000001;
            unsigned int nextAvailableOfficerIndex;

            bool officerAlreadyFound = false;

            // check all officers for the lowest next available time
            for (int i = 0; i < n; i ++) {
                // if the officer is already free, assign new arrival straight to him
                if (arrivalTime >= nOfficers[i].nextAvailableTime) {
                    nOfficers[i].nextAvailableTime = arrivalTime;
                    nOfficers[i].addArrival();

                    // add a new arrival to the vector with correct parameters
                    allArrivals.push_back({'N', arrivalTime, nOfficers[i].nextAvailableTime, i});

                    officerAlreadyFound = true;
                    break;
                } else if (nOfficers[i].nextAvailableTime < nextAvailableOfficer) { // else check all officers for their time and choose the next available
                    nextAvailableOfficer = nOfficers[i].nextAvailableTime;
                    nextAvailableOfficerIndex = i;

                }
            }

            // if arrival not assigned in the loop, then assign it to the next available officer and calculate correct time
            if (!officerAlreadyFound) {
                nOfficers[nextAvailableOfficerIndex].addArrival();
                unsigned int arrivalExitTime = nOfficers[nextAvailableOfficerIndex].nextAvailableTime;

                allArrivals.push_back({'N', arrivalTime, arrivalExitTime, nextAvailableOfficerIndex});
            }

            isEmpty = false;

        } else break;
    }

    // sort allArrivals vector using the custom comparison function
    sort(allArrivals.begin(), allArrivals.end(), compareArrivals);

    // print contents of the arrivals after sorting
    for (const auto& arrival : allArrivals) {
        fon << arrival.arrivalTime << ' ' << arrival.exitTime << endl;
    }

    // if nothing was read, print "nothing"
    if (isEmpty) {
        fon << "nothing";
    }

    // close files
    fin.close();
    fon.close();
    return 0;
}
