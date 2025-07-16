#define FF_HEADER_ONLY
#include <ff/buffer.hpp>  // per SWSR_Ptr_Buffer
#include <ff/ubuffer.hpp> // per uSWSR_Ptr_Buffer
#include <iostream>
#include <string>

int main() {
   // 1) Bounded queue test: capacity=2 (può contenere 1 elemento)
   std::cout << "=== SIMPLE TEST SWSR_Ptr_Buffer(cap=2) ===" << std::endl;
   ff::SWSR_Ptr_Buffer bbuf(2);
   if (!bbuf.init()) {
      std::cerr << "ERROR: failed to init SWSR_Ptr_Buffer\n";
      return 1;
   }
   int x = 42;
   void *px = &x;
   std::cout << "push(42) → " << (bbuf.push(px) ? "ok" : "fail") << std::endl;
   void *q = nullptr;
   bool got = bbuf.pop(&q);
   std::cout << "pop() → "
             << (got && q ? std::to_string(*static_cast<int *>(q)) : "fail")
             << std::endl;

   // 2) Unbounded queue test: same logic
   std::cout << "\n=== SIMPLE TEST uSWSR_Ptr_Buffer(init=2) ===" << std::endl;
   ff::uSWSR_Ptr_Buffer ubuf(2);
   if (!ubuf.init()) {
      std::cerr << "ERROR: failed to init uSWSR_Ptr_Buffer\n";
      return 1;
   }
   int y = 84;
   void *py = &y;
   std::cout << "push(84) → " << (ubuf.push(py) ? "ok" : "fail") << std::endl;
   q = nullptr;
   got = ubuf.pop(&q);
   std::cout << "pop() → "
             << (got && q ? std::to_string(*static_cast<int *>(q)) : "fail")
             << std::endl;

   return 0;
}
