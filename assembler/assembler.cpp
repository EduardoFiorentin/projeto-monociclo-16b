#include <iostream> 
#include <fstream> 
#include <string> 
#include <regex> 
#include <iterator>
#include <bitset> 
#include <chrono> 
#include <cmath> 

using namespace std;

#define DEFAULT_INPUT_FILENAME "index.txt"
#define DEFAULT_OUTPUT_FILENAME "instructions.txt"
#define INST_LINE_SIZE 4

string generate_bitset_const_8(int value) {
    return bitset<8>(static_cast<int>(value)).to_string(); 
}
string generate_bitset_addrs_4(int value) {
    return bitset<4>(static_cast<int>(value)).to_string(); 
}


// load instructions from a text file and return in a vector splited by lines 
void load_instructions(vector<string> &instructions, string file) {

    ifstream instruction_file(file); 
    string line; 

    if (instruction_file.is_open()) {
        while(getline(instruction_file, line)) {
            instructions.push_back(line); 
        }
        instruction_file.close();
    } else cerr << "Instruction file '" << file << "' has not found" << endl; 
    return; 
}

// debug function to instruction transformations 
void print_string_vector(vector<string> &vector) {

    for (string item: vector) {
        cout << item << endl;
    }
    cout << endl; 

}

void replace_instructions(vector<string> &instructions) {

    // print_string_vector(instructions); 

    // main rules
    vector<pair<regex, string>> main_rules = {
        pair<regex, string> {regex{"add"}, "0000"},
        pair<regex, string> {regex{"sub"}, "0010"},
        pair<regex, string> {regex{"mul"}, "0011"},
        pair<regex, string> {regex{"div"}, "0100"},
        pair<regex, string> {regex{"lw"}, "01010000"},
        pair<regex, string> {regex{"sw"}, "0110"},
        pair<regex, string> {regex{"beq"}, "0111"},
        pair<regex, string> {regex{"bne"}, "1000"},
        pair<regex, string> {regex{"bge"}, "1001"},
        pair<regex, string> {regex{"blt"}, "1010"},
        pair<regex, string> {regex{"jal"}, "1011"},
        pair<regex, string> {regex{"li"}, "1101"}
    };

    for (int i = 0; i < main_rules.size(); i++) {
        for (int j = 0; j < instructions.size(); j++) {

            instructions[j] = regex_replace(instructions[j], main_rules[i].first, main_rules[i].second); 

        }
    }

    // group rules 

    // constants 
    regex const_pattern("\\s([-]?\\d+)");
    int num;
    for (auto &instruction: instructions) {
        smatch matches;

        while (regex_search(instruction, matches, const_pattern)) {
            num = stoi(matches.str()); 
            instruction = regex_replace(instruction, const_pattern, generate_bitset_const_8(num), regex_constants::format_first_only);
        }

    }

    // registers and memory notation
    regex reg_memory_pattern ("[rm](\\d+)");
    for (auto &instruction: instructions) {
        smatch storage_matches;

        while (regex_search(instruction, storage_matches, reg_memory_pattern)) {
            num = stoi(storage_matches[1].str()); 
            instruction = regex_replace(instruction, reg_memory_pattern, generate_bitset_addrs_4(num), regex_constants::format_first_only);
        }

    }

    // remove white spaces and ','
    regex operators_pattern("[\\s;,]+");
    for (auto &instruction: instructions) {
        instruction = regex_replace(instruction, operators_pattern, ""); 
    }

    for (auto &instruction: instructions) {
        if (instruction.length() == 12) instruction += "0000";

    }

    instructions.insert(instructions.begin(), "0000000000000000");
    instructions.push_back("1111000000000000");

}

vector<string> format_instructions(vector<string> &instructions) {

    int instr_position = 0;
    string instr_line;
    vector<string> instructions_formated;


    for (int i = 0; i < instructions.size(); i++) {
        instructions[i] = "0b" + instructions[i]; 
        instr_line += instructions[i];

        if ((i + 1) % INST_LINE_SIZE == 0) { 
            instr_line += "\n"; 
            instructions_formated.push_back(instr_line);
            instr_line = "";
        } else {
            instr_line += "\t"; 
        }
    }

    if (!instr_line.empty()) {
        instructions_formated.push_back(instr_line);
    }

    return instructions_formated;

}


void save_instructions(vector<string> &instructions, string output_filename) {
    ofstream outfile(output_filename);

    if (!outfile.is_open()) cerr << "File '" << output_filename << "' had not found";
    
    for (auto instruction: instructions) {
        outfile << instruction; 
    }
}

int main(int argc, char* argv[]) {

    auto begin = chrono::high_resolution_clock::now();

    // io files 
    string input_filename, output_filename; 
    
    // input filename 
    if (argc > 1) input_filename = argv[1];
    else input_filename = DEFAULT_INPUT_FILENAME; 

    // output filename  
    if (argc > 2) output_filename = argv[2]; 
    else output_filename = DEFAULT_OUTPUT_FILENAME; 

    vector<string> instructions; 

    // load instructions from assembly file 
    load_instructions(instructions, input_filename); 

    replace_instructions(instructions); 

    vector<string> formated_instr = format_instructions(instructions); 


    save_instructions(formated_instr, output_filename);

    auto time = chrono::high_resolution_clock::now() - begin;
    long double seconds = chrono::duration_cast<std::chrono::milliseconds>(time).count();
    std::cout << "Execution time: " << seconds / 1000 << " secs" << endl;
    return 0; 
}