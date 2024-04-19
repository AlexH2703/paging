#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>

using namespace std;

// Page reference string and number of frames are initially defined
int pages[] = { 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5 };
int frames = 4;
int n = sizeof(pages) / sizeof(pages[0]);

// Function to print the FIFO (First-In-First-Out) page replacement algorithm output
void printFIFO(vector<int> pgTable, queue<int> pgQueue, int pageFaults) {
    // Print current state of page table
    cout << "Page Table: {";
    for (int j = 0; j < pgTable.size() - 1; j++) {
        cout << pgTable.at(j) << ", ";
    }
    if (!pgTable.empty()) {
        cout << pgTable.back();
    }
    cout << "}";

    // Print current state of frames and faults
    cout << ", Frames: [";
    queue<int> tempQueue = pgQueue;
    while (!tempQueue.empty()) {
        cout << tempQueue.front();
        tempQueue.pop();
        if (!tempQueue.empty()) cout << ",";
    }
    cout << "], Faults: " << pageFaults << endl;
}

// Function to print the LRU (Least Recently Used) and Optimal page replacement algorithm output
void printLRUOptimal(vector<int> pgTable, vector<int> pgFrames, int pageFaults) {
    // Print current state of page table
    cout << "Page Table: {";
    for (int j = 0; j < pgTable.size() - 1; j++) {
        cout << pgTable.at(j) << ", ";
    }
    if (!pgTable.empty()) {
        cout << pgTable.back();
    }
    cout << "}";

    // Print current state of frames and faults
    cout << ", Frames: [";
    for (int j = 0; j < pgFrames.size() - 1; j++) {
        cout << pgFrames.at(j) << ", ";
    }
    if (!pgFrames.empty()) {
        cout << pgFrames.back();
    }
    cout << "], Faults: " << pageFaults << endl;
}

// FIFO page replacement algorithm
int fifo() {
    queue<int> pageQueue;
    unordered_set<int> pageSet;
    int PF = 0;
    vector<int> pageTable;

    for (int i = 0; i < n; i++) {
        // If there are free frames, add page to frame and page table
        if (pageSet.size() < frames) {
            if (pageSet.find(pages[i]) == pageSet.end()) {
                pageSet.insert(pages[i]);
                PF++;
                pageQueue.push(pages[i]);
                pageTable.push_back(pages[i]);
            }
        }
        else {
            // If page is not in frame, replace page using FIFO strategy
            if (pageSet.find(pages[i]) == pageSet.end()) {
                int val = pageQueue.front();
                pageQueue.pop();
                pageSet.erase(val);
                pageSet.insert(pages[i]);
                pageQueue.push(pages[i]);
                PF++;
                // Update page table
                if (find(pageTable.begin(), pageTable.end(), pages[i]) == pageTable.end()) {
                    pageTable.push_back(pages[i]);
                }

            }
        }

        // Print current state of page table, frames, and faults
        printFIFO(pageTable, pageQueue, PF);
    }

    return PF;
}

// LRU page replacement algorithm
int lru() {
    vector<int> pageTable;
    vector<int> pageFrames;
    int PF = 0;

    // Iterating through the reference string
    for (int ref_page : pages) {
        // If a page already exists in frames, move it to the end indicating recent use
        auto it = std::find(pageFrames.begin(), pageFrames.end(), ref_page);
        if (it != pageFrames.end()) {
            pageFrames.erase(it);
            pageFrames.push_back(ref_page);
        }
        else {
            // Increment page faults
            PF++;
            // Add page to page table if not already present
            if (find(pageTable.begin(), pageTable.end(), ref_page) == pageTable.end()) {
                pageTable.push_back(ref_page);
            }
            // If frames are not full, add page to frames
            if (pageFrames.size() < frames) {
                pageFrames.push_back(ref_page);
            }
            else {
                // If frames are full, replace the least recently used page
                pageFrames.erase(pageFrames.begin());
                pageFrames.push_back(ref_page);
            }
        }

        // Print current state of page table, frames, and faults
        printLRUOptimal(pageTable, pageFrames, PF);
    }

    return PF;
}

// Function to search for a page in frames
bool searchPage(int key, vector<int>& frames)
{
    for (int i = 0; i < frames.size(); i++)
    {
        // if the frame is found, return true
        if (frames[i] == key)
            return true;
    }

    return false;
}

// Function to predict the page to replace using the Optimal algorithm
int predictPage(int page[], vector<int>& frames, int pageNumber, int pageIndex)
{
    int result = -1, farthestPage = pageIndex;
    for (int i = 0; i < frames.size(); i++)
    {
        int j;
        for (j = pageIndex; j < pageNumber; j++)
        {
            if (frames[i] == page[j])
            {
                if (j > farthestPage)
                {
                    farthestPage = j;
                    result = i;
                }
                break;
            }
        }

        if (j == pageNumber)
            return i;
    }

    if (result == -1)
        return 0;
    else
        return result;
}

// Optimal page replacement algorithm
int optimal() {
    vector<int> pageFrames;
    vector<int> pageTable;
    int PF = 0;
    for (int i = 0; i < n; i++) {
        // If page not in frames, increment page faults and add to page table
        if (!searchPage(pages[i], pageFrames)) {
            PF++;
            if (find(pageTable.begin(), pageTable.end(), pages[i]) == pageTable.end()) {
                pageTable.push_back(pages[i]);
            }
        }
        // If frames are not full, add page to frames
        if (pageFrames.size() < frames) {
            pageFrames.push_back(pages[i]);
        }
        else {
            // If frames are full, predict page to replace using Optimal algorithm
            int j = predictPage(pages, pageFrames, n, i + 1);
            pageFrames[j] = pages[i];
        }
        // Print current state of page table, frames, and faults
        printLRUOptimal(pageTable, pageFrames, PF);
    }

    return PF;
}

// Main function
int main() {
    cout << "FIFO output: " << endl;
    cout << "Total page faults: " << fifo() << endl << endl;
    cout << "LRU output: " << endl;
    cout << "Total page faults: " << lru() << endl << endl;
    cout << "Optimal output: " << endl;
    cout << "Total page faults: " << optimal() << endl;
    return 0;
}