#include <fstream>
#include <string>

// LINKED LIST - REVIEWERS
// define reviewer linked list for keeping unique names
struct Reviewer {
    std::string name;
    unsigned int numberOfPseudonyms = 0;
    Reviewer* next;

    // constructor
    Reviewer (std::string name, unsigned int numberOfPseudonyms) {
        this->name = name;
        this->numberOfPseudonyms = numberOfPseudonyms;
        next = nullptr;
    }
};

struct LinkedList {
    Reviewer* head;
    // constructor
    LinkedList() : head(nullptr) {}

    // destructor to free memory
    ~LinkedList() {
        Reviewer* current = head;
        while (current != nullptr) {
            Reviewer* next = current->next;
            delete current;
            current = next;
        }
    }

    // insertion at the beginning
    void insert(std::string name, unsigned int numberOfPseudonyms) {
        Reviewer* newNode = new Reviewer(name, numberOfPseudonyms);
        newNode->next = head;
        head = newNode;
    }

    // deletion by value
    void remove(std::string name) {
        if (head == nullptr)
            return;

        // if the node to be deleted is the head
        if (head->name == name) {
            Reviewer* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        // find the node to be deleted
        Reviewer* current = head;
        while (current->next != nullptr) {
            if (current->next->name == name) {
                Reviewer* temp = current->next;
                current->next = temp->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }

    // method to access a node by name
    Reviewer* getNodeByName(std::string name) {
        Reviewer* current = head;
        while (current != nullptr) {
            if (current->name == name) {
                return current; // return the node with the given name
            }
            current = current->next;
        }
        return nullptr; // node not found
    }
};


// BINARY TREE - PSEUDONAMES - PSEUDONAME AS A KEY
// binary search tree for keeping unique pseudonames
struct node {
    unsigned int key; // pseudonames
    std::string nameAssigned;
    struct node *left, *right;
};

// create a new node with the given key and nameAssigned
struct node *newNode(unsigned int key, const std::string& nameAssigned) {
    struct node *temp = new node;
    temp->key = key;
    temp->nameAssigned = nameAssigned;
    temp->left = temp->right = NULL;
    return temp;
}

// insert a node with the given key and nameAssigned
struct node *insert(struct node *root, unsigned int key, const std::string& nameAssigned) {
    // if the tree is empty, create a new node and return it
    if (root == NULL) {
        return newNode(key, nameAssigned);
    }

    // traverse to the right place and insert the node
    if (key < root->key) {
        root->left = insert(root->left, key, nameAssigned);
    } else if (key > root->key) {
        root->right = insert(root->right, key, nameAssigned);
    }

    // return the updated node pointer
    return root;
}

// search for a node with a given key in the tree
struct node* searchNodeByKey(struct node *node, unsigned int key) {
    // traverse the tree until the node with the matching key is found
    while (node != NULL) {
        // if the key matches the current node's key, return the node
        if (key == node->key) return node;

        // move to the left subtree if the key is smaller
        if (key < node->key) node = node->left;

        // move to the right subtree if the key is larger
        else node = node->right;
    }

    // if the key is not found in the tree, return NULL
    return NULL;
}

// search for nameAssigned by key
std::string searchNameByKey(struct node *root, unsigned int key) {
    // if the tree is empty or the key is not found, return an empty string
    if (root == NULL) return "";

    // if the current node's key matches the search key, return its nameAssigned
    if (root->key == key) return root->nameAssigned;

    // if the search key is less than the current node's key, search in the left subtree
    if (key < root->key) return searchNameByKey(root->left, key);

    // if the search key is greater than the current node's key, search in the right subtree
    return searchNameByKey(root->right, key);
}

// find the inorder successor
struct node *minValueNode(struct node *node) {
    struct node *current = node;

    // find the leftmost leaf
    while (current && current->left != NULL) current = current->left;

    return current;
}

// deleting nodes with a specific nameAssigned value
struct node *deleteNodesByName(struct node *root, const std::string& nameAssigned) {
    // return if the tree is empty
    if (root == NULL) return root;

    // recursively delete nodes with the given nameAssigned in the left and right subtrees
    root->left = deleteNodesByName(root->left, nameAssigned);
    root->right = deleteNodesByName(root->right, nameAssigned);

    // if the current node has the nameAssigned to be deleted
    if (root->nameAssigned == nameAssigned) {
        // if the node is a leaf node or has only one child
        if (root->left == NULL) {
            struct node *temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            struct node *temp = root->left;
            delete root;
            return temp;
        }
        // if the node has two children
        struct node *temp = minValueNode(root->right);
        root->key = temp->key;
        root->nameAssigned = temp->nameAssigned;
        root->right = deleteNodesByName(root->right, temp->nameAssigned);
    }
    return root;
}


// function to remove duplicates from the array
int removeDuplicates(unsigned int arr[], int n) {
    if (n == 0 || n == 1) // if the array has 0 or 1 elements, no duplicates to remove
        return n;

    int index = 0; // index to store the next unique element

    // traverse the array
    for (int i = 0; i < n - 1; i++) {
        // if the current element is not equal to the next element, store it
        if (arr[i] != arr[i + 1])
            arr[index++] = arr[i];
    }

    // store the last element (which is always unique)
    arr[index++] = arr[n - 1];

    return index; // return the new size of the array with duplicates removed
}


// MAIN PROGRAM
int main() {

    // open text files
    std::ifstream fin("reviewers.in.txt");
    std::ofstream fon("reviewers.out.txt");

    // initiate linked list for storing reviewers and binary search tree for pseudonames
    LinkedList reviewers;
    struct node *root = NULL;

    // read text input
    char textEntry;
    while (fin >> textEntry) {

        // if the symbol I, then insertion command
        if (textEntry == 'I') {


            // check variable for number of pseudonyms in a name and pseudoname uniqueness
            bool pseudonameCheckPassed = true;

            // read and store name
            std::string nameEntry;
            fin >> nameEntry;


            // find reviewer in linked list
            Reviewer* foundReviewerNode = reviewers.getNodeByName(nameEntry);

            unsigned int currentReviewerPseudos = 0;

            // store temporarily their number of pseudos (if exists)
            if (foundReviewerNode != nullptr) {
                currentReviewerPseudos = foundReviewerNode->numberOfPseudonyms;
            }

            // read and store number of pseudo names
            unsigned int pseudoCount;
            fin >> pseudoCount;

            // store temporary array with pseudonames
            unsigned int pseudonamesTemp[50];

            // quick check for cases where number of pseudo names is > 50 already in the text file
            if (pseudoCount > 50) pseudonameCheckPassed = false;

            // read all pseudo names n times, make all checks
            for (int i = 0; i < pseudoCount; i++) {

                // read only if it's useful for now (the checks are not yet failed)
                if (pseudonameCheckPassed) {

                    unsigned int pseudoNames;
                    fin >> pseudoNames;

                    // try to find the node with the same pseudoname in binary tree
                    struct node* searchedPseudoNode = searchNodeByKey(root, pseudoNames);

                    // CHECK: if pseudoname doesn't exists in the data storage
                    if (searchedPseudoNode == nullptr) {

                        // add this pseudoname to the array, increment pseudos count
                        pseudonamesTemp[currentReviewerPseudos] = pseudoNames;
                        currentReviewerPseudos++;

                        // increment internal number of pseudos if not the first entry of the Reviewer
                        if (foundReviewerNode != nullptr) {
                            foundReviewerNode->numberOfPseudonyms++;
                        }

                    } else { // CHECK: pseudoname already exists check if names are the same
                        if (searchedPseudoNode->nameAssigned != nameEntry) {

                            // fail all entries if names are different, but same numbers
                            pseudonameCheckPassed = false;
                        }
                    }
                } else {

                    // if checks failed, read all other numbers and skip
                    unsigned int pseudoNames;
                    fin >> pseudoNames;
                }
            }

            // check for number of pseudonames, if after possible changes it is > 50, then fail the command
            if (currentReviewerPseudos > 50) {
                pseudonameCheckPassed = false;
            }

            // cleanse the array from possible duplicates and store new number of elements
            pseudoCount = removeDuplicates(pseudonamesTemp, currentReviewerPseudos);



            // INSERT NEW RECORDS //
            // if check passed, then add the entry to the linked list
            // and add keys - pseudonames to the binary tree
            if (pseudonameCheckPassed) {

                // add reviewer if didn't exist
                if (foundReviewerNode == nullptr) {
                    reviewers.insert(nameEntry, currentReviewerPseudos);
                }


                // copy reviewers pseudo names array to the binary tree
                for (int i = 0; i < pseudoCount; i++) {
                    root = insert(root, pseudonamesTemp[i], nameEntry);
                }

                // display "ok" if operation is successful
                fon << "ok" << std::endl;

            } else {
                // if operation unsuccessful, display "no"
                fon << "no" << std::endl;
            }
        } else if (textEntry == 'D') { // if D - deletion command

            // read pseudoname to be deleted
            unsigned pseudonameDeleteValue;
            fin >> pseudonameDeleteValue;

            //find the name by pseudoname
            std::string nameToDelete = searchNameByKey(root, pseudonameDeleteValue);

            if (nameToDelete == "") { // if name not found
                fon << "no" << std::endl;
            } else {
                // remove entry in the linked list
                reviewers.remove(nameToDelete);

                // delete all records of pseudonames with name input
                root = deleteNodesByName(root, nameToDelete);
                fon << "ok" << std::endl;
            }

        } else if (textEntry == 'L') { // if L - lookup name by one of the pseudonames

            // read pseudoname to lookup
            unsigned pseudonameLookup;
            fin >> pseudonameLookup;

            // find name in the tree
            std::string lookup = searchNameByKey(root, pseudonameLookup);

            // print out operation result
            if (lookup == "") {
                fon << "no" << std::endl;
            } else {
                fon << lookup << std::endl;
            }
        }
    }

    // close files
    fin.close();
    fon.close();
    return 0;

}
