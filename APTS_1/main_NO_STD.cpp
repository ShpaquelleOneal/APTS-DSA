#include <fstream>
#include <vector>
#include <algorithm>

// Define customs officers
struct Officer {
    unsigned int officerId;
    char officerType;
    unsigned int avgTime;
    unsigned int nextAvailableTime = 0;

    // Function that adds appointments to the officer by adding time
    void addArrival () {
        nextAvailableTime += avgTime;
    }
};

// Define arrivals
struct Arrival {
    char arrivalType;
    unsigned int arrivalTime;
    unsigned int exitTime;
    unsigned int officerID;
};

// Define a custom comparison function for sorting
bool compareArrivals(const Arrival& a, const Arrival& b) {
    // Compare exit times first
    if (a.exitTime != b.exitTime) {
        return a.exitTime < b.exitTime; // Sort by exit time in ascending order
    }
    // If exit times are equal, prioritize priority arrivals
    if (a.arrivalType != b.arrivalType) {
        return a.arrivalType == 'P'; // Priority arrivals first
    }
    // If types are equal, sort by officerID
    return a.officerID < b.officerID; // Lower officerID first
}

int main() {
    // Open text files
    std::ifstream fin("customs.in");
    std::ofstream fon("customs.out");

    // Flag to check if anything has been read
    bool isEmpty = true;

    // Read first line and store needed preconditions
    unsigned int p, n, pTime, nTime;
    fin >> p >> n >> pTime >> nTime;

    // Initialize officers arrays
    std::vector<Officer> pOfficers(p);
    std::vector<Officer> nOfficers(n);

    // Initialize arrivals array
    std::vector<Arrival> allArrivals;

    // Create officers
    for (int i = 0; i < p; i++) {
        pOfficers[i].officerType = 'P';
        pOfficers[i].officerId = i + 1;
        pOfficers[i].avgTime = pTime;
    }
    for (int i = 0; i < n; i++) {
        nOfficers[i].officerType = 'N';
        nOfficers[i].officerId = i + 1;
        nOfficers[i].avgTime = nTime;
    }

    // Proceed to descriptions of specific officers or arrivals
    char firstSymbol;
    while (fin >> firstSymbol) {
        if (firstSymbol == 'T') { // If description of particular officer
            char t;
            unsigned int officerId, newTime;
            fin >> t >> officerId >> newTime;
            // Set new processing times for particular officers
            if (t == 'P') { // If priority officer
                pOfficers[officerId - 1].avgTime = newTime;
            } else if (t == 'N') { // If non-priority officer
                nOfficers[officerId - 1].avgTime = newTime;
            }
        } else if (firstSymbol == 'P') { // Processing of a priority arrival
            unsigned int arrivalTime;
            fin >> arrivalTime;
            // Declare variables to store officer with the lowest next available time
            unsigned int nextAvailableOfficer = 4000000001;
            unsigned int nextAvailableOfficerIndex;
            bool officerAlreadyFound = false;
            // Check all officers for the lowest next available time
            for (int i = 0; i < p; i++) {
                // If the officer is already free, assign new arrival straight to him
                if (arrivalTime >= pOfficers[i].nextAvailableTime) {
                    pOfficers[i].nextAvailableTime = arrivalTime;
                    pOfficers[i].addArrival();
                    // Add a new arrival to the vector with correct parameters
                    allArrivals.push_back({'P', arrivalTime, pOfficers[i].nextAvailableTime, i});
                    officerAlreadyFound = true;
                    break;
                } else if (pOfficers[i].nextAvailableTime < nextAvailableOfficer) {
                    // Else check all officers for their time and choose the next available
                    nextAvailableOfficer = pOfficers[i].nextAvailableTime;
                    nextAvailableOfficerIndex = i;
                }
            }
            // If arrival not assigned in the loop, then assign it to the next available officer and calculate correct time
            if (!officerAlreadyFound) {
                pOfficers[nextAvailableOfficerIndex].addArrival();
                unsigned int arrivalExitTime = pOfficers[nextAvailableOfficerIndex].nextAvailableTime;
                allArrivals.push_back({'P', arrivalTime, arrivalExitTime, nextAvailableOfficerIndex});
            }
            isEmpty = false;
        } else if (firstSymbol == 'N') { // Processing of a non-priority arrival
            unsigned int arrivalTime;
            fin >> arrivalTime;
            // Declare variables to store officer with the lowest next available time
            unsigned int nextAvailableOfficer = 4000000001;
            unsigned int nextAvailableOfficerIndex;
            bool officerAlreadyFound = false;
            // Check all officers for the lowest next available time
            for (int i = 0; i < n; i++) {
                // If the officer is already free, assign new arrival straight to him
                if (arrivalTime >= nOfficers[i].nextAvailableTime) {
                    nOfficers[i].nextAvailableTime = arrivalTime;
                    nOfficers[i].addArrival();
                    // Add a new arrival to the vector with correct parameters
                    allArrivals.push_back({'N', arrivalTime, nOfficers[i].nextAvailableTime, i});
                    officerAlreadyFound = true;
                    break;
                } else if (nOfficers[i].nextAvailableTime < nextAvailableOfficer) {
                    // Else check all officers for their time and choose the next available
                    nextAvailableOfficer = nOfficers[i].nextAvailableTime;
                    nextAvailableOfficerIndex = i;
                }
            }
            // If arrival not assigned in the loop, then assign it to the next available officer and calculate correct time
            if (!officerAlreadyFound) {
                nOfficers[nextAvailableOfficerIndex].addArrival();
                unsigned int arrivalExitTime = nOfficers[nextAvailableOfficerIndex].nextAvailableTime;
                allArrivals.push_back({'N', arrivalTime, arrivalExitTime, nextAvailableOfficerIndex});
            }
            isEmpty = false;
        } else {
            break;
        }
    }

    // Sort allArrivals vector using the custom comparison function
    std::sort(allArrivals.begin(), allArrivals.end(), compareArrivals);

    // Print contents of the arrivals after sorting
    for (const auto& arrival : allArrivals) {
        fon << arrival.arrivalTime << ' ' << arrival.exitTime << std::endl;
    }

    // If nothing was read, print "nothing"
    if (isEmpty) {
        fon << "nothing";
    }

    // Close files
    fin.close();
    fon.close();
    return 0;
}
