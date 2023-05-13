# Base_Class_Simulate_Differentiated_Services
Yangxiaoge Zhang <br>
Two modes:<br>
1. SPQ<br>
./ns3 run path/to/runner --qos_mechanism=SPQ --config_file=path/to/file <br>
2. DRR<br>
./ns3 run path/to/runner --qos_mechanism=SPQ --config_file=path/to/file <br>
Config File Example:
First line: number of traffic <br>
Second line: Max number of Packets <br>
Third line: weight1/priority1 w2/p2 w3/p3 ... <br>