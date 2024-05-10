#include "src/PassiveSocket.h" // Include header for active socket object definition
#include "src/requests.h"
using namespace std;

int main()
{
    int32_t points_cnt;
    CActiveSocket SocketActive( CSimpleSocket::CSocketType::SocketTypeUdp) ;
    cout << "starting" << endl;
    // Initialize our socket object
    cout << "SocketActive.Initialize = " << SocketActive.Initialize() << endl;
    request::ShowError(SocketActive, "SocketActive.Initialize");
    cout << "SocketActive.Open = " << SocketActive.Open("192.168.100.3", 5002) << endl;
    request::ShowError(SocketActive, "SocketActive.Open");


   request::get_API_version(SocketActive); // Запрос версии API
   request::get_sw_revision(SocketActive); // Запрос версии прошивки ГРУ
   request::get_gru_state(SocketActive);  // Запрос состояния ГРУ
    string Traject_file_name, answer;
    cout << "Do you want to use default traject? [y/n]";
    cin >> answer;

    if (answer == "n") {
        cout << "Input file name (with extension):";
        cin >> Traject_file_name;
    }
    else Traject_file_name = "traject.txt";

    auto nodes = request::get_nodes(Traject_file_name); //filling vector nodes
    int32_t TEMP_SIZE_NODE = nodes.size();
    points_cnt = nodes[TEMP_SIZE_NODE-1][0];



    for(auto &node : nodes)
    {
        cout<<node[0]<<"\t"<<node[1]<<endl; //vector nodes, output
    }
    cout << "Filled array:"<<endl;
    request::upload_traj(SocketActive, nodes);
    request::socket_close(SocketActive);   // Закрытие сокета
    //download_traject(SocketActive, points_cnt);
    return 0;
} //main

//==========================================

