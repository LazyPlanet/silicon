#include <iostream>
#include <silicon/backends/mhd.hh>
#include <silicon/api.hh>
#include <silicon/clients/libcurl_client.hh>
#include "symbols.hh"

using namespace s;

using namespace sl;

auto hello_api = http_api(

  GET = [] (auto p) { return D(_message = "This is the root of the server"); },
  GET / _test2 = [] (auto p) { return D(_message = "hello world."); },
  
  _test3 = http_api(
    POST / _test4 / _id[int()] * post_parameters(_name = std::string())
      = [] (auto p) { return D(_message = "This is the root of test3"); },
    GET / _test3 / _test2 = [] () { return D(_message = "hello world from test3."); }
    )
);

int main(int argc, char* argv[])
{

  auto ctx = mhd_json_serve(hello_api, 12345, _non_blocking);

  auto c = libcurl_json_client(hello_api, "127.0.0.1", 12345);
  auto r1 = c.http_get();
  auto r2 = c.http_get.test2();
  auto r3 = c.http_post.test3.test4(_id = 12, _name = "H");
  auto r4 = c.http_get.test3.test3.test2();
  
  std::cout << json_encode(r1) << std::endl;
  std::cout << json_encode(r2) << std::endl;
  std::cout << json_encode(r3) << std::endl;
  std::cout << json_encode(r4) << std::endl;
}
