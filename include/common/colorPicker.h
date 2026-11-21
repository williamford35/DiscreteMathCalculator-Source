#include <wx/wx.h>
#include <wx/colour.h>

#include <vector>
using namespace std;

struct color
{
	int r;
	int g;
	int b;
};

class colorPicker
{
public:
	colorPicker()
	{}

	void PickColor(vector<wxColour>& v_picked_colors, int count)
	{
		v_picked_colors.clear();

		for (int k = 0; k < count; k++)
			v_picked_colors.push_back(wxColour(rgbValues[k].r,rgbValues[k].g,rgbValues[k].b));

	}

private:
	int currIndx = 0;
	vector<wxColour> v_picked_colors;

	struct color rgbValues[16] =
	{
		/* Golden rod, Red, Lime, Orange*/
		{218,165,32},{255,0,0},{0,255,0},{255,165,0},
		/* Blue, Yellow, Cyan/Aqua, Magenta/Fuchsia */
		{0,0,255},{255,255,0},{0,255,255},{255,0,255},
		/* Silver, Gray, Maroon, Olive */
		{192,192,192},{128,128,128},{128,0,0},{128,128,0},
		/* Green, Purple, Teal, Navy */
		{0,128,0},{128,0,128},{0,128,128},{0,0,128}
	};
};
