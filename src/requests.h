#ifndef REQUESTS_H
#define REQUESTS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <bitset>
#include "PassiveSocket.h"

class CSimpleSocket;
class CActiveSocket;

namespace request
{

    void ShowError(CSimpleSocket &ss, const std::string &s);

    void get_API_version(CActiveSocket &SocketActive);

    void get_sw_revision(CActiveSocket &SocketActive);

    void get_gru_state(CActiveSocket &SocketActive);

    void socket_close(CActiveSocket &SocketActive);

    std::vector<int32_t> get_unloaded_num(const std::vector<int32_t> &segment_status);

    void upload_segment(CActiveSocket &SocketActive, int32_t seg_num, bool need_confirm, const std::vector<std::vector<int32_t>> &nodes);

    void upload_traj(CActiveSocket &SocketActive, const std::vector<std::vector<int32_t>> &nodes);

    void download_traject(CActiveSocket &SocketActive, int32_t points_cnt);

    std::vector<std::vector<int32_t>> get_nodes(const std::string &Traject_file_name);

    std::string hex_converting(int32_t num);

    void pack_int32_le(uint8_t *buffer, int32_t value);
}

#endif
