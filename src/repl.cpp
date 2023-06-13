//
// Created by 24627 on 2023/5/22.
//

#include "repl.h"

#include <fstream>
#include <iostream>
#include <stack>

#include "./eval_env.h"
#include "./parser.h"
#include "./tokenizer.h"
#include "error.h"
bool loadFileFinishHelper(std::string statement)  // 判断表达式是否完整
{
    int brackets{};
    for (auto i : statement) {
        if (i == '(') {
            brackets++;
        } else if (i == ')') {
            brackets--;
        }
        if (brackets < 0) {
            return false;
        }
    }
    if (brackets == 0) {
        return true;
    } else {
        return false;
    }
}
void repl() {
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;
            std::getline(std::cin, line);
            while (!loadFileFinishHelper(line)) {
                std::string temp;
                std::getline(std::cin, temp);
                line += temp;
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));  // TokenPtr 不支持复制
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            std::cout << result->toString() << std::endl;  // 输出外部表示
        } catch (std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void loadFile(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        std::exit(1);
    } else {
        std::cout << "loading file " << filename << std::endl;
    }
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::string line;
            std::getline(file, line);
            while (!loadFileFinishHelper(line)) {
                std::string temp;
                std::getline(file, temp);
                line += temp;
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));  // TokenPtr 不支持复制
            auto value = parser.parse();
            env->eval(std::move(value));
            if (file.eof()) {
                exit(0);
            }
        } catch (std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
