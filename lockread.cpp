#include <iostream>
#include <fstream>
#include <vector>

#include "stdint.h"
#include "time.h"

using namespace std;

enum LockState {
    LOADING,
    CLEANUP
};

struct TableLockInfo {
    uint64_t id;
    uint32_t tableOID;
    string ownerName;
    uint32_t ownerPID;
    int32_t ownerSessionID;
    int32_t ownerTxnID;
    LockState state;
    time_t creationTime;
    vector<uint32_t> dbrootList;

    void deserialize(istream &);
};


void TableLockInfo::deserialize(istream &i)  {
    uint16_t nameLen;
    uint16_t dbrootListSize;
    vector<char> buf;

    i.read((char *) &id, 8);
    i.read((char *) &tableOID, 4);
    i.read((char *) &ownerPID, 4);
    i.read((char *) &state, 4);
    i.read((char *) &ownerSessionID, 4);
    i.read((char *) &ownerTxnID, 4);
    i.read((char *) &creationTime, sizeof(time_t));
    i.read((char *) &nameLen, 2);

    buf.resize(nameLen);
    i.read(&buf[0], nameLen);
    ownerName = string(&buf[0], nameLen);

    i.read((char *) &dbrootListSize, 2);
    dbrootList.resize(dbrootListSize);
    for (auto &dbroot : dbrootList) {
        i.read((char *) &dbroot, 4);
    }
}

ostream& operator<< (ostream& os, const TableLockInfo& tli) {
    os << "id(" << tli.id << ")";
    os << ", tableOID(" << tli.tableOID << ")";
    os << ", ownerPID(" << tli.ownerPID << ")";
    os << ", state(" << (tli.state==LOADING?"LOADING":"CLEANUP") << ")";
    os << ", ownerSessionID(" << tli.ownerSessionID << ")";
    os << ", ownerTxnID(" << tli.ownerTxnID << ")";
    os << ", creationTime(" << tli.creationTime << ")";
    os << ", ownerName(" << tli.ownerName << ")";
    os << ", dbrootList(";
    for(const auto& dbroot : tli.dbrootList) {
        os << dbroot << ",";
    }
    os << ");";
}


int main(int argc, char **argv) {
    if(argc != 2) {
        cout << "usage: ./lockfix <lockfile>" << endl;
        return 1;
    }
    TableLockInfo tli;
    ifstream f(argv[1], ios::binary | ios::in);

    uint32_t size = 0;
    f.read((char *) &size, 4);
    for (uint32_t i = 0; i < size; i++) {

        tli.deserialize(f);
        cout << tli << endl;
    }
    return 0;
}