#include <iostream>
#include <vector>
#include "traffic_class.h"
#include "ns3/queue.h"


#ifndef DIFF_SERV_H
#define DIFF_SERV_H

using namespace ns3;

class DiffServ : public Queue<Packet> 
{
public:
    enum QueueMode 
    {
        PACKETS,
        BYTES
    };

    DiffServ() 
    {
        SetMode(QueueMode::PACKETS);
    }

    virtual bool Enqueue(Ptr<Packet> p) override 
    {
        return DoEnqueue(p);
    }

    virtual Ptr<Packet> Dequeue() override 
    {
        return DoDequeue();
    }

    virtual Ptr<const Packet> Peek() const override 
    {
        return DoPeek();
    }

    virtual Ptr<Packet> Remove() override 
    {
        Ptr<Packet> p = Schedule();
        return p;
    }

    void SetMode(QueueMode mode) 
    {
        m_mode = mode;
    }

    QueueMode GetMode() const 
    {
        return m_mode;
    }

    virtual Ptr<Packet> Schedule() = 0;

    uint32_t Classify(Ptr<Packet> p);

    virtual Ptr<Packet> Next() const = 0;

    virtual void readConfig(const std::string& config_file) = 0;

    void freeStuffs() 
    {
        for(TrafficClass* tc : q_class) 
        {
            tc->freeStuffs();
            delete tc;
        }
    }

    void AddTraffic(TrafficClass* tc) 
    {
        q_class.push_back(tc);
    }

    void AddFilter(int idx, Filter* filter)
    {
        q_class[idx]->filters.push_back(filter);
    }

protected:
    std::vector<TrafficClass*> q_class;
private:
    virtual bool DoEnqueue(Ptr<Packet> p);

    virtual Ptr<Packet> DoDequeue();

    virtual Ptr<Packet> DoPeek() const 
    {
        return Next();
    }

    virtual Ptr<Packet> DoRemove() 
    {
        return Schedule();
    }

    QueueMode m_mode;
};

#endif