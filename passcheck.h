#ifndef PASSCHECK_H
#define PASSCHECK_H
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
class passcheck
{
  public:
  enum class strong:int{
    very_bad=0,bad,medium,good,very_good
  };
  private:
    std::string pass="";
    std::string prev_pass_file="";
    std::string key_file="";
    mutable strong value;
    int kmp(const std::string&,const std::string& ) const;
    strong evaluate(double)const;
    class lcs
    {
        std::string a="",b="";
        mutable int** dp;
        int p,q;
      public:
        lcs(const std::string& a,const std::string& b);
        int do_lcs(int,int)const;
        ~lcs();
    };
  public:
    strong get_strength();
    passcheck(const std::string& pass,const std::string& prev_pass_file,const std::string& key_file);
    double check_lcs_file()const ;
    void check_lcs_special(const std::string&)const ;
    double pat_match_file()const;
    void pat_match_special(const std::string& s)const;
    void main_strength()const;
};
#endif //PASSCHECK_H