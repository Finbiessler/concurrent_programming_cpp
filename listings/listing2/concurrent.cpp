#include <iostream>
#include <fstream>
#include <string>
#include <thread> 
#include <math.h>

void write_to_file(std::string, std::string);

void write_to_file(
    std::string input_file_path,
    std::string ouput_file_path
){
    std::ifstream input;
    std::ofstream output;

    input.open(input_file_path);
    output.open(ouput_file_path, std::ios::app);

    if(input.is_open()){
        std::string line = "";

        while(getline(input, line)){
            output << line << std::endl; 
        }

        input.close();
        output.close();
    }
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();

    std::thread t1(write_to_file, "input/input1.txt", "output/output_concurrent.txt");
    std::thread t2(write_to_file, "input/input2.txt", "output/output_concurrent.txt");
    t1.join();
    t2.join();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count()/pow(10, 6) << std::endl;

    return 0;
}
