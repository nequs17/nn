#include "Header.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;

server srv;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    try {
        std::string jsonStr = msg->get_payload();  // Получение JSON строки из сообщения

        json data = json::parse(jsonStr);// Парсим JSON файл по строке

        auto test = data.template get<std::vector<int>>(); // Извлечение данных из объекта

        double* array = new double[784];

        std::cout << "\n\n";

        int j = 0;
        for (auto i : test)
        {
            std::cout << i << ' ';
            array[j] = i;
            j++;
        }
        j = 0;

        for (int i = 0; i < 784; i++)
        {
            if (i % 28 == 0)
            {
                printf("\n");
            }
            std::cout << array[i];
        }

        int predict = ai_func(array);

        std::string to_clien = std::to_string(predict);

        srv.send(hdl, to_clien, websocketpp::frame::opcode::text);
        std::cout << std::endl;

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error input JSON data: " << e.what() << std::endl;
    }
}

int main() {
    srv.set_message_handler(&on_message);

    srv.init_asio();
    srv.listen(9002);
    srv.start_accept();

    srv.run();

}