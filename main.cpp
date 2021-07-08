#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <algorithm>
#include <chrono>

const size_t NUMBER_OF_THREADS = 4;

using namespace std;

string process_line(string& line){
    for(size_t i=0; i<line.size(); i++){
        char ch = line[i];
        if (isdigit(line[i])){
            line[i] = '0' + (ch - '0' +1)%10;
        }else if (isupper(ch)){
            line[i] = tolower(ch);
        }else{
            line[i] = toupper(ch);
        }
    }
    return line;
}

void process_thread(vector<string>& lines, size_t begin, size_t end){
    for(size_t i=begin; i<end; i++){
        lines[i] = process_line(ref(lines[i]));
    }
}

int main() {
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    chrono::steady_clock::time_point data_load;
    try{
        cout<<"Load input data"<<endl;
        ifstream in_file("../input.data");

        if(!in_file.is_open()){
            cout<<"Fail to open file!"<<endl;
            return 0;
        }

        vector<string> lines;
        string buffer;
        while(in_file >> buffer){
            lines.push_back(buffer+"\n");
        }
        in_file.close();

        data_load = std::chrono::steady_clock::now();

        cout << "Start " << NUMBER_OF_THREADS << " threads" << endl;
        int lines_size = lines.size();
        int line_count = lines_size/NUMBER_OF_THREADS;
        vector<thread> thread_pool;
        for(size_t i=0; i<NUMBER_OF_THREADS-1; i++){
            thread_pool.emplace_back(process_thread, ref(lines), i*line_count , (i+1)*line_count-1);
        }
        thread_pool.emplace_back(process_thread, ref(lines), (NUMBER_OF_THREADS-1)*line_count ,lines_size);

        cout<<"Wait for finish"<<endl;
        for(auto& th : thread_pool){
            th.join();
        }

        cout<<"Save output data"<<endl;
        ofstream out_file("../output_cpp.data");
        for(auto &line : lines){
            out_file.write(line.c_str(), line.size());
        }
        out_file.close();

        cout<<"Done"<<endl;
    }catch (const exception& e){
        cout<<e.what();
    }

    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "data load time: " << std::chrono::duration_cast<std::chrono::milliseconds>(data_load - begin).count() << "ms" << std::endl;
    std::cout << "total execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
    return 0;
}
