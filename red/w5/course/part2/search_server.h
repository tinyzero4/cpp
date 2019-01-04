#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <mutex>
#include <future>
#include <shared_mutex>

using namespace std;

#define MAX_DOCUMENTS 50000

#define TOP_DOCUMENTS_COUNT 5

struct DocRating {
    size_t docid;
    size_t hitcount;
};

class InvertedIndex {
public:
    InvertedIndex() : index(unordered_map<string, vector<DocRating>>(500000)) {}

    void Add(const string &document);

    const vector<DocRating> &Lookup(const string &word) const;

    const string &GetDocument(size_t id) const {
        return docs[id];
    }

    const bool Contains(const string &word) const {
        return index.count(word) != 0;
    }

    const size_t Size() {
        return docs.size();
    }

private:
    unordered_map<string, vector<DocRating>> index;
    vector<string> docs;
};

class SearchServer {
public:
    SearchServer()= default;

    explicit SearchServer(istream &document_input);

    void UpdateDocumentBase(istream &document_input);

    void AddQueriesStream(istream &query_input, ostream &search_results_output);

    void WaitForUpdate() {
        last_update.get();
    }

private:
    InvertedIndex index;
    shared_mutex write_mutex;
    future<void> last_update;
    vector<future<void>> workers_results;
};
