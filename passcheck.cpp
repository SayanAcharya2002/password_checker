#include "passcheck.h"

passcheck::lcs::lcs(const std::string& a,const std::string& b): a(a),b(b)
{
  p=a.size();
  q=b.size();
  dp=new int*[p];
  for(int i=0;i<p;++i)
  {
    dp[i]=new int[q]{};//defualt is zero. so there is a bias of one, meaning -1 ==> 0
  }
}
int passcheck::lcs::do_lcs(int x,int y)const
{
  if(x==p || y==q)
    return 0;
  else if(dp[x][y])
    return dp[x][y]-1;
  else
  {
    int val1=do_lcs(x+1,y);
    int val2=do_lcs(x,y+1);    
    int val3=do_lcs(x+1,y+1);
    dp[x][y]=std::max(val1,std::max(val2,(a[x]==b[y])+val3))+1;    
    return dp[x][y]-1;
  }
}
passcheck::lcs::~lcs()
{
  for(int i=0;i<p;++i)
    delete[] dp[i];
  delete[] dp;
}
passcheck::passcheck(const std::string& pass,const std::string& prev_pass_file,const std::string& key_file)
  :pass(pass),prev_pass_file(prev_pass_file),key_file(key_file)
{

}
double passcheck::check_lcs_file()const 
{
  std::ifstream fin(prev_pass_file,std::ios::in);
  int count=0;
  double tot_match_frac=0;
  //std::cout<<prev_pass_file<<"\n";
  while(!fin.eof())
  {
    ++count;
    std::string s;
    getline(fin,s);
    // std::cout<<"string: "<<s<<"\n";
    lcs temp_lcs(pass,s);
    int match=temp_lcs.do_lcs(0,0);
    double temp=(double)match/(double)pass.size();
    // std::cout<<"lcs_match: "<<match<<"\n";
    tot_match_frac=std::max(tot_match_frac,temp);
  }
  if(!count)
  {
    return 0.0;
  }
  // tot_match_frac/=count;
  fin.close();
  return tot_match_frac;
}
void passcheck::check_lcs_special(const std::string& s)const 
{
  lcs temp_lcs(pass,s);
  double f= temp_lcs.do_lcs(0,0)/(double)pass.size();
  // std::cout<<"f: "<<f<<"\n";
  value=std::min(evaluate(1-f),value);
}
int passcheck::kmp(const std::string& s,const std::string& t)const
{
    int s_size=s.size(),t_size=t.size();
    if(s_size<t_size)
        return 0;
    std::vector<int> arr(t_size);
    int len=0;
    for(int i=0;i<t_size;++i)
    {
        if(!i)
        {
            arr[i]=0;
            continue;
        }
        while(1)
        {
            if(t[len]==t[i])
            {
                arr[i]=++len;
                break;
            }
            if(!len)
            {
                arr[i]=0;
                break;
            }
            len=arr[len-1];
        }
    }
    //printall(arr);
    int ans=0;
    for(int j=0,i=0;i<s_size;++i,++j)
    {
//        debug(i,j);
        if(s[i]==t[j])
        {
            if(j==t.length()-1)
            {
                //debug(i,j)
                j=arr[j]-1;
                //debug(j);
                ++ans;
            }
            continue;
        }
        if(!j)
        {
           --j;
           continue;
        }
        --i;
        j=arr[j-1]-1;
    }
    return ans;
}
double passcheck::pat_match_file()const
{
  std::ifstream fin(key_file,std::ios::in);
  int count=0;
  double tot_match_frac=0;
  while(!fin.eof())
  {
    ++count;
    std::string s;
    getline(fin,s);
    double temp=std::min((kmp(pass,s))*s.size()/(double)pass.size(),1.0);
    // std::cout<<"temp: "<<temp<<"\n";
    tot_match_frac=std::max(tot_match_frac,temp);
  }
  //tot_match_frac/=count;
  fin.close();
  return tot_match_frac;
}
void passcheck::pat_match_special(const std::string& s)const
{
  double f= std::min(kmp(pass,s)*s.size()/(double)pass.size(),1.0);
  value=std::min(value,evaluate(1-f));
}
void passcheck::main_strength()const
{
  double frac=std::max(pat_match_file(),check_lcs_file());
  // std::cout<<"frac: "<<frac<<"\n"; 
  value=evaluate(1-frac);
  check_lcs_special("01234567890");
  check_lcs_special("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  check_lcs_special("abcdefghijklmnopqrstuvwxyz");
}
passcheck::strong passcheck::evaluate(double d)const 
{
  int perc=std::min((int)(d*100),100);
  // std::cout<<"perc: "<<perc<<" d: "<<d<<"\n";
  if(perc>=80)
    return strong::very_good;
  else if(perc>=60)
    return strong::good;
  else if(perc>=40)
    return strong::medium;
  else if(perc>=20)
    return strong::bad;
  else
    return strong::very_bad;
}
passcheck::strong passcheck::get_strength()
{
  return value;
}