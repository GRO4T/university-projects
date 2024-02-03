/*
 * Project: TinyAd
 * Description: Config data used by client and server
 * Author: Rafal Kulus
 * Created: 03.05.2021
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace TinyAd {

const std::string MC_IP = "ff02::1337:1";
const int MC_PORT = 21337;
const int SRV_PORT = 21345;

}  // namespace TinyAd

#endif