#include <iostream>
#include <fstream>
#include <chrono>
#include "Header.h"
#include "network.h"
#include "matrix.h"

struct data_info {
    double* pixels;
    int digit;
};

data_info* read_mnist(std::string path, const data& Data, int& example)
{
    data_info* data;
    std::ifstream fin;
    fin.open(path);
    std::string tmp;
    fin >> tmp;
    if (tmp == "Examples") {
        fin >> example;
        std::cout << "Examples: " << example << std::endl;
        data = new data_info[example];
        for (int i = 0; i < example; ++i)
            data[i].pixels = new double[Data.size[0]];

        for (int i = 0; i < example; ++i) {
            fin >> data[i].digit;
            for (int j = 0; j < Data.size[0]; ++j) {
                fin >> data[i].pixels[j];
            }
        }
        fin.close();
        return data;
    }
    else
    {
        std::cout << "Error loading: " << path << std::endl;
        fin.close();
        return nullptr;
    }
}





int ai_func(double* input_arr)
{
    data Data{};
    data_info* data_information;
    std::ifstream fin;

    ///////////////////////////
    std::string tmp;
    int L;
    fin.open("config.txt");
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "Network") {
            fin >> L;
            Data.lays = L;
            Data.size = new int[L];
            for (int i = 0; i < L; i++) {
                fin >> Data.size[i];
            }
        }
    }
    fin.close();
    ///////////////////////////

    network i_net_work;
    std::chrono::duration<double> time;

    i_net_work.init(Data);
    //i_net_work.debug();

    double ra = 0, right, predict, maxra = 0;

    int epoch = 0;

    int study = 0;

    bool repeat = true;
    i_net_work.readweights();

    std::cout << "TESTING!!!" << std::endl;
    ////////////////////////////////

    i_net_work.setinput(input_arr);
    predict = i_net_work.forwarfeed();
    std::cout << predict << std::endl;
    i_net_work.memory_free();
    
    return static_cast<int>(predict);
}


/*

main function


 data Data{};
    data_info* data_information;
    std::ifstream fin;

    ///////////////////////////
    std::string tmp;
    int L;
    fin.open("config.txt");
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "Network") {
            fin >> L;
            Data.lays = L;
            Data.size = new int[L];
            for (int i = 0; i < L; i++) {
                fin >> Data.size[i];
            }
        }
    }
    fin.close();
    ///////////////////////////

    network i_net_work;
    std::chrono::duration<double> time;

    i_net_work.init(Data);
    i_net_work.debug();

    double ra = 0, right, predict, maxra = 0;

    int epoch = 0;

    int study;

    bool repeat = true;
    ////////////////////////////////
    while (repeat) {
        std::cout << "STUDY? (1/0)" << std::endl;
        std::cin >> study;
        if (study)
        {
            int examples;
            data_information = read_mnist("lib_MNIST.txt", Data, examples);
            auto begin = std::chrono::steady_clock::now();
            while (ra / examples * 100 < 100) {
                ra = 0;
                auto t1 = std::chrono::steady_clock::now();
                for (int i = 0; i < examples; ++i) {
                    i_net_work.setinput(data_information[i].pixels);
                    right = data_information[i].digit;
                    predict = i_net_work.forwarfeed();
                    if (predict != right) {
                        i_net_work.backpropogation(right);
                        i_net_work.weight_update(0.15 * exp(-epoch / 20.));
                    }
                    else
                        ra++;
                }
                auto t2 = std::chrono::steady_clock::now();
                time = t2 - t1;
                if (ra > maxra) maxra = ra;
                std::cout << "ra: " << ra / examples * 100 << "\t" << "maxra: " << maxra / examples * 100 << "\t" << "epoch: " << epoch << "\tTIME: " << time.count() << std::endl;
                epoch++;
                if (epoch == 30)
                    break;
            }
            auto end = std::chrono::steady_clock::now();
            time = end - begin;
            std::cout << "TIME: " << time.count() / 60. << " min" << std::endl;
            i_net_work.saveweights();
        }
        else {
            i_net_work.readweights();
        }
        std::cout << "Test? (1/0)\n";
        bool test_flag;
        std::cin >> test_flag;
        if (test_flag) {
            int ex_tests;
            data_info* data_test;
            data_test = read_mnist("lib_10k.txt", Data, ex_tests);
            ra = 0;
            for (int i = 0; i < ex_tests; ++i) {
                i_net_work.setinput(data_test[i].pixels);
                predict = i_net_work.forwarfeed();
                right = data_test[i].digit;
                if (right == predict)
                    ra++;
            }
            std::cout << "RA: " << ra / ex_tests * 100 << std::endl;
        }
        std::cout << "Repeat? (1/0)\n";
        std::cin >> repeat;
        i_net_work.memory_free();
    }



*/

///////////////