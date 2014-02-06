#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

template<class T>
class Cons
{
public:
	T car;
	Cons<T>* cdr;
	
	Cons(T car, Cons<T>* cdr)
	{
		this->car = car;
		this->cdr = cdr;
	}
};

template<class T>
class RandomList
{
public:
	//Create an empty list
	RandomList()
	{
		lst = NULL;
	}
	//Add an item to the list in a random position
	void put(T dat)
	{	
		double val;	
		Cons<T>* tmp = lst;
		
		//If empty or if random condition is met, put new element at beginning of list
		if(lst == NULL || (rand() % 127) > 20)
			lst = new Cons<T>(dat, lst);
		//If not empty and if not adding to beginning...
		else				
			while(1)
			{
				//If the end of the list is empty or condition is met, add to the end of the list
				if(tmp->cdr == NULL || (rand() % 127) > 20)
				{
					tmp->cdr = new Cons<T>(dat, tmp->cdr);
					
					return;
				}
				
				//Shift to the next element and try to find new position to add new element
				tmp = tmp->cdr;
			}
	}

	//Return if the list is empty or not
	bool isEmpty()
	{
		return (lst == NULL);
	}
	
	//Grab the beginning of the list
	T get()
	{
		Cons<T>* tmp = lst;
		lst = lst->cdr;
		
		T ret = tmp->car;
		
		delete tmp;
		
		return ret;
	}
	
private:
	Cons<T>* lst;
	
};