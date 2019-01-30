#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdint.h>
#include "getClock.h"
#include "darts.h"
#include <map>

using namespace darts;

class cd1 : public Codelet
{
public:
  cd1(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This codelet is the continuation of the intiated split phase
class cd2 : public Codelet
{
public:
  cd2(uint32_t dep, uint32_t res, ThreadedProcedure * myTP, uint32_t stat):
  Codelet(dep,res,myTP,stat) { }
  
  virtual void fire(void);
};

//This is the Fib threaded proceduer
class fib : public ThreadedProcedure
{
public:
    //These are the frame variables
    int num;
    int x1, x2;
    int y1, y2;
    int * result;
    cd1 check;
    cd2 adder;
    //This is who to signal when complete
    Codelet * toSignal;
	int redundant [4];   
	int index = 0; 

    //Constructor
    fib(int n, int * res, Codelet * toSig):
    ThreadedProcedure(),
    num(n),
    x1(0), x2(0),
    y1(0), y2(0),
    result(res),
    check(0,0,this,SHORTWAIT),
    adder(4,4,this,LONGWAIT),
    toSignal(toSig) 
    {  
        add(&check); 
        add(&adder);
    }
    
	bool matchingPairs(int arr[4]);
	void clearRedundant();
    //~fib(void)
    //{
    //    std::cout << "dieing "<< num << std::endl;
    //}
    
};

//checks for matching pairs
bool fib::matchingPairs(int arr[4])
{
	//<fibNum, duplicates>
	std::map<int, int> count;
	for(int i = 0; i < 4; i++){
		if(count.find(arr[i]) != count.end())
			count.find(arr[i])->second = count.find(arr[i])->second + 1;
		else
			count.emplace(arr[i], 1);
	}	
	//exactly 2 pairs
	if(count.size() == 2)
		return true;
	// fib(1) == fib(0), so all 4 values = 1
	else if(count.size() == 1 && count.begin()->first == 1)
		return true;
	else 
		return false;

}

void fib::clearRedundant()
{
	for(int i =0; i < 4; i++)
		redundant[i] = -1;
}

void
cd1::fire(void)
{
    fib * myFib = static_cast<fib*>(myTP_);
    
    //std::cout << "check " << myFib->num << std::endl;
    
    if(myFib->num<2)
    {
        (*myFib->result) = myFib->num;
        myFib->toSignal->decDep();
    }
    else
    {
		//always false
		if(rand() % 3 == 0) {
			invoke<fib>(myFib,-100,&myFib->x1,&myFib->adder);
			invoke<fib>(myFib,myFib->num - 2,&myFib->y1,&myFib->adder);

			//redundant threads
			invoke<fib>(myFib,myFib->num - 1,&myFib->x2,&myFib->adder);
		    invoke<fib>(myFib,myFib->num - 2,&myFib->y2,&myFib->adder);
		}
		else {        
			invoke<fib>(myFib,myFib->num - 1,&myFib->x1,&myFib->adder);
		    invoke<fib>(myFib,myFib->num - 2,&myFib->y1,&myFib->adder);
			//redundant threads
			invoke<fib>(myFib,myFib->num - 1,&myFib->x2,&myFib->adder);
		    invoke<fib>(myFib,myFib->num - 2,&myFib->y2,&myFib->adder);
		}
    }
}
 
void 
cd2::fire(void)
{ 
    fib * myFib = static_cast<fib*>(myTP_);    
   // std::cout << "add " << myFib->num << std::endl;//<< " x: " << myFib->x <<" y: " << myFib->y << std::endl;

	//add matching pairs
	myFib->redundant[myFib->index++] = myFib->x1;
	myFib->redundant[myFib->index++] = myFib->y1;
	myFib->redundant[myFib->index++] = myFib->x2;
	myFib->redundant[myFib->index++] = myFib->y2;

	
	if(myFib->matchingPairs(myFib->redundant)){
		(*myFib->result) = myFib->x1 + myFib->y1;
		myFib->toSignal->decDep();
	} else{
		//printf("RST ");
		//parent, input to TP, results, toSignal		
		invoke<fib>(myFib,*myFib->result,&myFib->num,&myFib->check);
		//myFib->clearRedundant();
		
	}
	myFib->index = 0;
	
}

int
main(int argc, char *argv[])
{
  int fibnum;
  int result = 0;
  uint64_t start, end;

  if(argc!=2)
  {
    std::cout << "error need a num" << std::endl;
    return 0;
  }
  
  fibnum = atoi(argv[1]);
  Runtime rt;
  for(int i = 0; i<fibnum+1; i++)
  {
    result = 0;
    std::cout << "fib(" << std::setw (2) << i << ") ";
    start = getTime();
    rt.run(launch<fib>(i,&result, &Runtime::finalSignal));
    end = getTime();
    std::cout << std::setw (6) << result;
    std::cout << " Total Time Taken: " << std::setw (12) << (end-start)/1000000 << " ms" << std::endl;
  }
  return 0;
}
