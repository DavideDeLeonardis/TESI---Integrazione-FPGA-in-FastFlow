#pragma once
#include "../include/types.hpp"
#include "fastflow_includes.hpp"
#include <algorithm>
#include <iostream>

/* Semplificazione: non usiamo code/thread, facciamo tutto in svc() */
class ff_node_acc_t : public ff::ff_node {
 public:
   ff_node_acc_t() { std::cerr << "[ctor accNode]\n"; }
   ~ff_node_acc_t() { std::cerr << "[dtor accNode]\n"; }

   void *svc(void *t) override {
      if (t == EOS) {
         std::cerr << "[accNode] received EOS, propagating\n";
         return EOS;
      }
      auto *task = static_cast<Task *>(t);
      std::cout << "[accNode] computing " << task->n << " elements\n";
      std::transform(task->a, task->a + task->n, task->b, task->c,
                     [](int x, int y) { return x + y; });
      // alloco il result e lo ritorno
      auto *res = new Result{task->c, task->n};
      return res; // FastFlow invierà questo al Collector
   }
};
