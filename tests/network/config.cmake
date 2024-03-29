################################################################################
#####
##### Tectiform TFFoundation CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

build_and_run_test(
    ipaddress_test
    IPAddressTest
    tests/network/ipaddress_test.cpp
)

build_and_run_test(
    network_interface_test
    NetworkInterfaceTest
    tests/network/network_interface_test.cpp
)

build_and_run_test(
    socket_test
    SocketTest
    tests/network/socket_test.cpp
)

build_and_run_test(
    network_interfaces_test
    NetworkInterfacesTest
    tests/network/network_interfaces_test.cpp
)

build_and_run_test(
    address_and_netmask_test
    IPAddressAndNetmaskTest
    tests/network/address_and_netmask_test.cpp
)
