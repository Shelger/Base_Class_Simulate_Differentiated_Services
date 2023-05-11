#include "diff_serv.h"

#ifndef DRR_H
#define DRR_H

using namespace ns3;

class DRR : public DiffServ {
public:
    DRR(){}
    DRR(std::string filename) {
        readConfig(filename);
    }
    Ptr<Packet> Schedule() override;
    void readConfig(const std::string& config_file) override;
    virtual Ptr<Packet> Next() const override;

private:
    uint32_t num_queues;
    uint32_t index;
    std::vector<double_t> cur_weights;
};

#endif