#include "requests.h"
#include "SimpleSocket.h"
#include <cstring>

namespace request
{
    constexpr size_t MAX_PACKET{4096};
    const int32_t NODES_PER_PACKET{200};
    const uint32_t PACKETS_WO_CONFIRM{1};
    const double CONFIRM_TIMEOUT_SEC{0.1};

    std::string hex_converting(int32_t num)
    {
        std::stringstream mystream;
        mystream << std::hex << std::setw(8) << std::setfill('0') << static_cast<uint32_t>(num);
        return mystream.str();
    }

    void pack_int32_le(uint8_t *buffer, int32_t value)
    {
        uint32_t u_value = static_cast<uint32_t>(value);
        buffer[0] = static_cast<uint8_t>(u_value);
        buffer[1] = static_cast<uint8_t>(u_value >> 8);
        buffer[2] = static_cast<uint8_t>(u_value >> 16);
        buffer[3] = static_cast<uint8_t>(u_value >> 24);
    }

    void ShowError(CSimpleSocket &ss, const std::string &s)
    {
        std::cerr << " " << s << " : " << " = " << ss.DescribeError() << std::endl;
        std::cerr << " IsSocketValid() = " << ss.IsSocketValid() << std::endl
                  << std::endl;
    }

    void get_API_version(CActiveSocket &SocketActive)
    {
        uint8_t buf[MAX_PACKET];
        buf[0] = 0xAA;
        buf[1] = 0xAA;
        buf[2] = 0x0C;
        buf[3] = 0x00;

        int32_t bytesToSend = 4;
        std::cout << "GET API VERSION" << std::endl;
        std::cout << "SocketActive.Send = " << SocketActive.Send(buf, bytesToSend) << std::endl;
        ShowError(SocketActive, "SocketActive.Send");

        std::cout << "listening..." << std::endl
                  << std::endl;
        int32_t bytesReceived = SocketActive.Receive(MAX_PACKET, buf);
        std::cout << "SocketActive.Receive = " << std::dec << bytesReceived << std::endl;

        if (bytesReceived > 0)
        {
            std::cout << "Bytes Received (" << bytesReceived << ") : ";
            std::cout << std::hex << std::setfill('0');
            for (int32_t i = 0; i < bytesReceived; i++)
            {
                std::cout << std::setw(2) << static_cast<int>(buf[i]) << " ";
            }
            std::cout << std::dec << std::endl
                      << std::endl;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
        }
        else
        {
            std::cout << "Receive error occurred." << std::endl;
        }
    }

    void get_sw_revision(CActiveSocket &SocketActive)
    {
        uint8_t buf[MAX_PACKET];
        buf[0] = 0xAA;
        buf[1] = 0xAA;
        buf[2] = 0x0D;
        buf[3] = 0x00;

        int32_t bytesToSend = 4;
        std::cout << "GET SW REVISION" << std::endl;
        std::cout << "SocketActive.Send = " << SocketActive.Send(buf, bytesToSend) << std::endl;
        ShowError(SocketActive, "SocketActive.Send");

        std::cout << "listening..." << std::endl
                  << std::endl;
        int32_t bytesReceived = SocketActive.Receive(MAX_PACKET, buf);
        std::cout << "SocketActive.Receive = " << std::dec << bytesReceived << std::endl;
        ShowError(SocketActive, "SocketActive.Receive");

        if (bytesReceived > 0)
        {
            std::cout << "Bytes Received (" << bytesReceived << ") : ";
            std::cout << std::hex << std::setfill('0');
            for (int32_t i = 0; i < bytesReceived; i++)
            {
                std::cout << std::setw(2) << static_cast<int>(buf[i]) << " ";
            }
            std::cout << std::dec << std::endl
                      << std::endl;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
        }
        else
        {
            std::cout << "Receive error occurred." << std::endl;
        }
    } // get_sw_revision

    void get_gru_state(CActiveSocket &SocketActive)
    {
        uint8_t buf[MAX_PACKET];
        buf[0] = 0xAA;
        buf[1] = 0xAA;
        buf[2] = 0x05;
        buf[3] = 0x00;

        int32_t bytesToSend = 4;
        std::cout << "GET GRU STATE" << std::endl;
        std::cout << "SocketActive.Send = " << SocketActive.Send(buf, bytesToSend) << std::endl;
        ShowError(SocketActive, "SocketActive.Send");

        std::cout << "listening..." << std::endl
                  << std::endl;
        int32_t bytesReceived = SocketActive.Receive(MAX_PACKET, buf);
        std::cout << "SocketActive.Receive = " << std::dec << bytesReceived << std::endl;
        ShowError(SocketActive, "SocketActive.Receive");

        if (bytesReceived > 0)
        {
            std::cout << "Bytes Received (" << bytesReceived << ") : ";
            std::cout << std::hex << std::setfill('0');
            for (int32_t i = 0; i < bytesReceived; i++)
            {
                std::cout << std::setw(2) << static_cast<int>(buf[i]) << " ";
            }
            std::cout << std::dec << std::endl
                      << std::endl;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
        }
        else
        {
            std::cout << "Receive error occurred." << std::endl;
        }
    }

    void socket_close(CActiveSocket &SocketActive)
    {
        std::cout << "Closing socket..." << std::endl;
        if (SocketActive.Close())
        {
            std::cout << "Socket closed successfully." << std::endl;
        }
        else
        {
            ShowError(SocketActive, "SocketActive.Close");
        }
        std::cout << "IsSocketValid() after close = " << SocketActive.IsSocketValid() << std::endl;
    }

    std::vector<std::vector<int32_t>> get_nodes(const std::string &Traject_file_name)
    {
        std::vector<std::vector<int32_t>> nodes;
        std::ifstream myfile(Traject_file_name);
        if (!myfile.is_open())
        {
            std::cerr << "Error: Unable to open file: " << Traject_file_name << std::endl;
            return nodes;
        }
        int32_t num1, num2;
        while (myfile >> num1 >> num2)
        {
            nodes.push_back({num1, num2});
        }
        myfile.close();
        return nodes;
    }

    std::vector<int32_t> get_unloaded_num(const std::vector<int32_t> &segment_status)
    {
        std::vector<int32_t> res;
        for (uint32_t i = 0; i < segment_status.size(); ++i)
        {
            if (segment_status[i] != 0)
            {
                res.push_back(i);
            }
        }
        return res;
    }

    void upload_segment(CActiveSocket &SocketActive, int32_t seg_num, bool need_confirm, const std::vector<std::vector<int32_t>> &nodes)
    {
        uint8_t buf[MAX_PACKET]{0};
        size_t current_pos = 0;

        buf[current_pos++] = 0xAA;
        buf[current_pos++] = 0xAA;
        buf[current_pos++] = 0x07;

        const int32_t nodes_total_size = static_cast<int32_t>(nodes.size());
        const int32_t first_node_idx = seg_num * NODES_PER_PACKET;

        if (first_node_idx >= nodes_total_size)
        {
            std::cerr << "Warning: upload_segment called for segment " << seg_num
                      << " which is out of bounds (" << nodes_total_size << " total nodes)." << std::endl;
            return;
        }

        const int32_t last_node_idx_exclusive = std::min(first_node_idx + NODES_PER_PACKET, nodes_total_size);
        const int32_t nodes_in_this_packet = last_node_idx_exclusive - first_node_idx;

        if (nodes_in_this_packet <= 0)
        {
            std::cerr << "Warning: upload_segment called for segment " << seg_num
                      << " resulting in zero nodes." << std::endl;
            return;
        }

        buf[current_pos++] = static_cast<uint8_t>(seg_num);
    

        buf[current_pos++] = need_confirm ? 0x80 : 0x00;
        buf[current_pos++] = static_cast<uint8_t>(nodes_in_this_packet);
        buf[current_pos++] = static_cast<uint8_t>(nodes_in_this_packet >> 8);

        for (int32_t i = first_node_idx; i < last_node_idx_exclusive; ++i)
        {
            if (current_pos + 8 > MAX_PACKET)
            {
                std::cerr << "Error: Packet buffer overflow in upload_segment for segment " << seg_num << std::endl;
                return;
            }
            pack_int32_le(buf + current_pos, nodes[i][0]);
            current_pos += 4;
            pack_int32_le(buf + current_pos, nodes[i][1]);
            current_pos += 4;
        }
        std::cout << "Uploading segment " << seg_num << " (" << nodes_in_this_packet << " nodes), need_confirm=" << need_confirm << std::endl;
        int32_t bytesToSend = static_cast<int32_t>(current_pos);
        std::cout << "SocketActive.Send (segment " << seg_num << ") = " << SocketActive.Send(buf, bytesToSend) << std::endl;
        ShowError(SocketActive, "SocketActive.Send Segment");

    }

    void upload_traj(CActiveSocket &SocketActive, const std::vector<std::vector<int32_t>> &nodes)
    {
        if (nodes.empty())
        {
            std::cout << "Trajectory is empty. Nothing to upload." << std::endl;
            return;
        }

        uint8_t buf[MAX_PACKET]{0};
        size_t current_pos = 0;

        buf[current_pos++] = 0xAA;
        buf[current_pos++] = 0xAA;
        buf[current_pos++] = 0x06;
        buf[current_pos++] = 0x00;

        int32_t nodes_cnt = static_cast<int32_t>(nodes.size());
        pack_int32_le(buf + current_pos, nodes_cnt);
        current_pos += 4;

        pack_int32_le(buf + current_pos, 0);
        current_pos += 4;

        std::cout << "UPLOADING TRAJECTORY (" << nodes_cnt << " points total)" << std::endl;
        int32_t bytesToSend = static_cast<int32_t>(current_pos);
        std::cout << "SocketActive.Send (start command) = " << SocketActive.Send(buf, bytesToSend) << std::endl;
        ShowError(SocketActive, "SocketActive.Send Start Traj");

        int32_t segments_cnt = (nodes_cnt + NODES_PER_PACKET - 1) / NODES_PER_PACKET;

        std::cout << "Total segments to send: " << segments_cnt << std::endl;

        for (int32_t seg_num = 0; seg_num < segments_cnt; ++seg_num)
        {
            bool need_confirm = ((seg_num + 1) % PACKETS_WO_CONFIRM == 0) || (seg_num == segments_cnt - 1);

            upload_segment(SocketActive, seg_num, need_confirm, nodes);
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        std::cout << "Trajectory upload initiated." << std::endl;

    }

    void download_traject(CActiveSocket &SocketActive, int32_t points_cnt)
    {
        uint8_t buf[MAX_PACKET]{0};
        size_t current_pos = 0;

        buf[current_pos++] = 0xAA;
        buf[current_pos++] = 0xAA;
        buf[current_pos++] = 0x09;
        buf[current_pos++] = 0x00;

        pack_int32_le(buf + current_pos, 0);
        current_pos += 4;

        pack_int32_le(buf + current_pos, points_cnt);
        current_pos += 4;

        std::cout << "REQUESTING DOWNLOAD TRAJECTORY (" << points_cnt << " points)" << std::endl;
        int32_t bytesToSend = static_cast<int32_t>(current_pos);
        std::cout << "SocketActive.Send (download request) = " << SocketActive.Send(buf, bytesToSend) << std::endl;
        ShowError(SocketActive, "SocketActive.Send Download Req");

        std::cout << "listening for downloaded trajectory..." << std::endl
                  << std::endl;

        std::vector<uint8_t> downloaded_data;
        int32_t total_bytes_received = 0;
        int32_t expected_total_bytes = points_cnt * 8;

        while (true)
        {
            int32_t bytesReceived = SocketActive.Receive(MAX_PACKET, buf);
            std::cout << "SocketActive.Receive = " << std::dec << bytesReceived << std::endl;

            if (bytesReceived > 0)
            {
                ShowError(SocketActive, "SocketActive.Receive (during download)");
                downloaded_data.insert(downloaded_data.end(), buf, buf + bytesReceived);
                total_bytes_received += bytesReceived;
                std::cout << "Received " << bytesReceived << " bytes. Total downloaded: " << total_bytes_received << std::endl;
                if (total_bytes_received >= expected_total_bytes)
                {
                    std::cout << "Expected number of bytes received." << std::endl;
                    break;
                }
            }
            else if (bytesReceived == 0)
            {
                std::cout << "Connection closed by peer during download." << std::endl;
                break;
            }
            else
            {
                if (SocketActive.IsSocketValid() && SocketActive.GetSocketError() == CSimpleSocket::SocketTimedout)
                {
                    std::cout << "Receive timeout occurred." << std::endl;
                }
                else
                {
                    ShowError(SocketActive, "SocketActive.Receive Error");
                }
                break;
            }
        }

        std::cout << "Total bytes received: " << total_bytes_received << std::endl;
        std::cout << "Parsing downloaded data..." << std::endl;

        std::ofstream outFile("downloaded_data_raw.bin", std::ios::binary);
        if (!outFile.is_open())
        {
            std::cerr << "Error opening file downloaded_data_raw.bin for writing" << std::endl;
        }
        else
        {
            outFile.write(reinterpret_cast<const char *>(downloaded_data.data()), downloaded_data.size());
            outFile.close();
            std::cout << "Raw downloaded data saved to downloaded_data_raw.bin" << std::endl;
        }

        std::ofstream outTextFile("downloaded_data_parsed.txt");
        if (!outTextFile.is_open())
        {
            std::cerr << "Error opening file downloaded_data_parsed.txt for writing" << std::endl;
        }
        else
        {
            size_t data_start_offset = 0;
            int points_parsed = 0;
            for (size_t i = data_start_offset; i + 7 < downloaded_data.size(); i += 8)
            {
                int32_t x, y;
                memcpy(&x, &downloaded_data[i], 4);
                memcpy(&y, &downloaded_data[i + 4], 4);
                outTextFile << x << "\t" << y << std::endl;
                points_parsed++;
            }
            outTextFile.close();
            std::cout << "Parsed " << points_parsed << " points saved to downloaded_data_parsed.txt" << std::endl;
        }

    }

}
