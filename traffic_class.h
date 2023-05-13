#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include "filter.h"
#include "ns3/point-to-point-module.h"

#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

class TrafficClass {
public:
    TrafficClass(uint32_t maxPackets, double_t weight,
                uint32_t priority_level, bool isDefault) :
                packets(0), maxPackets(maxPackets), weight(weight), 
                priority_level(priority_level), isDefault(isDefault) {}
    
    
    bool 
    Enqueue(ns3::Ptr<ns3::Packet> p) 
    {
        if(packets < maxPackets) 
        {
            m_queue.push(p);
            packets++;
            return true;
        }
        return false;
    }

    ns3::Ptr<ns3::Packet> 
    Dequeue() 
    {
        if(m_queue.empty()) 
        {
            return nullptr;
        }
        ns3::Ptr<ns3::Packet> p = m_queue.front();
        m_queue.pop();
        packets--;
        return p;
    }

    ns3::Ptr<ns3::Packet> 
    Peek() 
    {
        if(m_queue.empty()) 
        {
            return nullptr;
        }
        ns3::Ptr<ns3::Packet> p = m_queue.front();
        return p;
    }

    bool 
    match(ns3::Ptr<ns3::Packet> p) const 
    {
        for(Filter* filter : filters) 
        {
            if(filter->match(p)) 
            {
                return true;
            }
        }
        return false;
    }

    uint32_t 
    getPriority() 
    {
        return priority_level;
    }

    double_t 
    getWeight() 
    {
        return weight;
    }

    bool 
    isEmpty() 
    {
        return m_queue.empty();
    }
    
    void 
    freeStuffs() 
    {
        for(Filter* filter : filters) 
        {
            filter->freeStuffs();
            delete filter;
        }
    }

    double_t 
    getCurWeight() 
    {
        return cur_weight;
    }

    void setCurWeight(double_t w) 
    {
        cur_weight = w;
    }

    std::vector<Filter*> filters;
private:
    uint32_t packets;
    uint32_t maxPackets;
    double_t weight;
    double_t cur_weight;
    uint32_t priority_level;
    bool isDefault;
    std::queue<ns3::Ptr<ns3::Packet>> m_queue;
};

#endif