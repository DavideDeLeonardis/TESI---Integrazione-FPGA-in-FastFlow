#include "ff_node_acc_t.hpp"
#include <chrono>
#include <iostream>
#include <vector>

/* Emitter come prima */
class Emitter : public ff::ff_node {
 public:
   explicit Emitter(size_t n) : sent(false), task{} {
      a.resize(n);
      b.resize(n);
      c.resize(n);
      for (size_t i = 0; i < n; ++i) {
         a[i] = i;
         b[i] = 2 * i;
      }
      task = {a.data(), b.data(), c.data(), n};
   }
   void *svc(void *) override {
      if (!sent) {
         sent = true;
         return &task;
      }
      return EOS;
   }
   const std::vector<int> &A() const { return a; }
   const std::vector<int> &B() const { return b; }
   std::vector<int> &C() { return c; }

 private:
   bool sent;
   Task task;
   std::vector<int> a, b, c;
};

/* Collector come prima */
class Collector : public ff::ff_node {
 public:
   Collector(const std::vector<int> &A, const std::vector<int> &B,
             std::vector<int> &C)
       : a(A), b(B), c(C) {}
   void *svc(void *r) override {
      if (r == EOS)
         return EOS;
      auto *res = static_cast<Result *>(r);
      bool ok = true;
      for (size_t i = 0; i < res->n; i += res->n / 16 + 1) {
         if (c[i] != a[i] + b[i]) {
            ok = false;
            break;
         }
      }
      std::cout << (ok ? "OK" : "FAIL") << "\n";
      delete res;
      return GO_ON;
   }

 private:
   const std::vector<int> &a;
   const std::vector<int> &b;
   std::vector<int> &c;
};

/* main */
int main(int argc, char *argv[]) {
   size_t N = (argc > 1 ? std::stoull(argv[1]) : 1000000);
   Emitter emitter(N);
   ff_node_acc_t accNode;
   Collector collector(emitter.A(), emitter.B(), emitter.C());
   ff::ff_Pipe<> pipe(false, &emitter, &accNode, &collector);

   auto t0 = std::chrono::steady_clock::now();
   pipe.run_and_wait_end();
   auto t1 = std::chrono::steady_clock::now();
   auto us =
      std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
   std::cout << "Elapsed: " << us << " µs\n";
   return 0;
}
