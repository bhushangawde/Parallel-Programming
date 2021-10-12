#include <iostream>
#include <vector>

using namespace std;

class PgTable
{
  public:
    PgTable(int log, int phys);
    bool isValid(int page);
    void accessPg(int page);
    void storePg(int page);
  private:
    int num;

    class PTEntry
    {
      public:
        int frame;
        int timeStamp;
        bool valid;
        PTEntry() {
            valid = false;
        }
    };
    vector<PTEntry> pgMap;
    vector<int> freeFr;

    int selectSwapPage();
};

PgTable::PgTable(int log, int phys){
    num = 0;
    pgMap.resize(log);

    for (int i = phys-1; i >= 0; i--) {
        freeFr.push_back(i);
    }
}

bool PgTable::isValid(int page){
    return pgMap[page].valid;
}

void PgTable::accessPg(int page){
    cout << page << ": page found at frame " << pgMap[page].frame << endl;
}

void PgTable::storePg(int page){
    int frameNum;
    if (freeFr.empty()) {
        int swapPage = selectSwapPage();
        pgMap[swapPage].valid = false;
        frameNum = pgMap[swapPage].frame;
        cout << page << ": PAGE FAULT -- swapping page " << page << " into frame "
             << frameNum << endl << "   ";
    }
    else {
        frameNum = freeFr.back();
        freeFr.pop_back();
        cout << page << ": PAGE FAULT -- inserting page " << page << " into frame "
             << frameNum << endl << "   ";
    }

    pgMap[page].frame = frameNum;
    cout<<frameNum<<endl<<endl;
    pgMap[page].timeStamp = num;
    pgMap[page].valid = true;

    num++;
}

int PgTable::selectSwapPage(){
    int oldPage, oldTime = num;
    for (int i = 0; i < pgMap.size(); i++) {
        if (pgMap[i].valid && pgMap[i].timeStamp < oldTime) {
            oldTime = pgMap[i].timeStamp;
            oldPage = i;
        }
    }
    return oldPage;
}

int main()
{
    int log;
    cout << "Enter number of pages in logical memory: " << endl;
    cin >> log;

    int phys;
    cout << "Enter number of frames in physical memory: " << endl;
    cin >> phys;

    PgTable PT(log, phys);

    cout << "Enter the page-reference string: " << endl;
    int page;
    cin >> page;
    while (page != -1) {
        if (PT.isValid(page)) {
            PT.accessPg(page);
        }
        else {
            PT.storePg(page);
            PT.accessPg(page);
        }
        cin >> page;
    }
    return 0;
}
