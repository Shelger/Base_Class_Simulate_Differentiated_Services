#include "filter_element.h"




SourceIPAddress::SourceIPAddress(ns3::Ipv4Address value) : value_(value) {}
bool 
SourceIPAddress::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    if (ipv4Header.GetSource() == value_) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}



SourceMask::SourceMask(ns3::Ipv4Mask value, ns3::Ipv4Address address) : value_(value), address_(address) {}

bool 
SourceMask::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    if (value_.IsMatch(address_, ipv4Header.GetSource())) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}


SourcePortNumber::SourcePortNumber(uint32_t value) : value_(value) {}

bool 
SourcePortNumber::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    ns3::UdpHeader udpHeader;
    copy->RemoveHeader(udpHeader);
    if (udpHeader.GetSourcePort() == value_) {
        return true;
    } 
    else 
    {
        return false;
    }
}


DestinationIPAddress::DestinationIPAddress(ns3::Ipv4Address value) : value_(value) {}

bool 
DestinationIPAddress::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    if (ipv4Header.GetDestination() == value_) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}


DestinationMask::DestinationMask(ns3::Ipv4Mask value, ns3::Ipv4Address address) : value_(value), address_(address) {}

bool 
DestinationMask::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    if (value_.IsMatch(address_, ipv4Header.GetDestination())) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}


DestinationPortNumber::DestinationPortNumber(uint32_t value) : value_(value) {}

bool 
DestinationPortNumber::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    ns3::UdpHeader udpHeader;
    copy->RemoveHeader(udpHeader);
    if (udpHeader.GetDestinationPort() == value_) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}



ProtocolNumber::ProtocolNumber(uint32_t value) : value_(value) {}
bool 
ProtocolNumber::match(ns3::Ptr<ns3::Packet> p) const 
{
    ns3::Ptr<ns3::Packet> copy = p->Copy(); 
    ns3::Ipv4Header ipv4Header;
    copy->RemoveHeader(ipv4Header); 
    if (ipv4Header.GetProtocol() == value_) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}


