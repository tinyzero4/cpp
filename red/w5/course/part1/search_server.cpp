#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <utility>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(current_document);
  }

  index = move(new_index);
}


// Document count D = 50000
// words in a document WD = 50,
// queries count Q = 50000,
// words in a query WQ = 10

// 2D + Q * WQ
void SearchServer::AddQueriesStream(istream &query_input, ostream &search_results_output) {
    vector<DocRating> docs_ratings;
    for (string current_query; getline(query_input, current_query);) {
        const size_t index_size = index.Size();
        docs_ratings.assign(index_size, {0, 0});
        size_t count_to_return = min<size_t>(TOP_DOCUMENTS_COUNT, index_size);
        for (const auto &word : SplitIntoWords(current_query)) {
            if (!index.Contains(word)) continue;

            for (const auto &usage : index.Lookup(word)) {
                docs_ratings[usage.docid].docid = usage.docid;
                docs_ratings[usage.docid].hitcount += usage.hitcount;
            }
        }

        partial_sort(
                begin(docs_ratings),
                begin(docs_ratings) + count_to_return,
                end(docs_ratings),
                [](DocRating &lhs, DocRating &rhs) {
                    int64_t lhs_docid = lhs.docid;
                    int64_t rhs_docid = rhs.docid;
                    return make_tuple(lhs.hitcount, -lhs_docid) > make_tuple(rhs.hitcount, -rhs_docid);
                }
        );

        search_results_output << current_query << ':';

        for (auto&[docid, hitcount] : Head(docs_ratings, count_to_return)) {
            if (hitcount > 0) search_results_output << " {" << "docid: " << docid << ", " << "hitcount: " << hitcount << '}';
        }

        search_results_output << endl;
    }
}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;

  unordered_map<string, size_t> wordFrequency;
  for (const auto& word : SplitIntoWords(document)) {
      wordFrequency[word]++;
  }
  for (const auto&[word, frequency]: wordFrequency) {
      index[word].push_back({docid, frequency});
  }
}

const vector<DocRating>& InvertedIndex::Lookup(const string& word) const {
    return index.at(word);
}
