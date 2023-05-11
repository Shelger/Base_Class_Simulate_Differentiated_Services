#include <iostream>
#include <vector>
#include "traffic_class.h"
#include "ns3/queue.h"


#ifndef DIFF_SERV_H
#define DIFF_SERV_H

using namespace ns3;

class DiffServ : public Queue<Packet> {
public:
    enum QueueMode {
        PACKETS,
        BYTES
    };

    DiffServ() {
        SetMode(QueueMode::PACKETS);
    }

    virtual bool Enqueue(Ptr<Packet> p) override {
        PppHeader pppHeader;
        p->RemoveHeader(pppHeader);
        bool result = DoEnqueue(p);
        p->AddHeader(pppHeader);
    // Ipv4Header ipv4Header;
    // UdpHeader udpHeader;
    // Ptr<Packet> copy = p->Copy();
    // copy->RemoveHeader(pppHeader);
    // copy->RemoveHeader(ipv4Header);
    // copy->RemoveHeader(udpHeader);

    // uint16_t destPort = udpHeader.GetDestinationPort();
    // std::cout << "Packet header info:" << std::endl;
    // std::cout << "Source IP: " << ipv4Header.GetSource() << std::endl;
    // std::cout << "Dest IP: " << ipv4Header.GetDestination() << std::endl;
    // std::cout << "Source Port: " << udpHeader.GetSourcePort() << std::endl;
    // std::cout << "Dest Port: " << udpHeader.GetDestinationPort() << std::endl;
    //   std::cout << "traffic has " << q_class.size() << std::endl;
        return result;
    }

    virtual Ptr<Packet> Dequeue() override {
        // std::cout << "dequeue now" << std::endl;
        return DoDequeue();
    }

    virtual Ptr<const Packet> Peek() const override {
        return DoPeek();
    }

    virtual Ptr<Packet> Remove() override {
        Ptr<Packet> p = Schedule();
        // DropAfterDequeue(p);
        return p;
    }

    void SetMode(QueueMode mode) {
        m_mode = mode;
    }

    QueueMode GetMode() const {
        return m_mode;
    }

    virtual Ptr<Packet> Schedule() = 0;

    uint32_t Classify(Ptr<Packet> p) {
        for(uint16_t i = 0; i < q_class.size(); i++) {
            if(q_class[i]->match(p)) {
                return i;
            }
        }
        return q_class.size();
    }

    virtual Ptr<Packet> Next() const = 0;

    virtual void readConfig(const std::string& config_file) = 0;

    void freeStuffs() {
        for(TrafficClass* tc : q_class) {
            tc->freeStuffs();
            delete tc;
        }
    }

    void AddTraffic(TrafficClass* tc) {
        q_class.push_back(tc);
    }

    void AddFilter(int idx, Filter* filter) {
        q_class[idx]->filters.push_back(filter);
    }

    // void setQueue(int idx, PointToPointHelper pointToPoint) {
    //     q_class[idx]->setQueue(pointToPoint);
    // }

protected:
    std::vector<TrafficClass*> q_class;
private:
    virtual bool DoEnqueue(Ptr<Packet> p) {
        uint32_t index = Classify(p);
        if(index >= q_class.size()) {
            return false;
        }
        return q_class[index]->Enqueue(p);
    }

    virtual Ptr<Packet> DoDequeue()  {
        return Schedule();
    }

    virtual Ptr<Packet> DoPeek() const {
        return Next();
    }

    virtual Ptr<Packet> DoRemove() {
        return Schedule();
    }

    QueueMode m_mode;
};

#endif