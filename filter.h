#include <iostream>
#include <vector>
#include <memory>
#include "filter_element.h"
#include "ns3/packet.h"


#ifndef FILTER_H
#define FILTER_H

class Filter {
public:
    Filter() {}
    Filter(ns3::Ipv4Address source_ip, ns3::Ipv4Mask source_mask,
                    ns3::Ipv4Address destination_ip, ns3::Ipv4Mask destination_mask, uint32_t destination_port,
                    uint32_t protocol)
    {
        // elements.push_back(new SourceIPAddress(source_ip));
        elements.push_back(new SourceMask(source_mask, source_ip));
        // elements.push_back(new SourcePortNumber(source_port));
        // elements.push_back(new DestinationIPAddress(destination_ip));
        elements.push_back(new DestinationMask(destination_mask, destination_ip));
        elements.push_back(new DestinationPortNumber(destination_port));
        elements.push_back(new ProtocolNumber(protocol));
    };
    
    std::vector<FilterElement*> elements;

    bool match(ns3::Ptr<ns3::Packet> p) const {
        for(FilterElement* element : elements) {
            if(!element->match(p)) {
                return false;
            }
        }
        return true;
    };


    void freeStuffs() {
        for(FilterElement* fe : elements) {
            delete fe;
        }
    }
};

#endif