/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
#pragma once
#include <string>
#include <grpc++/grpc++.h>
#include "yaml-cpp/yaml.h"

using grpc::Channel;

namespace magma {


typedef struct {
  std::string ip;
  std::string port;
} ip_port_pair_t;

typedef struct {
  std::string ip;
  std::string port;
  std::string authority;
} create_grpc_channel_args_t;
/*
 * ServiceRegistrySingleton is a singleton used to get a grpc channel
 * given a service the client wants to connect to.
 */
class ServiceRegistrySingleton {
  public:
    static constexpr char* CLOUD = (char*) "cloud";
    static constexpr char* LOCAL = (char*) "local";

  public:
    static ServiceRegistrySingleton* Instance();

    static void flush(); // destroy instance

    /*
     * Gets the grpc args to the specified service based on service name
     * and destination.
     * @param service: service name to where a connection should be open.
     * @param destination: str indicating if a connection to the cloud service
     * or local service. Can be either "local" or "cloud".
     * @return create_grpc_channel_args_t, which contains a str ip,
     * a str port, and a str authority to be passed to CreateGrpcChannel.
     */
    const create_grpc_channel_args_t GetCreateGrpcChannelArgs(
      const std::string& service,
      const std::string& destination);
    /*
     * Gets the ip:port of a service if the service is local.
     * @param service: service name to where a connection should be open.
     * @return ip_port_pair_t, which contains a str ip, and a str port.
     */
    ip_port_pair_t GetServiceAddr(const std::string& service);

    /*
     * Gets the ip:port of a service in string form like "127.0.0.1:8888"
     * @param service: service name to where a connection should be open.
     * @return string of the ip:port pairing
     */
    std::string GetServiceAddrString(const std::string& service);

    /*
     * Gets a grpc connection to the specified service based on service name
     * and destination.
     * @param service: service name to where a connection should be open.
     * @param destination: str indicating if a connection to the cloud service
     * or local service. Can be either "local" or "cloud".
     * @return grpc::Channel to the given service. If a connection to cloud
     * service is requested, a connection to the control_proxy will be returned.
     */
    const std::shared_ptr<Channel> GetGrpcChannel(
      const std::string& service,
      const std::string& destination);

  private:
    ServiceRegistrySingleton(); // Prevent construction
    // Prevent construction by copying
    ServiceRegistrySingleton(const ServiceRegistrySingleton&);
    // Prevent assignment
    ServiceRegistrySingleton& operator=(const ServiceRegistrySingleton&);
    YAML::Node GetProxyConfig();
    YAML::Node GetRegistry();
    const std::shared_ptr<Channel> CreateGrpcChannel(
      const std::string& ip,
      const std::string& port,
      const std::string& authority);
    std::unique_ptr<YAML::Node> proxy_config_;
    std::unique_ptr<YAML::Node> registry_;
    static ServiceRegistrySingleton* instance_;

};

}
