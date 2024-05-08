//2023.12.04 tcp
//2023.12.10 udp +rx +tx
//2024.12.17

//netstat -a -p UDP
//ncat -u -l 127.0.0.1 2222


#include <iostream>
#include <fstream>
#include <vector>
#include <malloc.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdint>
using namespace std;

//#define WIN32
//#define _WIN32

//!!! #include "winsock2.h" //D:\Program Files (2)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib\libws2_32.a //ws2_32

#include "src/PassiveSocket.h" // Include header for active socket object definition

//#define MAX_PACKET 4096
constexpr size_t MAX_PACKET{4096};
const int32_t NODES_PER_PACKET{200};          // Number of points for 1 package
const uint32_t PACKETS_WO_CONFIRM{1};        // How many packages could be sent without confirmation
const double CONFIRM_TIMEOUT_SEC{0.1};

void ShowError(CSimpleSocket ss, string s)
{

    std::cout << " " << s << " : " << " = " << ss.DescribeError() << std::endl;
    std::cout << " IsSocketValid() = " << ss.IsSocketValid() << std::endl << std::endl;
} //ShowError

void switch_func (string &hexString, int32_t& counter, uint8 (&buf)[MAX_PACKET]);

void get_API_version (CActiveSocket& SocketActive){ //This function send API version request and shows response
    uint8 buf[MAX_PACKET] ;
    buf[0] = uint8(0xAA);
    buf[1] = uint8(0xAA);
    buf[2] = uint8(0x0C);
    buf[3] = uint8(0x00);

    cout << "GET API VERSION";
    cout << "SocketActive.Send = " << SocketActive.Send(buf, 4) << endl;
    ShowError(SocketActive, "SocketActive.Send");


    cout << "listening..." << endl << endl;
    cout << "SocketActive.Receive = " << hex <<SocketActive.Receive(MAX_PACKET, buf) << endl;
    ShowError(SocketActive, "SocketActive.Receive");

/// @return number of bytes actually received.
/// @return of zero means the connection has been shutdown on the other side.
/// @return of -1 means that an error has occurred.

    cout << "Bytes Received : " ;
    for(int32 ii=0; ii<SocketActive.GetBytesReceived(); ii++)
        {
            //cout << " buf[" << ii << "] = " << buf[ii] << " " << endl;
            cout << hex << buf[ii];
        } //for
        cout << endl << endl;
}// get_API_version



void get_sw_revision(CActiveSocket& SocketActive){ //This function send revision version request and shows response
    uint8 buf[MAX_PACKET] ;
    buf[0] = uint8(0xAA);
    buf[1] = uint8(0xAA);
    buf[2] = uint8(0x0D);
    buf[3] = uint8(0x00);

    cout << "GET SW REVISION";
    cout << "SocketActive.Send = " << SocketActive.Send(buf, 4) << endl;
    ShowError(SocketActive, "SocketActive.Send");


    cout << "listening..." << endl << endl;
    cout << "SocketActive.Receive = " << hex <<SocketActive.Receive(MAX_PACKET, buf) << endl;
    ShowError(SocketActive, "SocketActive.Receive");

/// @return number of bytes actually received.
/// @return of zero means the connection has been shutdown on the other side.
/// @return of -1 means that an error has occurred.

    cout << "Bytes Received : " ;
    for(int32 ii=0; ii<SocketActive.GetBytesReceived(); ii++)
        {
            //cout << " buf[" << ii << "] = " << buf[ii] << " " << endl;
            cout << hex << buf[ii];
        } //for
        cout << endl << endl;
}//get_sw_revision

void get_gru_state(CActiveSocket& SocketActive){ //This function send request of state gradient amplifier and shows response
    uint8 buf[MAX_PACKET] ;
    buf[0] = uint8(0xAA);
    buf[1] = uint8(0xAA);
    buf[2] = uint8(0x05);
    buf[3] = uint8(0x01);

    cout << "GET GRU STATE";
    cout << "SocketActive.Send = " << SocketActive.Send(buf, 4) << endl;
    ShowError(SocketActive, "SocketActive.Send");


    cout << "listening..." << endl << endl;
    cout << "SocketActive.Receive = " << hex <<SocketActive.Receive(MAX_PACKET, buf) << endl;
    ShowError(SocketActive, "SocketActive.Receive");

/// @return number of bytes actually received.
/// @return of zero means the connection has been shutdown on the other side.
/// @return of -1 means that an error has occurred.

    cout << "Bytes Received : " ;
    for(int32 ii=0; ii<SocketActive.GetBytesReceived(); ii++)
        {
            //cout << " buf[" << ii << "] = " << buf[ii] << " " << endl;
            cout << hex << buf[ii];
        } //for
        cout << endl << endl;

}//get_gru_state



void socket_close(CActiveSocket SocketActive){ // This function closes socket
std::cout << "SocketActive.Close() = " << SocketActive.Close() << std::endl;
    ShowError(SocketActive, "SocketActive.Close");
    cout << "closed" << endl;
}

vector<vector<int32_t>> get_nodes(const string& Traject_file_name) {
    vector<vector<int32_t>> nodes;
     ifstream myfile(Traject_file_name);
    if (!myfile.is_open()) { // Проверка, успешно ли открыт файл
        cerr << "Unable to open file\n"; // Выход из программы с ошибкой
    }
    int32_t num1, num2;
    while(myfile >> num1 >> num2) {
        nodes.push_back({num1, num2});
    }

    return nodes;
}//get nodes

vector <int32_t> get_unloaded_num(vector<int32_t> segment_status){
    vector <int32_t> res;
    for (uint32_t i = 0; i< segment_status.size();i++){
        if (segment_status[i] != 0){
            res.push_back(i);
        }
    }
    return res;

}//get_unloaded_num

//function to upload a segment of traject
void upload_segment(CActiveSocket &SocketActive, int32_t seg_num, bool need_confirm, vector<vector<int32_t>> nodes)
{
    int32_t counter=0;
    uint8 buf[MAX_PACKET]{0};
    stringstream s;
    s << hex << seg_num;
    string str1 = s.str();
    buf[0] = uint8(0xAA);
    buf[1] = uint8(0xAA);
    buf[2] = uint8(0x07);
    buf[3] = uint8(0x00);
    counter+=4;

    //data of segment traject

    if(need_confirm)
    {

    buf[4] = uint8(0xAA);
    buf[5] = uint8(0xAA);
    buf[6] = uint8(0x08);
    buf[7] = uint8(0x00);
    buf[8] = uint8(stoi(str1, nullptr, 16));
    counter+=5;

    }
    else{
        buf[4] = uint8(stoi(str1, nullptr, 16));
        counter++;
    }

    int32_t NULL32=0;
    int32_t NODES_SIZE = nodes.size();
    int32_t first_node_idx = max( seg_num * NODES_PER_PACKET - seg_num, NULL32 );
    int32_t last_node_idx = min( first_node_idx + NODES_PER_PACKET, NODES_SIZE) - 1;
    int32_t nodes_in_this_packet = last_node_idx - first_node_idx + 1;
    stringstream ss;
    ss << hex << nodes_in_this_packet;
    string str2 = ss.str();
    buf[counter] = uint8(stoi(str2, nullptr, 16));
    counter++;


    for(int i = first_node_idx; i <last_node_idx+1; i++)
    {

        stringstream sss_for_nodes;
        sss_for_nodes << hex << nodes[i][0];
        string hexString1 = sss_for_nodes.str();
        uint32_t tempcounter = counter;
        string temp1;
        string temp2;

        for(int j = hexString1.length()-1; j > 0; j=j- 2)
        {
            buf[tempcounter++] = uint8(stoi(hexString1.substr(j,j-2), nullptr, 16) );
        }
        if(hexString1.length() % 2 != 0)
        {
            temp1 = hexString1[0];
            hexString1.erase(0);
            buf[tempcounter++] = uint8(stoi(temp1, nullptr, 16) );
        }
        counter+=8;
        uint32_t tempcounter2 = counter;
        stringstream ss_for_nodes;
        ss_for_nodes << hex << nodes[i][1];
        string hexString2 = ss_for_nodes.str();

        for(int j = hexString2.length()-1; j > 0; j -= 2)
        {
            buf[tempcounter2++] = uint8(stoi(hexString1.substr(j,j-2), nullptr, 16) );
        }
        if(hexString2.length() % 2 != 0)
        {
            temp2 = hexString2[0];
            hexString1.erase(0);
            buf[tempcounter2++] = uint8(stoi(temp1, nullptr, 16) );
        }
        counter+=4;
    }


    cout << "SocketActive.Send = " << SocketActive.Send(buf, counter) << endl;
    ShowError(SocketActive, "SocketActive.Send");

}

void switch_func (string &hexString, int32_t& counter, uint8 (&buf)[MAX_PACKET]){
    switch(hexString.length())
    {
    case 1:
        buf[counter+1] = uint8(stoi(hexString, nullptr, 16));
        break;
    case 2:
        buf[counter+1] = uint8(stoi(hexString, nullptr, 16));
        break;
    case 3:
        {
        string part1 = hexString.substr(0,1);
        string part2 = hexString.substr(1);
        buf[counter] = uint8(stoi(part2, nullptr, 16));
        buf[counter+1] = uint8(stoi(part1, nullptr, 16));
        break;
        }
    case 4:
        {
        string part11 = hexString.substr(0,2);
        string part22 = hexString.substr(2);
        buf[counter] = uint8(stoi(part22, nullptr, 16));
        buf[counter+1] = uint8(stoi(part11, nullptr, 16));
        }
    break;
    }

}

void upload_traj(CActiveSocket& SocketActive, vector<vector<int32_t>> nodes){
    //This function send trajectory to gradient amplifier
    int32_t counter = 0;
    uint8 buf[MAX_PACKET]{0} ;
    buf[0] = uint8(0xAA);
    buf[1] = uint8(0xAA);
    buf[2] = uint8(0x06);
    buf[3] = uint8(0x00);
    counter+=4;
    stringstream ss;
    stringstream ss2;
    ss << hex << nodes.size();
    ss2 << hex << nodes[nodes.size()-1][0];

    string hexString = ss.str();
    string hexString2 = ss2.str();
    switch_func(hexString2, counter, buf);
    counter+=2;
    switch_func(hexString, counter, buf);
    counter+=2;

    cout << "UPLOADING TRAJECTORY";
    cout << "SocketActive.Send = " << SocketActive.Send(buf, 12) << endl;
    ShowError(SocketActive, "SocketActive.Send");

    int32_t nodes_cnt = nodes.size();
    int32_t segments_cnt = nodes_cnt / NODES_PER_PACKET; // segments_count

    if (nodes_cnt % NODES_PER_PACKET != 0)
        segments_cnt += 1;

    vector<int32_t> segment_status(segments_cnt); //Заполнение по умолчанию
    segment_status.assign(segments_cnt, -2);
    int32_t left_wo_confirm{PACKETS_WO_CONFIRM};

    //bool confirm_timeout_detected = false;
    //string prev_debug_info = " ";

    vector <int32_t> uploaded_nums;
    uploaded_nums = get_unloaded_num(segment_status);
    int32_t counter_uploaded_nums = uploaded_nums.size()-1;

    while (!uploaded_nums.empty()){

        int32_t seg_num;
        seg_num = uploaded_nums.back();
        uploaded_nums.pop_back();

        if (segment_status[seg_num] != -2){
            cout << "Repeating upload segment" << seg_num << "with status" <<segment_status[seg_num] <<endl;
        }//if

        //Запрашивать ли подтверждение сейчас?

        bool need_confirm = false;    //by default

        if (left_wo_confirm == 0)
            {
            left_wo_confirm = PACKETS_WO_CONFIRM;
            need_confirm = true;
            }
        else left_wo_confirm -= 1;    //for next iteration

        if (seg_num == counter_uploaded_nums)
            need_confirm = true;

        upload_segment(SocketActive, seg_num, need_confirm, nodes);

        this_thread::sleep_for(chrono::microseconds(10));
    }//while


}//upload_traj

vector <int32_t> download_traject (CActiveSocket &SocketActive, int32_t points_cnt)
{
    vector<int32_t> points;
    uint8 buf[MAX_PACKET] ;
    buf[0] = uint8(0xAA);
    buf[1] = uint8(0xAA);
    buf[2] = uint8(0x09);
    buf[3] = uint8(0x00);
    buf[4] = uint8(0x00);
    buf[5] = uint8(points_cnt);

    cout << "DOWNLOADING TRAJECTORY";
    cout << "SocketActive.Send = " << SocketActive.Send(buf, 6) << endl;
    ShowError(SocketActive, "SocketActive.Send");
    uint32_t expected_packets_cnt = points_cnt / NODES_PER_PACKET;
    if (points_cnt % NODES_PER_PACKET != 0)
    {
        expected_packets_cnt += 1;
    }
    vector<int32_t> downloaded_segments;
    downloaded_segments.assign(expected_packets_cnt, 0);
    points.assign(points_cnt, 0);
    while(!downloaded_segments.empty())
    {

        downloaded_segments.pop_back();
        cout << "listening..." << endl << endl;

        cout << "SocketActive.Receive = " << hex <<SocketActive.Receive(MAX_PACKET, buf) << endl;
        ShowError(SocketActive, "SocketActive.Receive");
        for(int i = 7; i < NODES_PER_PACKET; i++)
        {
            points.push_back(buf[i]);
        }
    }
    return points;
}
int main()
{
    int32_t points_cnt;
    CActiveSocket SocketActive( CSimpleSocket::CSocketType::SocketTypeUdp) ;
    cout << "starting" << endl;
    // Initialize our socket object
    cout << "SocketActive.Initialize = " << SocketActive.Initialize() << endl;
    ShowError(SocketActive, "SocketActive.Initialize");
    cout << "SocketActive.Open = " << SocketActive.Open("192.168.100.3", 5002) << endl;
    ShowError(SocketActive, "SocketActive.Open");


   // get_API_version(SocketActive); // Запрос версии API
   // get_sw_revision(SocketActive); // Запрос версии прошивки ГРУ
   // get_gru_state(SocketActive);  // Запрос состояния ГРУ
    string Traject_file_name, answer;
    cout << "Do you want to use default traject? [y/n]";
    cin >> answer;

    if (answer == "n") {
        cout << "Input file name (with extension):";
        cin >> Traject_file_name;
    }
    else Traject_file_name = "traject.txt";

    auto nodes = get_nodes(Traject_file_name); //filling vector nodes
    int32_t TEMP_SIZE_NODE = nodes.size();
    points_cnt = nodes[TEMP_SIZE_NODE-1][0];



    for(auto &node : nodes)
    {
        cout<<node[0]<<"\t"<<node[1]<<endl; //vector nodes, output
    }
    cout << "Filled array:"<<endl;
    upload_traj(SocketActive, nodes);
    socket_close(SocketActive);   // Закрытие сокета
    //download_traject(SocketActive, points_cnt);
    return 1;
} //main

//==========================================

