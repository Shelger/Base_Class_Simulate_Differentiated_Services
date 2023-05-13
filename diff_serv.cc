#include "drr.h"
#include "spq.h"

using namespace ns3;


Ptr<Packet>
DRR::Schedule() 
{
    bool check = false;
    for(uint16_t i = 0; i < num_queues; i++) 
    {
        if(!q_class[i]->isEmpty()) check = true;
    }
    if(!check) return nullptr;
    if(index >= num_queues) 
    {
        index = 0;
        q_class[index]->setCurWeight(q_class[index]->getWeight());
    }
    while(q_class[index]->isEmpty() || q_class[index]->Peek()->GetSize() > q_class[index]->getCurWeight()) 
    {
        index = (index+1) % num_queues;
        q_class[index]->setCurWeight(q_class[index]->getCurWeight() + q_class[index]->getWeight());
    }
    q_class[index]->setCurWeight(q_class[index]->getCurWeight() - q_class[index]->Peek()->GetSize());
    return q_class[index]->Dequeue();
}

void 
DRR::readConfig(const std::string& config_file) 
{
    std::ifstream file(config_file);
    if (!file) 
    {
        std::cerr << "Error opening configuration file: " << config_file << std::endl;
        return;
    }
    file >> num_queues;
    index = num_queues;
    int max_packets;
    file >> max_packets;

    for (uint16_t i = 0; i < num_queues; ++i) 
    {
        double_t weight;
        file >> weight;
        TrafficClass* q;
        // cur_weights.push_back(weight);
        q = new TrafficClass(max_packets, weight, 0, false);
        q->setCurWeight(0);
        q_class.push_back(q);
    }
}

Ptr<Packet> 
DRR::Next() const 
{
    uint32_t idx = num_queues;
    for(uint32_t i = 0; i < num_queues; i++) 
    {
        uint32_t cur = index + i;
        if(cur > num_queues) cur = cur % num_queues;
        if(q_class[cur]->isEmpty()) continue;
        double_t weight = q_class[cur]->getCurWeight();
        uint32_t packet_size = q_class[i]->Peek()->GetSize();
        if(weight >= packet_size) 
        {
            idx = cur;
            break;
        }
    }
    if(idx == num_queues) return nullptr;
    return q_class[idx]->Peek();
}


Ptr<Packet> 
SPQ::Schedule() 
{
    uint32_t highest_priority = q_class.size();
    for(uint32_t i = 0; i < q_class.size(); i++) 
    {
        if(q_class[i]->isEmpty()) continue;
        if(highest_priority == q_class.size()) 
        {
            highest_priority = i;
        } 
        else if(q_class[i]->getPriority() > q_class[highest_priority]->getPriority()) 
        {
            highest_priority = i;
        }
    }
    if(highest_priority == q_class.size()) 
    {
        return nullptr;
    }
    return q_class[highest_priority]->Dequeue();
}


void 
SPQ::readConfig(const std::string& config_file) 
{
    std::ifstream file(config_file);
    if (!file) 
    {
        std::cerr << "Error opening configuration file: " << config_file << std::endl;
        return;
    }
    int num_queues;
    file >> num_queues;
    int max_packets;
    file >> max_packets;

    for (int i = 0; i < num_queues; ++i)
    {
        int priority;
        file >> priority;
        TrafficClass* q;
        q = new TrafficClass(max_packets, 0, priority, false);
        q_class.push_back(q);
    }
}

Ptr<Packet> 
SPQ::Next() const 
{
    uint32_t highest_priority = q_class.size();
    for(uint32_t i = 0; i < q_class.size(); i++) 
    {
        if(q_class[i]->isEmpty()) continue;
        if(highest_priority == q_class.size()) 
        {
            highest_priority = i;
        } 
        else if(q_class[i]->getPriority() > q_class[highest_priority]->getPriority()) 
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

uint32_t 
DiffServ::Classify(Ptr<Packet> p) 
{
    for(uint16_t i = 0; i < q_class.size(); i++) 
    {
        if(q_class[i]->match(p)) 
        {
            return i;
        }
    }
    return q_class.size();
}

bool 
DiffServ::DoEnqueue(Ptr<Packet> p) 
{
    PppHeader pppHeader;
    p->RemoveHeader(pppHeader);
    uint32_t index = Classify(p);
    if(index >= q_class.size()) 
    {
        return false;
    }
    p->AddHeader(pppHeader);
    return q_class[index]->Enqueue(p);
}

Ptr<Packet> 
DiffServ::DoDequeue()  
{
    Ptr<Packet> p = Schedule();
    if(p == nullptr) 
    {
        return nullptr;
    }
    return p;
}