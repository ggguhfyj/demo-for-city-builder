#pragma once

// the basis for all gridMap occupiers. implement a basic is full and is destoryed fuction to be used.
struct GridCell
{
	GridCell()//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{

	}
	GridCell(double height)//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{
		Height = height;
	}

	GridCell(double height, int x_param, int y_param)//should accept a normalized height value between 0 and 1 from the perlin noisemap
	{
		Height = height;
		x = x_param;
		y = y_param;
	}

	// normal gridcells hold information about the terrian it is situated on, if it is being used by some building or person and etc
	// public functions > getter and setter functions to be called when creating or changing the gridcell.


	void setHeight(double height) { Height = height; }
	double getHeight() const { return Height; }
	


	int x = 0, y = 0;
private:
	double Height = 0.0; 
};  