#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

bool write_to_file(
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
        return true;
    }

    return false;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    if(write_to_file("input/input1.txt", "output/output_sequential.txt")) {
        if(write_to_file("input/input2.txt", "output/output_sequential.txt")){
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << duration.count()/pow(10, 6) << std::endl;
            return 0;
        }
    }

    return 1;
}
