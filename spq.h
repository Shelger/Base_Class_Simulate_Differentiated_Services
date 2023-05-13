#include "diff_serv.h"

#ifndef SPQ_H
#define SPQ_H
using namespace ns3;

class SPQ : public DiffServ {
public:
    Ptr<Packet> Schedule() override;
    void readConfig(const std::string& config_file) override;
    virtual Ptr<Packet> Next() const override;
};

#endif