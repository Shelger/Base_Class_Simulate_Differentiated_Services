#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/traffic-control-module.h"
#include "diff_serv.h"
#include "drr.h"
#include "spq.h"
 
using namespace ns3;

int main(int argc, char* argv[]) {
    CommandLine cmd(__FILE__);
    std::string qos_mechanism;
    std::string config_file;
    cmd.AddValue("qos_mechanism", "QoS mechanism: 'SPQ' or 'DRR'", qos_mechanism);
    cmd.AddValue("config_file", "Configuration file for the QoS mechanism", config_file);
    cmd.Parse(argc, argv);

    Time::SetResolution (Time::NS);
    // LogComponentEnable("UdpSocketImpl", LOG_LEVEL_INFO);
    // LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
    // LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
    

    NodeContainer nodes;
    nodes.Create(3);
    PointToPointHelper pointToPoint1, pointToPoint2;
    pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
    pointToPoint1.SetChannelAttribute ("Delay", StringValue ("2ms"));
    pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    pointToPoint2.SetChannelAttribute ("Delay", StringValue ("2ms"));

    NetDeviceContainer devices1, devices2;
    devices1 = pointToPoint1.Install(nodes.Get(0), nodes.Get(1));
    devices2 = pointToPoint2.Install(nodes.Get(1), nodes.Get(2));


    InternetStackHelper stack;
    stack.Install (nodes);

    Ipv4AddressHelper address;
    Ipv4InterfaceContainer interfaces1, interfaces2;
    address.SetBase("10.1.1.0", "255.255.255.0");
    interfaces1 = address.Assign(devices1);
    address.SetBase("10.1.2.0", "255.255.255.0");
    interfaces2 = address.Assign(devices2);


    Ipv4Address sourceAddress = interfaces1.GetAddress(0);
    Ipv4Address destinationAddress = interfaces2.GetAddress(1);
    Ipv4Mask mask("255.255.255.0");

    Ptr<DiffServ> diffServ;
    if (qos_mechanism == "SPQ") {
      Ptr<SPQ> spq = CreateObject<SPQ>();
      spq->readConfig(config_file);
      diffServ = spq;
      Filter* filter1 = new Filter(sourceAddress, mask, destinationAddress, mask, 8080, 17);
      Filter* filter2 = new Filter(sourceAddress, mask, destinationAddress, mask, 8081, 17);
      spq->AddFilter(0, filter1);
      spq->AddFilter(1, filter2);

      Ptr<ns3::PointToPointNetDevice> p2pDevice1 = DynamicCast<ns3::PointToPointNetDevice>(devices1.Get(1));
      p2pDevice1->SetQueue(diffServ);
      Ptr<ns3::PointToPointNetDevice> p2pDevice2 = DynamicCast<ns3::PointToPointNetDevice>(devices2.Get(0));
      p2pDevice2->SetQueue(diffServ);

      UdpClientHelper client1 (interfaces2.GetAddress (1), 8080);
      client1.SetAttribute ("MaxPackets", UintegerValue (100000));
      client1.SetAttribute ("Interval", TimeValue (Seconds (0.001)));
      client1.SetAttribute ("PacketSize", UintegerValue (1024));
      ApplicationContainer clientApps1 = client1.Install (nodes.Get (0));
      clientApps1.Start (Seconds (2.0)); 
      clientApps1.Stop (Seconds (3.0)); 

      UdpClientHelper client2 (interfaces2.GetAddress (1), 8081);
      client2.SetAttribute ("MaxPackets", UintegerValue (100000));
      client2.SetAttribute ("Interval", TimeValue (Seconds (0.001)));
      client2.SetAttribute ("PacketSize", UintegerValue (1024));
      ApplicationContainer clientApps2 = client2.Install (nodes.Get (0));
      clientApps2.Start (Seconds (1.0)); 
      clientApps2.Stop (Seconds (3.0)); 

      PacketSinkHelper sinkA ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
      ApplicationContainer serverAppsA = sinkA.Install (nodes.Get (2));
      serverAppsA.Start (Seconds (1.0));
      serverAppsA.Stop (Seconds (20.0));

      PacketSinkHelper sinkB ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8081));
      ApplicationContainer serverAppsB = sinkB.Install (nodes.Get (2));
      serverAppsB.Start (Seconds (1.0));
      serverAppsB.Stop (Seconds (20.0));
        
// here how to use trafficcontrolhelper to connect
  } 
  else if (qos_mechanism == "DRR") 
  {
      Ptr<DRR> drr = CreateObject<DRR>();
      drr->readConfig(config_file);
      diffServ = drr;
      Filter* filter1 = new Filter(sourceAddress, mask, destinationAddress, mask, 8080, 17);
      Filter* filter2 = new Filter(sourceAddress, mask, destinationAddress, mask, 8081, 17);
      Filter* filter3 = new Filter(sourceAddress, mask, destinationAddress, mask, 8082, 17);
      drr->AddFilter(0, filter1);
      drr->AddFilter(1, filter2);
      drr->AddFilter(2, filter3);

      Ptr<ns3::PointToPointNetDevice> p2pDevice1 = DynamicCast<ns3::PointToPointNetDevice>(devices1.Get(1));
      p2pDevice1->SetQueue(diffServ);
      Ptr<ns3::PointToPointNetDevice> p2pDevice2 = DynamicCast<ns3::PointToPointNetDevice>(devices2.Get(0));
      p2pDevice2->SetQueue(diffServ);

      UdpClientHelper client1 (interfaces2.GetAddress (1), 8080);
      client1.SetAttribute ("MaxPackets", UintegerValue (100000));
      client1.SetAttribute ("Interval", TimeValue (Seconds (0.5)));
      client1.SetAttribute ("PacketSize", UintegerValue (1024));
      ApplicationContainer clientApps1 = client1.Install (nodes.Get (0));
      clientApps1.Start (Seconds (1.0)); 
      clientApps1.Stop (Seconds (3.0)); 

      UdpClientHelper client2 (interfaces2.GetAddress (1), 8081);
      client2.SetAttribute ("MaxPackets", UintegerValue (100000));
      client2.SetAttribute ("Interval", TimeValue (Seconds (0.5)));
      client2.SetAttribute ("PacketSize", UintegerValue (1024));
      ApplicationContainer clientApps2 = client2.Install (nodes.Get (0));
      clientApps2.Start (Seconds (1.0)); 
      clientApps2.Stop (Seconds (3.0)); 

      UdpClientHelper client3 (interfaces2.GetAddress (1), 8082);
      client3.SetAttribute ("MaxPackets", UintegerValue (100000));
      client3.SetAttribute ("Interval", TimeValue (Seconds (0.5)));
      client3.SetAttribute ("PacketSize", UintegerValue (1024));
      ApplicationContainer clientApps3 = client3.Install (nodes.Get (0));
      clientApps3.Start (Seconds (1.0)); 
      clientApps3.Stop (Seconds (3.0)); 

      PacketSinkHelper sinkA ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8080));
      ApplicationContainer serverAppsA = sinkA.Install (nodes.Get (2));
      serverAppsA.Start (Seconds (1.0));
      serverAppsA.Stop (Seconds (20.0));

      PacketSinkHelper sinkB ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8081));
      ApplicationContainer serverAppsB = sinkB.Install (nodes.Get (2));
      serverAppsB.Start (Seconds (1.0));
      serverAppsB.Stop (Seconds (20.0));

      PacketSinkHelper sinkC ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 8082));
      ApplicationContainer serverAppsC = sinkC.Install (nodes.Get (2));
      serverAppsC.Start (Seconds (1.0));
      serverAppsC.Stop (Seconds (20.0));
  } else {
    std::cerr << "Invalid QoS mechanism specified. Use 'SPQ' or 'DRR'." << std::endl;
    return 1;
  }

  // pointToPoint1.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue("1000p"));
  // pointToPoint2.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue("1000p"));
  // LogComponentEnable("UdpSocketImpl", LOG_LEVEL_INFO);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  pointToPoint1.EnablePcapAll("udp");
  pointToPoint2.EnablePcapAll("udp");

  Simulator::Run ();
  Simulator::Destroy ();
  diffServ->freeStuffs();
  return 0;
}