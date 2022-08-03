#include <iostream>
#include <cstdlib>
using namespace std;

int const ROBOT = 1;
int const HUMAN = 2;
int x = 0, y = 0;
int arr[3][3];

void conversion(int input)
{
  if (input == 1)
  {
    x = 0;
    y = 0;
  }
  if (input == 2)
  {
    x = 0;
    y = 1;
  }
  if (input == 3)
  {
    x = 0;
    y = 2;
  }
  if(input == 4)
  {
    x = 1;
    y = 0;
  }
  if (input == 5)
  {
    x = 1; 
    y = 1;
  }
  if(input == 6)
  {
    x = 1;
    y = 2;
  }
  if (input == 7)
  {
    x = 2;
    y = 0;
  }
  if (input == 8)
  {
    x = 2;
    y = 1;
  }
  if (input == 9)
  {
    x = 2;
    y = 2;
  }

}

void bestSpot(int index)
{
  arr[x][y] = HUMAN;
  bool center = false;
  bool other = false;

if (index == 1)
{
  if (x == 1 && y == 1)
  { //center
    arr[0][2] = ROBOT;
    center = true;
  }

  else // not center
  {
    if (x == 0 || y == 0)
    arr[2][2] = ROBOT;
    else 
    arr[0][0] = ROBOT;

    other = true;
  }
}
if (index == 2)
{
  if (center)
  {
  if (x == 0 && y == 0)
  arr[2][2] = ROBOT;

  else if (x == 2 && y == 2)
  arr[0][0] = ROBOT;
  
  else if (x == 0 && y == 1)
  arr[2][1] = ROBOT;
  else if (x == 1 && y == 0)
  arr[1][2] = ROBOT;
  else if (x == 1 && y == 2)
  arr[1][0] = ROBOT;
  else if (x == 2 && y == 1)
  arr[0][1] = ROBOT;
  }
  if (other)
  {
    if (arr[2][2] == ROBOT)
    {
      if (x == 2 && y == 1)
      {
        if(arr[0][0] == HUMAN)
        arr[0][2] = ROBOT;
        if (arr[0][2] == HUMAN)
        arr[0][0] = ROBOT;
        else 
        arr[1][1] = ROBOT;
      }
      else 
      arr[2][1] = ROBOT; //WINNER
    }
    if (arr[0][0] == ROBOT)
    {
      if (x == 1 && y == 0)
      {
        if (arr[1][2] == HUMAN)
        arr[1][1] = ROBOT;
        else if (arr[2][1] == HUMAN || arr[2][2] == HUMAN)
        arr[0][2] = ROBOT;
      }
      else 
      arr[0][1] = ROBOT; //WINNER
    }
    }
  }
if (index == 3) 
{
	if(center)
	{
		if(arr[0][0] == HUMAN) //WINNER
		{
			if(x == 1 && y == 2)
			arr[2][1] = ROBOT;
			else 
			arr[1][2] = ROBOT;
			
		}
		if(arr[2][2] == HUMAN) //WINNER
		{
			if(x == 0 && y == 1)
			arr[1][0] = ROBOT;
			else 
			arr[0][1] = ROBOT;
		}
		
		if(x == 2 && y == 2)
			arr[0][0] = ROBOT;
		if (x == 0 && y == 0)
			arr[2][2] = ROBOT;
		if (x == 0 && y == 1)
			arr[2][1] = ROBOT;
		if (x == 2 && y == 1)
			arr[0][1] = ROBOT;
	}
	if(other) //WINNER
	{
		if(arr[1][1] == ROBOT)
		{
			if(x == 0 && y == 0)
			arr[0][2] = ROBOT;
			else 
			arr[0][0] = ROBOT;
		}
		if (arr[0][2] == ROBOT)
		{
			if(x == 1 && y == 1)
			arr[1][2] = ROBOT;
			else 
			arr[1][1] = ROBOT;
		}	
		if (arr[0][0] == ROBOT)
		{
			if(x == 0 && y == 1)
			arr[1][1] = ROBOT;
			else 
			arr[0][1] = ROBOT;
		}
	}
if (index == 4) //TIE
{
	for(int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if(arr[row][col] != ROBOT && arr[row][col] != HUMAN)
			arr[row][col] = ROBOT;
		}
	}
	}
}
}


int main(){
int input;
arr[2][0] = ROBOT;


for (int index = 1; index <= 4; index++)
{
  cin >> input;
  conversion(input);
  bestSpot(index);

}


  return EXIT_SUCCESS;
}
