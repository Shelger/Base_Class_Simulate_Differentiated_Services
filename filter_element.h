#include <iostream>
#include <memory>
#include "ns3/packet.h"
#include "ns3/core-module.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

// Declare the FilterElements class
class FilterElement {
public:
    virtual ~FilterElement() = default;
    virtual bool match(ns3::Ptr<ns3::Packet> p) const = 0;
};

// 7 subclasses
class SourceIPAddress : public FilterElement {
public:
    SourceIPAddress(ns3::Ipv4Address value);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    ns3::Ipv4Address value_;
};


class SourceMask : public FilterElement {
public:
    SourceMask(ns3::Ipv4Mask value, ns3::Ipv4Address);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    ns3::Ipv4Mask value_;
    ns3::Ipv4Address address_;
};


class SourcePortNumber : public FilterElement {
public:
    SourcePortNumber(uint32_t value);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    uint32_t value_;
};


class DestinationIPAddress : public FilterElement {
public:
    DestinationIPAddress(ns3::Ipv4Address value);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    ns3::Ipv4Address value_;
};


class DestinationMask : public FilterElement {
public:
    DestinationMask(ns3::Ipv4Mask value, ns3::Ipv4Address);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    ns3::Ipv4Mask value_;
    ns3::Ipv4Address address_;
};


class DestinationPortNumber : public FilterElement {
public:
    DestinationPortNumber(uint32_t value);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    uint32_t value_;
};


class ProtocolNumber : public FilterElement {
public:
    ProtocolNumber(uint32_t value);
    bool match(ns3::Ptr<ns3::Packet> p) const override;
private:
    uint32_t value_;
};

#endif 