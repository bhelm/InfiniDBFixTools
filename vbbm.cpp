#include <iostream>
#include "UnbufferedFile.h"
#include <string>
#include <fstream>

typedef int32_t OID;
typedef int64_t LBID_t; /// The LBID space is currently 36 bits.  We use 64 here.

typedef int32_t VER_t;
typedef OID OID_t;
#define VBBM_MAGIC_V1 0x7b27ec13
#define VBBM_MAGIC_V2 0x1fb58c7a

struct VBBMEntry {
    LBID_t lbid;
    VER_t verID;
    OID_t vbOID;
    uint32_t vbFBO;
    int next;
    VBBMEntry()
    {
        lbid = -1;
        verID = 0;
        vbOID = 0;
        vbFBO = 0;
        next = -1;
    }

    std::string toString() {
        std::cout << "VBBMentry: lbid: " << lbid << " verID: " << verID << " oid: " << vbOID << " FBO: " << vbFBO << " next: " << next << std::endl;
    }
};


struct VBFileMetadata {
    OID_t OID;
    uint64_t fileSize;
    uint64_t nextOffset;
};

using namespace std;
using namespace idbdatafile;

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;

    if(argc != 2) {
        cout << "usage: ./vbbmfix <vbbmfile>" << endl;
        return 1;
    }

    VBFileMetadata files;


    int vbbmEntries;
	int nFiles;
	int i;




    cout << "size of file " << sizeof(VBFileMetadata) << endl;

    cout << "size of vbbm entry: " << sizeof(VBBMEntry) << endl;

    UnbufferedFile in(argv[1], "r", 0);
    int magic;

    int bytes = in.read((char *) &magic, 4);
    if (bytes != 4) {
        cout << "VBBM::load(): failed to read magic.";
        throw runtime_error("VBBM::load(): failed to read magic.");
    }
    switch (magic) {
        case VBBM_MAGIC_V1:
            cout << "magic1" << endl;
            break;
        case VBBM_MAGIC_V2:
            cout << "magic2" << endl;
            break;
        default:
            throw runtime_error("VBBM::load(): Bad magic.  Not a VBBM file?");
    }

    cout << "magic: " << magic << endl;

	if (in.read((char *) &vbbmEntries, 4) != 4) {

		throw runtime_error("VBBM::load(): Failed to read entry number");
	}
    cout << "number of entries: " << vbbmEntries << endl;

	if (in.read((char *) &nFiles, 4) != 4) {
		throw runtime_error("VBBM::load(): Failed to read file number");
	}
    cout << "number of files: " << nFiles << endl;

    for (i = 0; i < nFiles; i++) {
        if (in.read((char *) &files, sizeof(files)) != sizeof(files)) {

            throw runtime_error("VBBM::load(): Failed to load vb file meta data");
        }
    }
    cout << "done reading files" << endl;

    for (i = 0; i < vbbmEntries; i++) {
        VBBMEntry entry;
        int size = in.read((char *)&entry, sizeof(entry));
        if (size != sizeof(entry)) {
            cout << "VBBM::load(): Failed to load entry, expected " + to_string(sizeof(entry)) + " got " + to_string(size) << endl;
            break;
        }
    }

    if(i != vbbmEntries) {
        cout << "found only" << i << " of " << vbbmEntries << endl;
        cout << "this is a diff of " << vbbmEntries - i << " entries" << endl;
        fstream out(argv[1], ios::binary | ios::in | ios::out);
        out.seekp(4);
        out.write((char *)&i, 4);
        cout << "fixed " << argv[1] << endl;

    }else{
        cout << "entires count machtes, all fine " << endl;
    }

    return 0;
}