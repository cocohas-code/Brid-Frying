#include "MyPG.h"
#include "player_damege.h"

DG::Image::SP imgheart;
int damege;
int d_x, d_y;

void  damege_Initialize()
{
	imgheart = DG::Image::Create("./data/image/imagekari/heart.png");
	d_x = 0;
	d_y = 0;
}
void  damege_Finalize()
{
	imgheart.reset();
}
void  damege_UpDate()
{

}
void  damege_Render2D(int x)
{

	for (int i = 0; i < x; ++i)
	{
		ML::Box2D draw(0+i*80, 58, 50, 42);
		draw.Offset(d_x, d_y);
		ML::Box2D src(0, 0, 50, 42);
		imgheart->Draw(draw, src);
	}
}