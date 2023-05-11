#include "drr.h"
#include "spq.h"

using namespace ns3;


Ptr<Packet> DRR::Schedule() {
    uint32_t idx = num_queues;
    for(uint32_t i = 0; i < num_queues; i++) {
        uint32_t cur = index + i;
        if(cur >= num_queues) cur = cur % num_queues;
        if(q_class[cur]->isEmpty()) continue;
        double_t weight = cur_weights[cur];
        uint32_t packet_size = q_class[cur]->Peek()->GetSize();
        if(weight >= packet_size) {
            idx = cur;
            cur_weights[cur] = q_class[i]->getWeight() + weight - packet_size;
            break;
        } else {
            cur_weights[cur] = q_class[i]->getWeight() + weight;
        }
    }
    if(idx == num_queues) {
        return nullptr;
    }
    index = (idx + 1) % num_queues;
    return q_class[idx]->Dequeue();
}

void DRR::readConfig(const std::string& config_file) {
    std::ifstream file(config_file);
    if (!file) {
        std::cerr << "Error opening configuration file: " << config_file << std::endl;
        return;
    }
    file >> num_queues;
    int max_packets;
    file >> max_packets;

    for (uint16_t i = 0; i < num_queues; ++i) {
        double_t weight;
        file >> weight;
        TrafficClass* q;
        cur_weights.push_back(weight);
        q = new TrafficClass(max_packets, weight, 0, false);
        q_class.push_back(q);
    }
}

Ptr<Packet> DRR::Next() const {
    uint32_t idx = num_queues;
    for(uint32_t i = 0; i < num_queues; i++) {
        uint32_t cur = index + i;
        if(cur > num_queues) cur = cur % num_queues;
        if(q_class[cur]->isEmpty()) continue;
        double_t weight = cur_weights[cur];
        uint32_t packet_size = q_class[i]->Peek()->GetSize();
        if(weight >= packet_size) {
            idx = cur;
            break;
        }
    }
    if(idx == num_queues) return nullptr;
    return q_class[idx]->Peek();
}


Ptr<Packet> SPQ::Schedule() {
    uint32_t highest_priority = q_class.size();
    for(uint32_t i = 0; i < q_class.size(); i++) {
        if(q_class[i]->isEmpty()) continue;
        if(highest_priority == q_class.size()) 
        {
            highest_priority = i;
        } else if(q_class[i]->getPriority() > q_class[highest_priority]->getPriority()) 
        {
            highest_priority = i;
        }
    }
    if(highest_priority == q_class.size()) 
    {
        return nullptr;
    }
    // std::cout << highest_priority << std::endl;
    //     std::cout << "traffic1 size " << q_class[0]->m_queue.size() << std::endl;
    //     std::cout << "traffic2 size " << q_class[1]->m_queue.size() << std::endl;
    return q_class[highest_priority]->Dequeue();
}


void SPQ::readConfig(const std::string& config_file) {
    std::ifstream file(config_file);
    if (!file) {
        std::cerr << "Error opening configuration file: " << config_file << std::endl;
        return;
    }
    int num_queues;
    file >> num_queues;
    int max_packets;
    file >> max_packets;

    for (int i = 0; i < num_queues; ++i) {
        int priority;
        file >> priority;
        TrafficClass* q;
        q = new TrafficClass(max_packets, 0, priority, false);
        q_class.push_back(q);
    }
}

Ptr<Packet> SPQ::Next() const {
    uint32_t highest_priority = q_class.size();
    for(uint32_t i = 0; i < q_class.size(); i++) {
        if(q_class[i]->isEmpty()) continue;
        if(highest_priority == q_class.size()) 
        {
            highest_priority = i;
        } else if(q_class[i]->getPriority() > q_class[highest_priority]->getPriority()) 
        {
            highest_priority = i;
        }
    }
    if(highest_priority == q_class.size())
    {
        return nullptr;
    } 
    return q_class[highest_priority]->Peek();
}

