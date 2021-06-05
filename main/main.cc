/**
 * @file main.cc
 *
 */

#include "backtrack.h"
#include "candidate_set.h"
#include "common.h"
#include "graph.h"

#include <iostream>
#include <fstream>


int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./program <data graph file> <query graph file> "
                     "<candidate set file>\n";
        return EXIT_FAILURE;
    }

    std::string data_file_name = argv[1];
    std::string query_file_name = argv[2];
    std::string candidate_set_file_name = argv[3];

    Graph data(data_file_name);
    Graph query(query_file_name, true);
    CandidateSet candidate_set(candidate_set_file_name);

    Backtrack backtrack;

    backtrack.PrintAllMatches(data, query, candidate_set);

    return EXIT_SUCCESS;
}


//int main(int argc, char* argv[]) {
//
//    vector<std::string> data_type;
//    data_type.emplace_back("lcc_hprd");
//    data_type.emplace_back("lcc_human");
//    data_type.emplace_back("lcc_yeast");
//    vector<std::string> query_type = {"_n1","_n3","_n5","_n8","_s1","_s3","_s5","_s8"};
//
//    Backtrack backtrack;
//
//    ofstream file;
//    auto backup = cout.rdbuf();
//
//    for(std::string d: data_type){
//        for(std::string q: query_type){
//
//            std::string output_name = "./out/";
//            output_name.append(d);
//            output_name.append(q);
//            output_name.append(".out");
//            file.open(output_name);
//
//            std::string data_file_name = "./data/";
//            data_file_name.append(d);
//            data_file_name.append(".igraph");
//
//            std::string query_file_name = "./query/";
//            query_file_name.append(d);
//            query_file_name.append(q);
//            query_file_name.append(".igraph");
//
//            std::string candidate_set_file_name = "./candidate_set/";
//            candidate_set_file_name.append(d);
//            candidate_set_file_name.append(q);
//            candidate_set_file_name.append(".cs");
//
//
//            cout << data_file_name << " " << query_file_name << " " << candidate_set_file_name << "\n";
//
//            Graph data(data_file_name);
//            Graph query(query_file_name, true);
//            CandidateSet candidate_set(candidate_set_file_name);
//
//            cout.rdbuf(file.rdbuf());
//            backtrack.PrintAllMatches(data, query, candidate_set);
//            cout << "\n";
//            cout.rdbuf(backup);
//
//            file.close();
//        }
//    }
//
//
//    return EXIT_SUCCESS;
//}