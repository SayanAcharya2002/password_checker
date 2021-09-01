#include "passcheck.h"
//have to add passcheck.cpp while compiling
#include <iostream>
using namespace  std;
int main()
{
  passcheck p("!@+)(","a.txt","b.txt");
  p.main_strength();
  cout<<(p.get_strength()==passcheck::strong::very_bad)<<endl;
  cout<<(p.get_strength()==passcheck::strong::bad)<<endl;
  cout<<(p.get_strength()==passcheck::strong::medium)<<endl;
  cout<<(p.get_strength()==passcheck::strong::good)<<endl;
  cout<<(p.get_strength()==passcheck::strong::very_good)<<endl;
  cout<<static_cast<int>(p.get_strength())<<endl;
}